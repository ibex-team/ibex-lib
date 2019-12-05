#include "ibex_LPSolver.h"

namespace {

soplex::DSVectorReal ivec2dsvec(const ibex::Vector& ivec) {
    soplex::DSVectorReal dsvec(ivec.size());
    for(int i = 0; i < ivec.size(); ++i) {
        dsvec.add(i, ivec[i]);
    }
    return dsvec;
}

ibex::Vector dsvec2ivec(const soplex::DSVectorReal& dsvec) {
    ibex::Vector ivec(dsvec.dim());
    for(int i = 0; i < dsvec.size(); ++i) {
        ivec[dsvec.index(i)] = dsvec.value(i);
    }
    return ivec;
}

soplex::DVectorReal ivec2dvec(const ibex::Vector& ivec) {
    return soplex::DVectorReal(VectorReal(ivec.size(), ivec.raw()));
}

ibex::Vector dvec2ivec(const soplex::DVectorReal& dvec) {
    return ibex::Vector(dvec.dim(), dvec.get_const_ptr());
}

ibex::Vector rawvec2ivec(const soplex::VectorReal& dvec) {
    return ibex::Vector(dvec.dim(), dvec.get_const_ptr());
}

soplex::LPRowBase::Type cmpop2type(ibexCmpOp op) {
    using Type = soplex::LPRowBase::Type;
    switch(op) {
    case LT:
    case LEQ:
        return Type::LESS_EQUAL;
    case GT:
    case GEQ:
        return Type::GREATER_EQUAL;
    case EQ:
        return Type::EQUAL;
    }
    ibex::ibex_error("Unreachable");

}

} /* end anonymous namespace */

namespace ibex {

using namespace soplex;

LPSolver::LPSolver(int vars_count=0, Sense sense=Sense::Minimize, double precision=default_precision,
        int timeout=default_timeout, int max_iter=default_max_iter) : sense_(sens), status_(LPSolver::Status::Unknown)
{
    mysoplex = new soplex::SoPlex();
	mysoplex->setIntParam(SoPlex::VERBOSITY, SoPlex::VERBOSITY_ERROR);

	mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
	mysoplex->setIntParam(SoPlex::ITERLIMIT, max_iter);
	mysoplex->setRealParam(SoPlex::TIMELIMIT, timeout);
	mysoplex->setRealParam(SoPlex::FEASTOL, tolerance);
	mysoplex->setRealParam(SoPlex::OPTTOL, tolerance);
	mysoplex->setIntParam(SoPlex::SOLVEMODE, SoPlex::SOLVEMODE_REAL);

    if(vars_count > 0) {
        add_variables(IntervalVector(vars_count, Interval::ALL_REALS), Vector(vars_count, 0.0));
    }
}

void LPSolver::add_variable(const Interval& bounds, double obj) {
    Vector col(rows_count(), 0.0);
    return add_variable(col, bounds, obj);
}

void LPSolver::add_variable(const Vector& col, const Interval& bounds, double obj) {
    invalidate();
    const int n = vars_count();
    DSVectorReal dscol = ivec2dsvec(col);
    mysoplex->addColReal(LPColReal(obj, dscol, bounds.ub(), bounds.lb()));

    ivec_bounds_.resize(n+1);
    ivec_bounds_[n] = bounds;

    // Add bound constraint to the LP
    bounds_ctrs_.emplace_back(-1);
    add_bound_constraint(n, bounds);
}

void LPSolver::add_variables(const IntervalVector& bounds, const Vector& obj) {
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(bounds[i], obj[i]);
    }
}

void LPSolver::add_variables(const Matrix& cols, const IntervalVector& bounds, const Vector& obj) {
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(cols.col(i), bounds[i], obj[i]);
    }
}

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
    invalidate();
    mysoplex->addRowReal(LPRowReal(lhs, ivec2dsvec, rhs));
    return rows_count()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    invalidate();
    using Type = soplex::LPRowBase::Type;
    Type type = cmpop2type(op);
    DSVectorReal dsrow = ivec2dsvec(row);
    mysoplex->addRowReal(LPRowReal(dsrow, type, rhs));
    return rows_count()-1;
}

void LPSolver::add_bound_constraint(int var, const Interval& bounds) {
    DSVectorReal dsrow(vars_count());
    dsrow.add(var, 1);
    bounds_ctrs_[var] = mysoplex->addRowReal(LPRowReal(bounds.lb(), dsrow, bounds.ub()));
}

void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
    for(int i = 0; i < lhs.size(); ++i) {
        add_constraint(lhs[i], rows.row(i), rhs[i]);
    }
}

void LPSolver::add_constraints(const Matrix& rows, const std::vector<CmpOp>& ops, const Vector& rhs) {
    for(int i = 0; i < rhs.size(); ++i) {
        add_constraint(rows.row(i), ops[i], rhs[i]);
    }
}

Status LPSolver::optimize(PostProcessing pp) {
    invalidate();
    SPxSolver soplex_status = mysoplex->solve();
    status_ = LPSolver::Status::Unknown;
    switch(soplex_status) {
    case SPxSolver::OPTIMAL:
        {
            DVectorReal dvec_primal, dvec_dual;
            mysoplex->getPrimalReal(dvec_primal);
            mysoplex->getDualReal(dvec_dual);
            uncertified_dual_ = dvec2ivec(dvec_dual);
            uncertified_primal_ = dvec2ivec(dvec_primal);
            has_solution_ = true;
            if(pp == PostProcessing::NeumaierShcherbina) {
                // Neumaier Shcherbina cannot fail
                neumaier_shcherbina_postprocessing();
                status_ = LPSolver::Status::OptimalProved;
            } else {
                status_ LPSolver::Status::Optimal;
            }
        }
        break;
    case SPxSolver::ABORT_TIME:
        status_ = LPSolver::Status::Timeout;
        break;
    case SPxSolver::ABORT_ITER:
        status_ = LPSolver::Status::MaxIter;
        break;
    case SPxSolver::UNBOUNDED:
        status_ = LPSolver::Status::Unbounded;
        break;
    case SPxSolver::INFEASIBLE:
        {
            status_ = LPSolver::Status::Infeasible;
            if(pp == PostProcessing::NeumaierShcherbina) {
                bool infeasible_proved = neumaier_shcherbina_infeasibility_test();
                if(infeasible_proved) {
                    status_ = LPSolver::Status::InfeasibleProved;
                }
            }
        }
        break;
    default:
        ibex_warning("LPSolver: solve status is an internal Soplex status. This is probably an error for you.");
        status_ = LPSolver::Status::Unknown;
    }
    return status_;
}

void LPSolver::set_sense(LPSolver::Sense new_sense) {
    if(sense_ != new_sense) {
        invalidate();
        switch(new_sense) {
        case LPSolver::Sense::Minimize:
            mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
            break;
        case LPSolver::Sense::Maximize:
            mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MAXIMIZE);
            break;
        }
        sense_ = new_sense;
    }
}

void LPSolver::set_obj(const Vector& obj) {
    invalidate();
    mysoplex->changeObjReal(ivec2dvec(obj));
}

void LPSolver::set_var_obj(int index, double value) {
    invalidate();
    mysoplex->changeObjReal(index, value);
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
    invalidate();
    ivec_bounds_ = bounds;
    mysoplex->changeBoundsReal(ivec2dvec(bounds.lower()), ivec2dvec(bounds.upper()));
    int var = 0;
    for(int i : bounds_ctrs_) {
        mysoplex->changeRangeReal(i, bounds[var].lb(), bounds[var].ub());
        ++var;
    }
}

void LPSolver::set_var_bounds(int var, const Interval& bounds) {
    invalidate();
    ivec_bounds_[var] = bounds;
    mysoplex->changeBoundsReal(var, bounds.lb(), bounds.ub());
    mysoplex->changeRangeReal(bounds_ctrs_[var], bounds.lb(), bounds.ub());
}

void LPSolver::set_tolerance(double tolerance) {
    invalidate();
	mysoplex->setRealParam(SoPlex::FEASTOL, tolerance);
	mysoplex->setRealParam(SoPlex::OPTTOL, tolerance);
}

void LPSolver::set_timeout(double timeout) {
    invalidate();
    mysoplex->setRealParam(SoPlex::TIMELIMIT, timeout);
}

void LPSolver::set_max_iter(int max_iter) {
    invalidate();
	mysoplex->setIntParam(SoPlex::ITERLIMIT, max_iter);
}

int LPSolver::rows_count() const {
    return mysoplex->numRowsReal();
}

int LPSolver::vars_count() const {
    return mysoplex->numColsReal();
}

double LPSolver::tolerance() const {
    return mysoplex->realParam(SoPlex::FEASTOL);
}

int LPSolver::max_iter() const {
    return mysoplex->intParam(SoPlex::ITERLIMIT);
}

double LPSolver::timeout() const {
    return mysoplex->realParam(SoPlex::TIMELIMIT);
}

Status LPSolver::status() const {
    return status_;
}

Matrix LPSolver::rows() const {
    Matrix m(rows_count(), vars_count());
    for(int i = 0; i < rows_count(); ++i) {
        m.set_row(i, row(i));
    }
    return m;
}

Vector LPSolver::row(int index) const {
    DSVectorReal dsrow(vars_count);
    mysoplex->getRowVectorReal(index, dsrow);
    return dsvec2ivec(dsrow);
}

Matrix LPSolver::rows_transposed() const {
    return rows().transposed();
}

Vector LPSolver::col(int index) const {
    DSVectorReal dscol(vars_count);
    mysoplex->getColVectorReal(index, dscol);
    return dsvec2ivec(dscol);
}

Vector LPSolver::lhs() const {
    DVectorReal dcol(vars_count);
    mysoplex->getLhsReal(index, dcol);
    return dvec2ivec(dcol);
}

double LPSolver::lhs(int index) const {
    return mysoplex->lhsReal(index);
}

Vector LPSolver::rhs() const {
    DVectorReal dcol(vars_count);
    mysoplex->getRhsReal(index, dcol);
    return dvec2ivec(dcol);
}
double LPSolver::rhs(int index) const {
    return mysoplex->rhsReal(index);
}

IntervalVector LPSolver::lhs_rhs() const {
    IntervalVector lhs_rhs_vec(rows_count());
    for(int i = 0; i < lhs_rhs_vec.size(); ++i) {
        lhs_rhs_vec[i] = Interval(mysoplex->lhsReal(i), mysoplex->rhsReal(i));
    }
    return lhs_rhs_vec;
}

Interval LPSolver::lhs_rhs(int index) const {
    return Interval(mysoplex->lhsReal(i), mysoplex->rhsReal(i));
}

IntervalVector LPSolver::bounds() const {
    return ivec_bounds_;
}

Interval LPSolver::bounds(int index) const {
    return ivec_bounds_[index];
}

Vector LPSolver::obj() const {
    VectorReal vec;
    mysoplex->getObjReal(vec);
    return rawvec2ivec(vec);
}

Vector LPSolver::var_obj(int index) const {
    return mysoplex->objReal(index);
}

Vector LPSolver::certified_obj() const {
    if(certified_obj_ == nullptr) {
        if(has_solution_)
            ibex_error("LPSolver: no certified objective found. Apply a postprocessing step.");
        else
            ibex_error("LPSolver: no solution found.")
    }
    return *certified_obj_;
}

Vector LPSolver::uncertified_obj() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored.");
    }
    return uncertified_obj_;
}

Vector LPSolver::uncertified_primal_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored.");
    }
    return primal_;
}
Vector LPSolver::uncertified_dual_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored.");
    }
    return dual_;
}

bool LPSolver::uncertified_infeasible_dir(Vector& infeasible_dir) const {
    if(uncertified_infeasible_dir_ != nullptr) {
        infeasible_dir = *uncertified_infeasible_dir;
        return true;
    }
    return return false;
}

bool LPSolver::certified_infeasible_dir(Vector& infeasible_dir) const {
    if(certified_infeasible_dir_ != nullptr) {
        infeasible_dir = *certified_infeasible_dir;
        return true;
    }
    return return false;
}

bool LPSolver::is_inner_point(const Vector& point) const {
    ibex_not_implemented("Not implemented");
}

void LPSolver::write_to_file(const std::string& filename) const {
    mysoplex->writeFileReal(name.c_str(), NULL, NULL, NULL);
}

// Clear functions
void LPSolver::clear_obj() {
    invalidate();
    DVectorReal new_obj(vars_count());
    new_obj.clear();
    mysoplex->setObjReal(new_obj);
}

void LPSolver::clear_constraints() {
    invalidate();
    mysoplex->removeRowRangeReal(0, rows_count()-1);
    const int n = vars_count();
    for(int i = 0; i < n; ++i) {
        add_bound_constraint(i, ivec_bounds_[i]);
    }
}

void LPSolver::clear_bounds() {
    invalidate()
    IntervalVector new_bounds(vars_count(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}

} /* end namespace ibex */