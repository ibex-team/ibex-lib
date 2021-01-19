#include "ibex_LPSolver.h"

namespace {

IloNumArray ivec2ilonumarray(const IloEnv env, const ibex::Vector& ivec) {
	IloNumArray array(env, ivec.size());
	for(int i = 0; i < ivec.size(); ++i) {
		array[i] = ivec[i];
	}
	return array;
}

ibex::Vector ilonumarray2ivec(const IloNumArray array) {
	ibex::Vector ivec(array.getSize());
	for(int i = 0; i < ivec.size(); ++i) {
		ivec[i] = array[i];
	}
	return ivec;
}

bool isfinite(const ibex::Vector& v) {
    for(int i = 0; i < v.size(); ++i) {
        if(!std::isfinite(v[i])) {
            return false;
        }
    }
    return true;
}

} // end anonymous namespace

namespace ibex {

LPSolver::LPSolver(int nb_vars, LPSolver::Mode mode, double tolerance,
        double timeout, int max_iter) {
	assert(nb_vars > 0);
	init(mode, tolerance, timeout, max_iter);
	reset(nb_vars);
}

LPSolver::LPSolver(std::string filename) {
    init(LPSolver::Mode::NotCertified, LPSolver::default_tolerance, LPSolver::default_timeout, LPSolver::default_max_iter);
    cplex.importModel(model, filename.c_str(), cost_, x, constraints);
    ivec_bounds_ = IntervalVector(x.getSize());
    for(int i = 0; i < x.getSize(); ++i) {
        ivec_bounds_[i] = Interval(x[i].getLB(), x[i].getUB());
        variableIdToIndex.emplace(x[i].getId(), i);
    }
}

LPSolver::~LPSolver() {
    env.end();
}

void LPSolver::init(LPSolver::Mode mode, double tolerance, double timeout, int max_iter) {
    env.setOut(env.getNullStream());
    model = IloModel(env);
	x = IloNumVarArray(env);
    cplex = IloCplex(env);
    cplex.setOut(env.getNullStream());
	cost_ = IloObjective(env);
	cost_.setSense(IloObjective::Sense::Minimize);
	constraints = IloRangeArray(env);
	model.add(cost_);
	model.add(constraints);
	mode_ = mode;
	set_tolerance(tolerance);
	set_timeout(timeout);
	set_max_iter(max_iter);
}

int LPSolver::add_constraint(double lhs, const Vector& row, double rhs) {
    assert(row.size() == nb_vars());
    assert(std::isfinite(lhs) && std::isfinite(rhs));
    assert(isfinite(row));

    has_changed = true;
	IloRange range(env, lhs, rhs);
	range.setLinearCoefs(x, ivec2ilonumarray(env, row));
	constraints.add(range);
	model.add(range);
    return nb_rows()-1;
}

int LPSolver::add_constraint(const Vector& row, CmpOp op, double rhs) {
    assert(row.size() == nb_vars());
    assert(std::isfinite(rhs));
    assert(isfinite(row));

    has_changed = true;
	IloRange range(env, -IloInfinity, IloInfinity);
	switch(op) {
	case ibex::LT:
	case ibex::LEQ:
		range.setUB(rhs);
		break;
	case ibex::GT:
	case ibex::GEQ:
		range.setLB(rhs);
		break;
	case ibex::EQ:
		range.setUB(rhs);
		range.setLB(rhs);
	}
	range.setLinearCoefs(x, ivec2ilonumarray(env, row));
	constraints.add(range);
	model.add(range);
    return nb_rows()-1;
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

LPSolver::Status LPSolver::minimize() {
    invalidate();
    assert(!ivec_bounds_.is_unbounded());

	cplex.extract(model);
	cplex.solve();
    IloCplex::CplexStatus cplex_status = cplex.getCplexStatus();
    status_ = LPSolver::Status::Unknown;
    switch(cplex_status) {
    case IloCplex::CplexStatus::Optimal:
        {
            IloNumArray vec_primal(env);
			cplex.getValues(vec_primal, x);
            IloNumArray vec_dual(env);
			cplex.getDuals(vec_dual, constraints);

            obj_ = cplex.getObjValue();

            uncertified_dual_ = ilonumarray2ivec(vec_dual);
            uncertified_primal_ = ilonumarray2ivec(vec_primal);
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
    case IloCplex::CplexStatus::AbortTimeLim:
        status_ = LPSolver::Status::Timeout;
        break;
    case IloCplex::CplexStatus::AbortItLim:
        status_ = LPSolver::Status::MaxIter;
        break;
    case IloCplex::CplexStatus::Infeasible:
        {
            status_ = LPSolver::Status::Infeasible;
			IloConstraintArray rng(env);
			IloNumArray dual_farkas(env);
            cplex.dualFarkas(rng, dual_farkas);
			uncertified_infeasible_dir_ = ilonumarray2ivec(dual_farkas);
			has_infeasible_dir_ = true;
			// CPLEX seems to always return a Farkas proof when the status is infeasible
			// doc: https://www.ibm.com/support/knowledgecenter/SSSA5P_12.10.0/ilog.odms.cplex.help/refcppcplex/html/classes/IloCplex.html?view=kc#method_dualFarkas
            /*if(b) {
                uncertified_infeasible_dir_ = ilonumarray2ivec(dual_farkas);
                has_infeasible_dir_ = true;
            }*/
            if(mode_ == LPSolver::Mode::Certified) {
                bool infeasible_proved = neumaier_shcherbina_infeasibility_test();
                if(infeasible_proved) {
                    status_ = LPSolver::Status::InfeasibleProved;
                }
            }
        }
        break;
    case IloCplex::CplexStatus::Unbounded:
        status_ = LPSolver::Status::Unbounded;
        break;
    default:
    	// TODO: we need an option to log such warnings, see issue #440.
        //std::string error_msg = "LPSolver: solve status is an internal Cplex status (" + std::to_string(cplex_status) + "). This is probably an error for you.";
        //ibex_warning(error_msg.c_str());
        status_ = LPSolver::Status::Unknown;
    }
    return status_;
}

void LPSolver::set_cost(const Vector& obj) {
    assert(obj.size() == nb_vars());
    assert(isfinite(obj));

    has_changed = true;
	cost_.setLinearCoefs(x, ivec2ilonumarray(env, obj));
}

void LPSolver::set_cost(int index, double value) {
    assert(index >= 0 && index < nb_vars());
    assert(std::isfinite(value));

    has_changed = true;
    cost_.setLinearCoef(x[index], value);
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
    assert(bounds.size() == nb_vars());
    assert(!bounds.is_unbounded());
    has_changed = true;
    for(int i = 0; i < nb_vars(); ++i) {
		set_bounds(i, bounds[i]);
	}
}

void LPSolver::set_bounds(int var, const Interval& bounds) {
    assert(var >= 0 && var < nb_vars());
    assert(!bounds.is_unbounded());

    has_changed = true;
    ivec_bounds_[var] = bounds;
	x[var].setBounds(bounds.lb(), bounds.ub());
	constraints[var].setBounds(bounds.lb(), bounds.ub());
}

void LPSolver::set_tolerance(double tolerance) {
    has_changed = true;
    if(tolerance < 1e-9) {
        std::string msg = "Invalid CPLEX tolerance: " + std::to_string(tolerance) + ", setting to " + std::to_string(0.99e-9);
        ibex_warning(msg);
        tolerance =1e-9;
    }
    else if(tolerance > 1e-1) {
        std::string msg = "Invalid CPLEX tolerance: " + std::to_string(tolerance) + ", setting to " + std::to_string(0.09);
        ibex_warning(msg);
        tolerance = 1e-1;
    }
	cplex.setParam(IloCplex::Param::Simplex::Tolerances::Feasibility, tolerance);
	cplex.setParam(IloCplex::Param::Simplex::Tolerances::Optimality, tolerance);
}

void LPSolver::set_timeout(double timeout) {
    if(timeout < 0) {
        timeout = std::numeric_limits<decltype(timeout)>::max();
    }
    has_changed = true;
    cplex.setParam(IloCplex::Param::TimeLimit, timeout);
}

void LPSolver::set_max_iter(int max_iter) {
    if(max_iter < 1) {
        max_iter = std::numeric_limits<decltype(max_iter)>::max();
    }
    has_changed = true;
	cplex.setParam(IloCplex::Param::Simplex::Limits::Iterations, max_iter);
}

int LPSolver::nb_rows() const {
    return constraints.getSize();
}

int LPSolver::nb_vars() const {
    return x.getSize();
}

double LPSolver::tolerance() const {
    return cplex.getParam(IloCplex::Param::Simplex::Tolerances::Optimality);
}

int LPSolver::max_iter() const {
    return cplex.getParam(IloCplex::Param::Simplex::Limits::Iterations);
}

double LPSolver::timeout() const {
    return cplex.getParam(IloCplex::Param::TimeLimit);
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
    return linearit2ivec(nb_vars(), constraints[index].getLinearIterator());
}

Matrix LPSolver::rows_transposed() const {
    return rows().transpose();
}

Vector LPSolver::col(int index) const {
    not_implemented("LPSolver: col accessor not implemented for CPLEX");
}

Vector LPSolver::lhs() const {
    Vector ivec(nb_rows());
	for(int i = 0; i < ivec.size(); ++i) {
		ivec[i] = constraints[i].getLB();
	}
	return ivec;
}

double LPSolver::lhs(int index) const {
    assert(index >= 0 && index < nb_rows());
	return constraints[index].getLB();
}

Vector LPSolver::rhs() const {
    Vector ivec(nb_rows());
	for(int i = 0; i < ivec.size(); ++i) {
		ivec[i] = constraints[i].getUB();
	}
	return ivec;
}
double LPSolver::rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return constraints[index].getUB();
}

IntervalVector LPSolver::lhs_rhs() const {
    IntervalVector lhs_rhs_vec(nb_rows());
    for(int i = 0; i < lhs_rhs_vec.size(); ++i) {
        lhs_rhs_vec[i] = Interval(lhs(i), rhs(i));
    }
    return lhs_rhs_vec;
}

Interval LPSolver::lhs_rhs(int index) const {
    assert(index >= 0 && index < nb_rows());
    return Interval(lhs(index), rhs(index));
}

IntervalVector LPSolver::bounds() const {
    return ivec_bounds_;
}

Interval LPSolver::bounds(int index) const {
    assert(index >= 0 && index < nb_vars());
    return ivec_bounds_[index];
}

Vector LPSolver::cost() const {
    return linearit2ivec(nb_vars(), cost_.getLinearIterator());
}

double LPSolver::cost(int index) const {
    assert(index >= 0 && index < nb_vars());
    return cost()[index];
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
    cplex.extract(model);
    cplex.exportModel(filename.c_str());
}

// Clear functions
void LPSolver::set_cost_to_zero() {
    set_cost(Vector(nb_rows(), 0.0));
}

void LPSolver::clear_constraints() {
    for(int i = nb_vars(); i < nb_rows(); ++i) {
        constraints[i].end();
    }
    constraints.remove(nb_vars(), nb_rows() - nb_vars());
}

void LPSolver::clear_bounds() {
    IntervalVector new_bounds(nb_vars(), Interval::ALL_REALS);
    set_bounds(new_bounds);
}



void LPSolver::reset(int nb_vars) {
	assert(nb_vars > 0);
	invalidate();

	// Create
	cost_.setConstant(0);
	x.endElements();
    x.clear();
    variableIdToIndex.clear();
    for(int i = 0; i < nb_vars; ++i) {
        IloNumVar var(env);
        variableIdToIndex.emplace(var.getId(), i);
        x.add(var);
    }
	model.add(x);
	constraints.endElements();
    constraints.clear();
	for(int i = 0; i < nb_vars; ++i) {
		IloRange range(env, -IloInfinity, IloInfinity);
		range.setLinearCoef(x[i], 1);
		constraints.add(range);
	}
	model.add(constraints);
	ivec_bounds_ = IntervalVector(nb_vars, Interval::ALL_REALS);
}

ibex::Vector LPSolver::linearit2ivec(int n, IloExpr::LinearIterator it) const {
    Vector ivec(n, 0.0);
    while(it.ok()) {
        int id = it.getVar().getId();
        ivec[variableIdToIndex.at(id)] = it.getCoef();
        ++it;
    }
    return ivec;
}

} /* end namespace ibex */
