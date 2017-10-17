LPSolver::LPSolver(int nb_vars, int max_iter,
	int max_time_out, double eps):
	nb_vars(0), nb_rows(0), obj_value(0.0), epsilon(0),
	primal_solution(1), dual_solution(1 /*tmp*/),
	status_prim(0), status_dual(0), boundvar(1)
{
	ibex_warning("No LP Solver available (use --lp-lib).");
}

LPSolver::~LPSolver() {
}

LPSolver::Status_Sol LPSolver::solve() {
	return LPSolver::UNKNOWN;
}

void LPSolver::write_file(const char* name) {
	throw LPException();
}

void LPSolver::get_coef_obj(Vector& obj) const {
	throw LPException();
}

void LPSolver::get_rows(Matrix& A) const {
	throw LPException();
}

void LPSolver::get_rows_trans(Matrix& A_trans) const {
	throw LPException();
}

void LPSolver::get_lhs_rhs(IntervalVector& B) const {
	throw LPException();
}

void LPSolver::get_primal_sol(Vector& prim) const {
	throw LPException();
}

void LPSolver::get_dual_sol(Vector& dual) const {
	throw LPException();
}

void LPSolver::get_infeasible_dir(Vector& sol) const {
	throw LPException();
}

void LPSolver::clean_ctrs() {
	throw LPException();
}

void LPSolver::clean_all() {
	throw LPException();
}

void LPSolver::set_max_iter(int max) {
	throw LPException();
}

void LPSolver::set_max_time_out(int time) {
	throw LPException();
}

void LPSolver::set_sense(Sense s) {
	throw LPException();
}

void setObj(const Vector& coef) {
	throw LPException();
}

void LPSolver::set_obj_var(int var, double coef) {
	throw LPException();
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
	throw LPException();
}

void LPSolver::set_bounds_var(int var, const Interval& bound) {
	throw LPException();
}

void LPSolver::set_epsilon(double eps) {
	throw LPException();
}

void LPSolver::add_constraint(const Vector& row, CmpOp sign, double rhs) {
	throw LPException();
}
