
LPSolver::LPSolver(int nb_vars, int max_iter, int max_time_out, double eps):
	nb_vars(0), nb_rows(0), boundvar(1), sense(LPSolver::MINIMIZE),
	obj_value(POS_INFINITY), primal_solution(1), dual_solution(1 /*tmp*/),
	status_prim(false), status_dual(false)
{
	ibex_warning("No LP Solver available (use --lp-lib option in waf configure). ");
}

LPSolver::~LPSolver() {
}

LPSolver::Status_Sol LPSolver::solve() {
	return LPSolver::UNKNOWN;
}

void LPSolver::write_file(const char* name) {
	throw LPException();
}

Vector LPSolver::get_infeasible_dir() const {
	throw LPException();
}

Matrix LPSolver::get_rows() const{
	throw LPException();
}

Matrix LPSolver::get_rows_trans() const{
	throw LPException();
}

IntervalVector LPSolver::get_lhs_rhs() const{
	throw LPException();
}

Vector LPSolver::get_coef_obj() const{
	throw LPException();
}

double LPSolver::get_epsilon() const{
	throw LPException();
}

void LPSolver::clean_ctrs(){
	throw LPException();
}

void LPSolver::set_max_iter(int max){
	throw LPException();
}

void LPSolver::set_max_time_out(int time){
	throw LPException();
}

void LPSolver::set_sense(Sense s){
	throw LPException();
}

void LPSolver::set_obj(const Vector& coef){
	throw LPException();
}

void LPSolver::set_obj_var(int var, double coef){
	throw LPException();
}

void LPSolver::set_bounds(const IntervalVector& bounds){
	throw LPException();
}

void LPSolver::set_bounds_var(int var, const Interval& bound){
	throw LPException();
}

void LPSolver::set_epsilon(double eps){
	throw LPException();
}

void LPSolver::add_constraint(const Vector & row, CmpOp sign, double rhs ){
	throw LPException();
}
