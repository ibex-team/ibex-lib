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
    ibex::Vector ivec_copy(ivec);
    return soplex::DVectorReal(soplex::VectorReal(ivec.size(), ivec_copy.raw()));
}

ibex::Vector dvec2ivec(const soplex::DVectorReal& dvec) {
    double copy[dvec.dim()];
    std::copy(dvec.get_const_ptr(), dvec.get_const_ptr()+dvec.dim(), copy);
    return ibex::Vector(dvec.dim(), copy);
}

ibex::Vector rawvec2ivec(const soplex::VectorReal& dvec) {
    double copy[dvec.dim()];
    std::copy(dvec.get_const_ptr(), dvec.get_const_ptr()+dvec.dim(), copy);
    return ibex::Vector(dvec.dim(), copy);
}

soplex::LPRowReal::Type cmpop2type(ibex::CmpOp op) {
    using Type = soplex::LPRowReal::Type;
    switch(op) {
    case ibex::LT:
    case ibex::LEQ:
        return Type::LESS_EQUAL;
    case ibex::GT:
    case ibex::GEQ:
        return Type::GREATER_EQUAL;
    case ibex::EQ:
        return Type::EQUAL;
    }
    ibex::ibex_error("Unreachable");

}

} /* end anonymous namespace */

namespace ibex {

using namespace soplex;

LPSolver::LPSolver(int vars_count, Sense sense, double tolerance,
        double timeout, int max_iter) : sense_(sense), status_(LPSolver::Status::Unknown)
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

LPSolver::~LPSolver() {
    delete mysoplex;
}

void LPSolver::add_variable(const Interval& bounds, double obj) {
    Vector col(rows_count(), 0.0);
    return add_variable(col, bounds, obj);
}

void LPSolver::add_variable(const Vector& col, const Interval& bounds, double obj) {
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
    mysoplex->addRowReal(LPRowReal(lhs, ivec2dsvec(row), rhs));
    return rows_count()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    using Type = soplex::LPRowReal::Type;
    Type type = cmpop2type(op);
    DSVectorReal dsrow = ivec2dsvec(row);
    mysoplex->addRowReal(LPRowReal(dsrow, type, rhs));
    return rows_count()-1;
}

void LPSolver::add_bound_constraint(int var, const Interval& bounds) {
    DSVectorReal dsrow(vars_count());
    dsrow.add(var, 1);
    mysoplex->addRowReal(LPRowReal(bounds.lb(), dsrow, bounds.ub()));
    bounds_ctrs_[var] = rows_count()-1;
}

void LPSolver::add_constraints(const Vector& lhs, const Matrix& rows, const Vector& rhs) {
    for(int i = 0; i < lhs.size(); ++i) {
        add_constraint(lhs[i], rows.row(i), rhs[i]);
    }
}

void LPSolver::add_constraints(const Matrix& rows, CmpOp op, const Vector& rhs) {
    for(int i = 0; i < rhs.size(); ++i) {
        add_constraint(rows.row(i), op, rhs[i]);
    }
}

LPSolver::Status LPSolver::optimize(PostProcessing pp) {
    invalidate();
    SPxSolver::Status soplex_status = mysoplex->solve();
    status_ = LPSolver::Status::Unknown;
    switch(soplex_status) {
    case SPxSolver::OPTIMAL:
        {
            DVectorReal dvec_primal(vars_count());
            DVectorReal dvec_dual(vars_count());
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
                status_ = LPSolver::Status::Optimal;
            }
        }
        break;
    case SPxSolver::ABORT_TIME:
        status_ = LPSolver::Status::Timeout;
        break;
    case SPxSolver::ABORT_ITER:
        status_ = LPSolver::Status::MaxIter;
        break;
    case SPxSolver::INFEASIBLE:
        {
            status_ = LPSolver::Status::Infeasible;
            DVectorReal dual_farkas(vars_count());
            bool b = mysoplex->getDualFarkasReal(dual_farkas);
            if(b) {
                uncertified_infeasible_dir_ = new Vector(dvec2ivec(dual_farkas));
            }
            if(pp == PostProcessing::NeumaierShcherbina) {
                bool infeasible_proved = neumaier_shcherbina_infeasibility_test();
                if(infeasible_proved) {
                    status_ = LPSolver::Status::InfeasibleProved;
                }
            }
        }
        break;
    case SPxSolver::UNBOUNDED:
        status_ = LPSolver::Status::Unbounded;
        break;
    default:
        ibex_warning("LPSolver: solve status is an internal Soplex status. This is probably an error for you.");
        status_ = LPSolver::Status::Unknown;
    }
    return status_;
}

void LPSolver::set_sense(LPSolver::Sense new_sense) {
    if(sense_ != new_sense) {
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
    mysoplex->changeObjReal(ivec2dvec(obj));
}

void LPSolver::set_var_obj(int index, double value) {
    mysoplex->changeObjReal(index, value);
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
    ivec_bounds_ = bounds;
    mysoplex->changeBoundsReal(ivec2dvec(bounds.lb()), ivec2dvec(bounds.ub()));
    int var = 0;
    for(int i : bounds_ctrs_) {
        mysoplex->changeRangeReal(i, bounds[var].lb(), bounds[var].ub());
        ++var;
    }
}

void LPSolver::set_var_bounds(int var, const Interval& bounds) {
    ivec_bounds_[var] = bounds;
    mysoplex->changeBoundsReal(var, bounds.lb(), bounds.ub());
    mysoplex->changeRangeReal(bounds_ctrs_[var], bounds.lb(), bounds.ub());
}

void LPSolver::set_tolerance(double tolerance) {
	mysoplex->setRealParam(SoPlex::FEASTOL, tolerance);
	mysoplex->setRealParam(SoPlex::OPTTOL, tolerance);
}

void LPSolver::set_timeout(double timeout) {
    mysoplex->setRealParam(SoPlex::TIMELIMIT, timeout);
}

void LPSolver::set_max_iter(int max_iter) {
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

LPSolver::Status LPSolver::status() const {
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
    DSVectorReal dsrow(vars_count());
    mysoplex->getRowVectorReal(index, dsrow);
    return dsvec2ivec(dsrow);
}

Matrix LPSolver::rows_transposed() const {
    return rows().transpose();
}

Vector LPSolver::col(int index) const {
    DSVectorReal dscol(vars_count());
    mysoplex->getColVectorReal(index, dscol);
    return dsvec2ivec(dscol);
}

Vector LPSolver::lhs() const {
    DVectorReal dcol(vars_count());
    mysoplex->getLhsReal(dcol);
    return dvec2ivec(dcol);
}

double LPSolver::lhs(int index) const {
    return mysoplex->lhsReal(index);
}

Vector LPSolver::rhs() const {
    DVectorReal dcol(vars_count());
    mysoplex->getRhsReal(dcol);
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
    return Interval(mysoplex->lhsReal(index), mysoplex->rhsReal(index));
}

IntervalVector LPSolver::bounds() const {
    return ivec_bounds_;
}

Interval LPSolver::bounds(int index) const {
    return ivec_bounds_[index];
}

Vector LPSolver::obj_vec() const {
    DVectorReal vec(vars_count());
    mysoplex->getObjReal(vec);
    return dvec2ivec(vec);
}

Vector LPSolver::var_obj(int index) const {
    return mysoplex->objReal(index);
}

Interval LPSolver::certified_obj() const {
    if(certified_obj_ == nullptr) {
        throw LPException();
    }
    return *certified_obj_;
}

Interval LPSolver::uncertified_obj() const {
    if(!has_solution_) {
        throw LPException();
    }
    return uncertified_obj_;
}

Vector LPSolver::uncertified_primal_sol() const {
    if(!has_solution_) {
        throw LPException();
    }
    return uncertified_primal_;
}
Vector LPSolver::uncertified_dual_sol() const {
    if(!has_solution_) {
        throw LPException();
    }
    return uncertified_dual_;
}

bool LPSolver::uncertified_infeasible_dir(Vector& infeasible_dir) const {
    if(uncertified_infeasible_dir_ != nullptr) {
        infeasible_dir = *uncertified_infeasible_dir_;
        return true;
    }
    return false;
}

bool LPSolver::is_inner_point(const Vector& point) const {
    not_implemented("Not implemented");
}

void LPSolver::write_to_file(const std::string& filename) const {
    mysoplex->writeFileReal(filename.c_str(), NULL, NULL, NULL);
}

// Clear functions
void LPSolver::clear_obj() {
    DVectorReal new_obj(vars_count());
    new_obj.clear();
    mysoplex->changeObjReal(new_obj);
}

void LPSolver::clear_constraints() {
    mysoplex->removeRowRangeReal(0, rows_count()-1);
    const int n = vars_count();
    for(int i = 0; i < n; ++i) {
        add_bound_constraint(i, ivec_bounds_[i]);
    }
}

void LPSolver::clear_bounds() {
    IntervalVector new_bounds(vars_count(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}

} /* end namespace ibex */