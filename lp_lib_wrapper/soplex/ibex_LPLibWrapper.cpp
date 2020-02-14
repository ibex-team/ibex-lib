#include "ibex_LPSolver.h"

namespace {

soplex::DSVectorReal ivec2dsvec(const ibex::Vector& ivec) {
    soplex::DSVectorReal dsvec(ivec.size());
    for(int i = 0; i < ivec.size(); ++i) {
        dsvec.add(i, ivec[i]);
    }
    return dsvec;
}

ibex::Vector dsvec2ivec(const soplex::DSVectorReal& dsvec, int size) {
    ibex::Vector ivec(size);
    for(int i = 0; i < dsvec.size(); ++i) {
        ivec[dsvec.index(i)] = dsvec.value(i);
    }
    return ivec;
}

soplex::DVectorReal ivec2dvec(const ibex::Vector& ivec) {
    /*soplex::DVectorReal dvec(ivec.size());
    for(int i = 0; i < ivec.size(); ++i) {
        dvec[i] = ivec[i];
    }
    return dvec;*/
    ibex::Vector ivec_copy(ivec);
    return soplex::DVectorReal(soplex::VectorReal(ivec.size(), ivec_copy.raw()));
}

ibex::Vector dvec2ivec(const soplex::DVectorReal& dvec) {
    /*ibex::Vector ivec(dvec.dim());
    for(int i = 0 ; i < ivec.size(); ++i) {
        ivec[i] = dvec[i];
    }
    return ivec;*/
    double copy[dvec.dim()];
    std::copy(dvec.get_const_ptr(), dvec.get_const_ptr()+dvec.dim(), copy);
    return ibex::Vector(dvec.dim(), copy);
}

/*ibex::Vector rawvec2ivec(const soplex::VectorReal& dvec) {
    double copy[dvec.dim()];
    std::copy(dvec.get_const_ptr(), dvec.get_const_ptr()+dvec.dim(), copy);
    return ibex::Vector(dvec.dim(), copy);
}*/

// Convert Ibex operator type to soplex operator type.
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

bool isfinite(const ibex::Vector& v) {
    for(int i = 0; i < v.size(); ++i) {
        if(!std::isfinite(v[i])) {
            return false;
        }
    }
    return true;
}

} /* end anonymous namespace */

namespace ibex {

using namespace soplex;

LPSolver::LPSolver(int nb_vars, LPSolver::Mode mode, double tolerance,
        double timeout, int max_iter)
{
    assert(nb_vars > 0);
    init(mode, tolerance, timeout, max_iter);


    reset(nb_vars);
}

LPSolver::LPSolver(std::string filename) {
    init(LPSolver::Mode::NotCertified, LPSolver::default_tolerance, LPSolver::default_timeout, LPSolver::default_max_iter);
    mysoplex->setIntParam(SoPlex::READMODE, SoPlex::READMODE_REAL);
    bool result = mysoplex->readFile(filename.c_str());
    if(!result) {
        std::string msg = "LPSolver: file " + filename + " could not be read.";
        ibex_error(msg.c_str());
    }
    ivec_bounds_ = IntervalVector(nb_vars());
    for(int i = 0; i < ivec_bounds_.size(); ++i) {
        ivec_bounds_[i] = Interval(mysoplex->lowerReal(i), mysoplex->upperReal(i));
    }
}

LPSolver::~LPSolver() {
    delete mysoplex;
}

void LPSolver::init(LPSolver::Mode mode, double tolerance, double timeout, int max_iter) {
    mysoplex = new soplex::SoPlex();
    mode_ = mode;
    mysoplex->setIntParam(SoPlex::VERBOSITY, SoPlex::VERBOSITY_ERROR);
    mysoplex->setIntParam(SoPlex::ITERLIMIT, max_iter);
	mysoplex->setRealParam(SoPlex::TIMELIMIT, timeout);
	mysoplex->setRealParam(SoPlex::FEASTOL, tolerance);
	mysoplex->setRealParam(SoPlex::OPTTOL, tolerance);
	mysoplex->setIntParam(SoPlex::SOLVEMODE, SoPlex::SOLVEMODE_REAL);
    mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
    mysoplex->setBoolParam(SoPlex::ENSURERAY, true);
}

/*void LPSolver::add_variable(const Interval& bounds, double obj) {
    Vector col(nb_rows(), 0.0);
    return add_variable(col, bounds, obj);
}

void LPSolver::add_variable(const Vector& col, const Interval& bounds, double obj) {
    assert(col.size() == nb_vars());
    const int n = nb_vars();
    DSVectorReal dscol = ivec2dsvec(col);
    mysoplex->addColReal(LPColReal(obj, dscol, bounds.ub(), bounds.lb()));

    ivec_bounds_.resize(n+1);
    ivec_bounds_[n] = bounds;

    // Add bound constraint to the LP
    bounds_ctrs_.emplace_back(-1);
    add_bound_constraint(n, bounds);
}

void LPSolver::add_variables(const IntervalVector& bounds, const Vector& obj) {
    assert(obj.size() == bounds.size());
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(bounds[i], obj[i]);
    }
}

void LPSolver::add_variables(const Matrix& cols, const IntervalVector& bounds, const Vector& obj) {
    for(int i = 0; i < bounds.size(); ++i) {
        add_variable(cols.col(i), bounds[i], obj[i]);
    }
}*/

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
    assert(row.size() == nb_vars());
    assert(std::isfinite(lhs) && std::isfinite(rhs));
    assert(isfinite(row));

    has_changed = true;
    mysoplex->addRowReal(LPRowReal(lhs, ivec2dsvec(row), rhs));
    return nb_rows()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    assert(row.size() == nb_vars());
    assert(isfinite(row));
    assert(std::isfinite(rhs));

    has_changed = true;
    using Type = soplex::LPRowReal::Type;
    Type type = cmpop2type(op);
    DSVectorReal dsrow = ivec2dsvec(row);
    mysoplex->addRowReal(LPRowReal(dsrow, type, rhs));
    return nb_rows()-1;
}

/*void LPSolver::add_bound_constraint(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    DSVectorReal dsrow(nb_vars());
    dsrow.add(var, 1);
    mysoplex->addRowReal(LPRowReal(bounds.lb(), dsrow, bounds.ub()));
    bounds_ctrs_[var] = nb_rows()-1;
}*/

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

LPSolver::Status LPSolver::minimize() {
    invalidate();
    assert(!ivec_bounds_.is_unbounded());

    mysoplex->solve();
    mysoplex->ignoreUnscaledViolations();
    SPxSolver::Status soplex_status = mysoplex->status();
    status_ = LPSolver::Status::Unknown;
    switch(soplex_status) {
    case SPxSolver::OPTIMAL:
        {
            DVectorReal dvec_primal(nb_vars());
            DVectorReal dvec_dual(nb_rows());
            mysoplex->getPrimalReal(dvec_primal);
            mysoplex->getDualReal(dvec_dual);
            obj_ = mysoplex->objValueReal();

            uncertified_dual_ = dvec2ivec(dvec_dual);
            uncertified_primal_ = dvec2ivec(dvec_primal);
            has_solution_ = true;
            if(mode_ == LPSolver::Mode::Certified) {
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
            DVectorReal dual_farkas(nb_rows());
            bool b = mysoplex->getDualFarkasReal(dual_farkas);
            if(b) {
                uncertified_infeasible_dir_ = dvec2ivec(dual_farkas);
                has_infeasible_dir_ = true;
            }
            if(mode_ == LPSolver::Mode::Certified) {
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
    	// TODO: we need an option to log such warnings, see issue #440.
        //std::string error_msg = "LPSolver: solve status is an internal Soplex status (" + std::to_string(soplex_status) + "). This is probably an error for you.";
        //ibex_warning(error_msg.c_str());
        status_ = LPSolver::Status::Unknown;
    }
    return status_;
}

void LPSolver::set_cost(const Vector& obj) {
    assert(obj.size() == nb_vars());
    assert(isfinite(obj));
    has_changed = true;
    mysoplex->changeObjReal(ivec2dvec(obj));
}

void LPSolver::set_cost(int index, double value) {
    assert(index >= 0 && index < nb_vars());
    assert(std::isfinite(value));
    has_changed = true;
    mysoplex->changeObjReal(index, value);
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
    assert(bounds.size() == nb_vars());
    assert(!bounds.is_unbounded());

    has_changed = true;
    // The bounds have to be changed in 3 places: ivec_bounds_,
    // in soplex variable bounds, and in bounds constraints.
    ivec_bounds_ = bounds;
    mysoplex->changeBoundsReal(ivec2dvec(bounds.lb()), ivec2dvec(bounds.ub()));
    const int n = nb_vars();
    for(int i = 0; i < n; ++i) {
        mysoplex->changeRangeReal(i, bounds[i].lb(), bounds[i].ub());
    }
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    assert(!bounds.is_unbounded());
    has_changed = true;
    ivec_bounds_[var] = bounds;
    mysoplex->changeBoundsReal(var, bounds.lb(), bounds.ub());
    mysoplex->changeRangeReal(var, bounds.lb(), bounds.ub());
}

void LPSolver::set_tolerance(double tolerance) {
    has_changed = true;
	mysoplex->setRealParam(SoPlex::FEASTOL, tolerance);
	mysoplex->setRealParam(SoPlex::OPTTOL, tolerance);
}

void LPSolver::set_timeout(double timeout) {
    has_changed = true;
    mysoplex->setRealParam(SoPlex::TIMELIMIT, timeout);
}

void LPSolver::set_max_iter(int max_iter) {
    has_changed = true;
	mysoplex->setIntParam(SoPlex::ITERLIMIT, max_iter);
}

int LPSolver::nb_rows() const {
    return mysoplex->numRowsReal();
}

int LPSolver::nb_vars() const {
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
    Matrix m(nb_rows(), nb_vars());
    for(int i = 0; i < nb_rows(); ++i) {
        m.set_row(i, row(i));
    }
    return m;
}

Vector LPSolver::row(int index) const {
    assert(index >= 0 && index < nb_rows());
    DSVectorReal dsrow(nb_vars());
    mysoplex->getRowVectorReal(index, dsrow);
    return dsvec2ivec(dsrow, nb_vars());
}

Matrix LPSolver::rows_transposed() const {
    return rows().transpose();
}

Vector LPSolver::col(int index) const {
    assert(index >= 0 && index < nb_vars());
    DSVectorReal dscol(nb_rows());
    mysoplex->getColVectorReal(index, dscol);
    return dsvec2ivec(dscol, nb_rows());
}

Vector LPSolver::lhs() const {
    DVectorReal dcol(nb_rows());
    mysoplex->getLhsReal(dcol);
    return dvec2ivec(dcol);
}

double LPSolver::lhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return mysoplex->lhsReal(index);
}

Vector LPSolver::rhs() const {
    DVectorReal dcol(nb_rows());
    mysoplex->getRhsReal(dcol);
    return dvec2ivec(dcol);
}
double LPSolver::rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return mysoplex->rhsReal(index);
}

IntervalVector LPSolver::lhs_rhs() const {
    IntervalVector lhs_rhs_vec(nb_rows());
    for(int i = 0; i < lhs_rhs_vec.size(); ++i) {
        lhs_rhs_vec[i] = Interval(mysoplex->lhsReal(i), mysoplex->rhsReal(i));
    }
    return lhs_rhs_vec;
}

Interval LPSolver::lhs_rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return Interval(mysoplex->lhsReal(index), mysoplex->rhsReal(index));
}

IntervalVector LPSolver::bounds() const {
    return ivec_bounds_;
}

Interval LPSolver::bounds(int index) const {
    assert(index >= 0 && index < nb_vars());
    return ivec_bounds_[index];
}

Vector LPSolver::cost() const {
    DVectorReal vec(nb_vars());
    mysoplex->getObjReal(vec);
    return dvec2ivec(vec);
}

double LPSolver::cost(int index) const {
    assert(index >= 0 && index < nb_vars());
    return mysoplex->objReal(index);
}

Interval LPSolver::minimum() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return obj_;
}

Vector LPSolver::not_proved_primal_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return uncertified_primal_;
}
Vector LPSolver::not_proved_dual_sol() const {
    if(!has_solution_) {
        ibex_error("LPSolver: no solution stored. Check solver status with LPSolver::status().");
    }
    return uncertified_dual_;
}

bool LPSolver::uncertified_infeasible_dir(Vector& infeasible_dir) const {
    if(has_infeasible_dir_) {
        infeasible_dir = uncertified_infeasible_dir_;
        return true;
    }
    return false;
}

void LPSolver::write_to_file(const std::string& filename) const {
    mysoplex->writeFileReal(filename.c_str(), NULL, NULL, NULL);
}

// Clear functions
void LPSolver::set_cost_to_zero() {
    DVectorReal new_obj(nb_vars());
    new_obj.clear();
    mysoplex->changeObjReal(new_obj);
}

void LPSolver::clear_constraints() {
    mysoplex->removeRowRangeReal(nb_vars(), nb_rows()-1);
}

void LPSolver::clear_bounds() {
    IntervalVector new_bounds(nb_vars(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}

void LPSolver::reset(int nb_vars) {
    assert(nb_vars > 0);
    invalidate();
    mysoplex->clearLPReal();

    // The default sense of optimization in soplex is Maximize,
    // and clearLPReal() also reset the sense of optimization.
    mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
    DSVectorReal dscol(0);
    for(int i = 0; i < nb_vars; ++i) {
        mysoplex->addColReal(LPColReal(0, dscol, soplex::infinity, -soplex::infinity));
    }
    DSVectorReal dsrow(nb_vars);
    for(int i = 0; i < nb_vars; ++i) {
        dsrow.add(i, 1);
        mysoplex->addRowReal(LPRowReal(-soplex::infinity, dsrow, soplex::infinity));
        dsrow.clear();
    }
    ivec_bounds_ = IntervalVector(nb_vars, Interval::ALL_REALS);
}

} /* end namespace ibex */
