//============================================================================
//                                  I B E X
// Interface with the linear solver
// File        : ibex_LinearSolver.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 15, 2013
// Last Update : May 15, 2013
//============================================================================

#include "ibex_LinearSolver.h"
#include  <cfloat>

namespace ibex {

const double LinearSolver::default_eps = 1e-9;
const double LinearSolver::default_max_bound = 1e20; // max bound in CPLEX
const int LinearSolver::default_max_time_out=100;
const int LinearSolver::default_max_iter=100;
const Interval LinearSolver::default_limit_diam_box = Interval(1.e-14,1.e6);



/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LinearSolver::Status_Sol x){

	switch(x) {
	case(LinearSolver::OPTIMAL) :{
			os << "OPTIMAL";
			break;
	}
	case(LinearSolver::INFEASIBLE) :{
			os << "INFEASIBLE";
			break;
	}
	case(LinearSolver::OPTIMAL_PROVED) :{
			os << "OPTIMAL_PROVED";
			break;
	}
	case(LinearSolver::INFEASIBLE_PROVED) :{
			os << "INFEASIBLE_PROVED";
			break;
	}
	case(LinearSolver::TIME_OUT) :{
			os << "TIME_OUT";
			break;
	}
	case(LinearSolver::MAX_ITER) :{
			os << "MAX_ITER";
			break;
	}
	case(LinearSolver::UNKNOWN) :{
		os << "UNKNOWN";
		break;
	}
	}
	return os;

}


LinearSolver::Status_Sol LinearSolver::solve_proved() {
	LinearSolver::Status_Sol stat = solve();
	switch (stat) {
	case LinearSolver::OPTIMAL : {
		// Neumaier - Shcherbina postprocessing
		Interval obj = neumaier_shcherbina_postprocessing();
		obj_value = Interval(obj.lb(),obj_value.ub());
		stat = LinearSolver::OPTIMAL_PROVED;
		break;
	}
	case  LinearSolver::INFEASIBLE: {
		// infeasibility test  cf Neumaier Shcherbina paper
		if (neumaier_shcherbina_infeasibilitytest()) {
			stat = LinearSolver::INFEASIBLE_PROVED;
		}
		break;
	}
	default:
		stat = LinearSolver::UNKNOWN;
		break;
	}
	return stat;

}


int LinearSolver::get_nb_rows() const {
	return nb_rows;
}

Interval LinearSolver::get_obj_value() const {
	return obj_value;
}

void LinearSolver::add_constraint(const Matrix & A, CmpOp sign, const Vector& rhs ) {
	for (int i=0; i<A.nb_rows(); i++) {
		try {
			add_constraint(A[i],sign,rhs[i]);
		} catch (LPException&) { }
	}
}


void LinearSolver::clean_bounds() {
	set_bounds(IntervalVector(nb_vars));
}

void LinearSolver::clean_obj() {
	set_obj(ibex::Vector::zeros(nb_vars));
}

void LinearSolver::clean_all() {
	clean_ctrs();
	clean_obj();
	clean_bounds();
}

Vector LinearSolver::get_primal_sol() const {
	try {
		if (status_prim) {
			return primal_solution;
		} else
			throw LPException();
	}
	catch(...) {
		throw LPException();
	}
}

Vector LinearSolver::get_dual_sol() const {
	try {
		if (status_dual) {
			return dual_solution;
		} else
			throw LPException();
	}
	catch(...) {
		throw LPException();
	}
}

///////////////////////////////////////////////////////////////////////////////////

LinearSolver::Status_Sol LinearSolver::solve_var(LinearSolver::Sense s, int var, Interval& obj) {
	assert((0<=var)&&(var<=nb_vars));

	LinearSolver::Status_Sol stat = LinearSolver::UNKNOWN;
	set_sense(LinearSolver::MINIMIZE);

	try {
		// the linear solver is always called in a minimization mode : in case of maximization of var , the opposite of var is minimized
		if(s==LinearSolver::MINIMIZE)
			set_obj_var(var, 1.0);
		else
			set_obj_var(var, -1.0);

		//	mylinearsolver->write_file("coucou.lp");
		//	system("cat coucou.lp");
		stat = solve();
		//std::cout << "[polytope-hull]->[solve_var] solver returns " << stat << std::endl;
		switch (stat) {
		case LinearSolver::OPTIMAL : {
			// Neumaier - Shcherbina postprocessing
			obj_value = neumaier_shcherbina_postprocessing_var(var, s);
			//std::cout << "[polytope-hull]->[solve_var] obj return " << obj_value << std::endl;
			obj = obj_value;
			stat = LinearSolver::OPTIMAL_PROVED;
			break;
		}
		case  LinearSolver::INFEASIBLE: {
			// infeasibility test  cf Neumaier Shcherbina paper
			if (neumaier_shcherbina_infeasibilitytest()) {
				stat = LinearSolver::INFEASIBLE_PROVED;
			}
			break;
		}
		default:
			stat = LinearSolver::UNKNOWN;
			break;
		}
		// Reset the objective of the LP solver
		set_obj_var(var, 0.0);
	}
	catch (LPException&) {
		stat = LinearSolver::UNKNOWN;
		// Reset the objective of the LP solver
		set_obj_var(var, 0.0);
	}

	return stat;

}

Interval LinearSolver::neumaier_shcherbina_postprocessing_var (int var, LinearSolver::Sense s) {
	try {
		// the dual solution : used to compute the bound
		ibex::Vector dual = get_dual_sol();

		ibex::Matrix A_trans = get_rows_trans();

		IntervalVector B = get_lhs_rhs();

		//cout <<" BOUND_test "<< endl;
		IntervalVector Rest(nb_vars);
		IntervalVector Lambda(dual);


		//		std::cout << " A_t " << A_trans << std::endl;
		//		std::cout << " B " << B << std::endl;
		//		std::cout << " dual " << Lambda << std::endl;
		//		std::cout << " box " << boundvar << std::endl;
		//		std::cout << " dual B " << Lambda * B << std::endl;

		Rest = A_trans * Lambda ;   // Rest = Transpose(As) * Lambda
		if (s==LinearSolver::MINIMIZE) {
			Rest[var] -=1; // because C is a vector of zero except for the coef "var"
			return (Lambda * B - Rest * boundvar);
		} else {
			Rest[var] +=1;
			return -(Lambda * B - Rest * boundvar);
		}

	} catch (...) {
		throw LPException();
	}
}


Interval LinearSolver::neumaier_shcherbina_postprocessing() {
	try {
		// the dual solution : used to compute the bound
		ibex::Vector dual =	get_dual_sol();

		ibex::Matrix A_trans = get_rows_trans();

		IntervalVector B = get_lhs_rhs();

		ibex::Vector obj = get_coef_obj();

		//cout <<" BOUND_test "<< endl;
		IntervalVector Rest(nb_vars);
		IntervalVector Lambda(dual);

		Rest = A_trans * Lambda ;
		Rest -= obj;   // Rest = Transpose(As) * Lambda - obj
		return (Lambda * B - Rest * boundvar);

		Rest = A_trans * Lambda ;   // Rest = Transpose(As) * Lambda
		if (sense==LinearSolver::MINIMIZE) {
			Rest -= obj;   // Rest = Transpose(As) * Lambda - obj
			return (Lambda * B - Rest * boundvar);
		} else {
			Rest += obj;   // Rest = Transpose(As) * Lambda - obj
			return -(Lambda * B - Rest * boundvar);
		}

	} catch (...) {
		throw LPException();
	}
}

bool LinearSolver::neumaier_shcherbina_infeasibilitytest() {
	try {
		ibex::Vector infeasible_dir = get_infeasible_dir();

		ibex::Matrix A_trans = get_rows_trans();

		IntervalVector B = get_lhs_rhs();

		IntervalVector Lambda(infeasible_dir);

		IntervalVector Rest(nb_vars);
		Rest = A_trans * Lambda ;
		Interval d= Rest * boundvar - Lambda * B;

		// if 0 does not belong to d, the infeasibility is proved

		if (d.contains(0.0))
			return false;
		else
			return true;

	} catch (LPException&) {
		return false;
	}

}





/////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _IBEX_WITH_SOPLEX_


LinearSolver::LinearSolver(int nb_vars1, int max_iter, int max_time_out, double eps) :
			nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1) , sense(LinearSolver::MINIMIZE),
			obj_value(0.0), primal_solution(nb_vars1), dual_solution(1 /*tmp*/),
			status_prim(false), status_dual(false){


	mysoplex= new soplex::SoPlex();

	mysoplex->changeSense(soplex::SPxLP::MINIMIZE);
	mysoplex->setTerminationIter(max_iter);
	mysoplex->setTerminationTime(max_time_out);
	mysoplex->setDelta(eps);

	// initialize the number of variables of the LP
	soplex::DSVector col1(0);
	for (int j=0; j<nb_vars; j++){
		mysoplex->addCol(soplex::LPCol(0.0, col1, soplex::infinity, -( soplex::infinity )));
	}

	// initialize the constraint of the bound of the variable
	soplex::DSVector row1(nb_vars);
	for (int j=0; j<nb_vars; j++){
		row1.add (j,1.0);
		mysoplex->addRow(soplex::LPRow(-soplex::infinity, row1,soplex::infinity));
		row1.clear();
	}

	nb_rows += nb_vars;

}

LinearSolver::~LinearSolver() {
	delete mysoplex;
}

LinearSolver::Status_Sol LinearSolver::solve() {
	obj_value = Interval::ALL_REALS;

	soplex::SPxSolver::Status stat = soplex::SPxSolver::UNKNOWN;

	try{
		status_prim = false;
		status_dual = false;
		stat = mysoplex->solve();
		switch (stat) {
		case (soplex::SPxSolver::OPTIMAL) : {
			obj_value = mysoplex->objValue();

			// the primal solution : used by choose_next_variable
			soplex::DVector primal(nb_vars);
			mysoplex->getPrimal(primal);
			status_prim = true;
			for (int i=0; i< nb_vars ; i++) {
				primal_solution[i]=primal[i];
			}
			// the dual solution ; used by Neumaier Shcherbina test
			soplex::DVector dual(nb_rows);

			dual_solution.resize(nb_rows);

			mysoplex->getDual(dual);
			status_dual = true;
			for (int i=0; i<nb_rows; i++) {
				if 	( ((mysoplex->rhs(i) >=  default_max_bound) && (dual[i]<=0)) ||
						((mysoplex->lhs(i) <= -default_max_bound) && (dual[i]>=0))   ) {
					dual_solution[i]=0;
				}
				else {
					dual_solution[i]=dual[i];
				}
			}
			return OPTIMAL;
		}
	    case(soplex::SPxSolver::ABORT_TIME): {
	    	return TIME_OUT;
	    }
	    case (soplex::SPxSolver::ABORT_ITER):{
	    	return MAX_ITER;
	    }
	    case (soplex::SPxSolver::INFEASIBLE):{
	    	return INFEASIBLE;
	    }
	    default : {
	    	return UNKNOWN;
	    }
	    }
	}catch(...){
		return UNKNOWN;
	}

}

void LinearSolver::write_file(const char* name) {
	try {
		mysoplex->writeFile(name, NULL, NULL, NULL);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

ibex::Vector LinearSolver::get_coef_obj() const {
	ibex::Vector obj(nb_vars);
	try {
		soplex::DVector newobj(nb_vars);
		mysoplex->getObj(newobj);
		for (int j=0;j<nb_vars; j++){
			obj[j] = newobj[j];
		}
	}
	catch(...) {
		throw LPException();
	}
	return obj;
}

ibex::Matrix LinearSolver::get_rows() const {
	ibex::Matrix A(nb_rows, nb_vars);
	try {
		for (int i=0;i<nb_rows; i++){
			for (int j=0;j<nb_vars; j++){
				A.row(i)[j] =  mysoplex->rowVector(i)[j];
			}
		}
	}
	catch(...) {
		throw LPException();
	}
	return A;
}

ibex::Matrix LinearSolver::get_rows_trans() const {
	ibex::Matrix A_trans(nb_vars,nb_rows);
	try {
		for (int i=0;i<nb_rows; i++){
			for (int j=0;j<nb_vars; j++){
				A_trans.row(j)[i] =  mysoplex->rowVector(i)[j];
			}
		}
	}
	catch(...) {
		throw LPException();
	}
	return A_trans;
}

IntervalVector  LinearSolver::get_lhs_rhs() const{
	IntervalVector B(nb_rows);
	try {
		for (int i=0;i<nb_rows; i++){
			B[i]=Interval( mysoplex->lhs(i) , mysoplex->rhs(i) );
		}
//		// Get the bounds of the variables
//		for (int i=0;i<nb_vars; i++){
//			B[i]=Interval( mysoplex->lhs(i) , mysoplex->rhs(i) );
//		}
//
//		// Get the bounds of the constraints
//		for (int i=nb_vars;i<nb_rows; i++){
//			B[i]=Interval( 	(mysoplex->lhs(i)>-default_max_bound)? mysoplex->lhs(i):-default_max_bound,
//					        (mysoplex->rhs(i)< default_max_bound)? mysoplex->rhs(i): default_max_bound   );
//			//	B[i]=Interval( 	mysoplex->lhs(i),mysoplex->rhs(i));
//			//Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA
//			//           cout << B(i+1) << endl;
//		}
	}
	catch(...) {
		throw LPException();
	}
	return B;
}

ibex::Vector LinearSolver::get_infeasible_dir() const {

	try {
		ibex::Vector sol(nb_rows);
		soplex::SPxSolver::Status stat1;
		soplex::DVector sol_found(nb_rows);
		mysoplex->getDualfarkas(sol_found);
		//stat1 = mysoplex->getDualfarkas(sol_found);
		// if (stat1==soplex::SPxSolver::OPTIMAL) // TODO I'm not sure of the value that return getDualfarkas : this condition does not work BNE
//		for (int i=0; i<nb_rows; i++) {
//			if (((mysoplex->lhs(i) <= -default_max_bound) && (sol_found[i]>=0))||
//				((mysoplex->rhs(i) >=  default_max_bound) && (sol_found[i]<=0))	) {
//				sol[i]=0.0;
//				}
//			else {
//				sol[i]=sol_found[i];
//			}
//		}
		//	else	res = FAIL; this condition does not work BNE

		for(int i=0; i<nb_rows; i++) {
			sol[i]=sol_found[i];
		}
		return sol;
	}
	catch(...) {
		throw LPException();
	}
}

double LinearSolver::get_epsilon() const {
	return mysoplex->delta();
}

void LinearSolver::clean_ctrs() {

	try {
		status_prim = false;
		status_dual = false;
		if ((nb_vars)<=  (nb_rows - 1))  {
			mysoplex->removeRowRange(nb_vars, nb_rows-1);
		}
		nb_rows = nb_vars;
		obj_value = POS_INFINITY;
	}
	catch(...) {
		throw LPException();
	}
	return ;

}


void LinearSolver::set_max_iter(int max) {

	try {
		mysoplex->setTerminationIter(max);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_max_time_out(int time) {

	try {
		double t = time;
		mysoplex->setTerminationTime(t);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_sense(Sense s) {

	try {
		if (s==LinearSolver::MINIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MINIMIZE);
			sense = LinearSolver::MINIMIZE;
		}
		else if (s==LinearSolver::MAXIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MAXIMIZE);
			sense = LinearSolver::MAXIMIZE;
		}
		else
			throw LPException();

	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_obj(const ibex::Vector& coef) {

	try {
		soplex::Vector newobj(nb_vars, coef.raw_const());
		mysoplex->changeObj(newobj);
	}
	catch(...) {
		throw LPException();
	}

	return;
}

void LinearSolver::set_obj_var(int var, double coef) {

	try {
		mysoplex->changeObj(var, coef);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds(const IntervalVector& bounds) {

	try {
		for (int j=0; j<nb_vars; j++){
			// Change the LHS and RHS of each constraint associated to the bounds of the variable
			mysoplex->changeRange(j ,bounds[j].lb(),bounds[j].ub());
		}
		boundvar = bounds;
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds_var(int var, const Interval& bound) {

	try {
		mysoplex->changeRange(var ,bound.lb(),bound.ub());
		//std::cout << "improve bound var "<<var<< std::endl;
		boundvar[var] = bound;
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_epsilon(double eps) {

	try {
		mysoplex->setDelta(eps);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

	try {
		soplex::DSVector row1(nb_vars);
		for (int i=0; i< nb_vars ; i++) {
			row1.add(i, row[i]);
		}

		if (sign==LEQ || sign==LT) {
			mysoplex->addRow(soplex::LPRow(-soplex::infinity, row1, rhs));
			nb_rows++;
		}
		else if (sign==GEQ || sign==GT) {
			mysoplex->addRow(soplex::LPRow(rhs, row1, soplex::infinity));
			nb_rows++;
		}
		else
			throw LPException();

	}
	catch(...) {
		throw LPException();
	}

	return ;
}


#endif  // END DEF with SOPLEX








//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _IBEX_WITH_CPLEX_

LinearSolver::LinearSolver(int nb_vars1, int max_iter, int max_time_out, double eps) :
		nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1), sense(LinearSolver::MINIMIZE),
		 obj_value(0.0), primal_solution(nb_vars1), dual_solution(1 /*tmp*/),
		status_prim(false), status_dual(false),
		envcplex(NULL), lpcplex(NULL) {

	int status;
	envcplex = CPXopenCPLEX(&status);

	/* Create the problem. */
	lpcplex = CPXcreateprob(envcplex, &status, "IBEX");


	if (envcplex == NULL) {
		char* errmsg = new char[1024];
		std::cerr << "Error CPLEX: Could not open CPLEX environment."<< status<< std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}
	double epsilon=eps;
	if (eps <1.e-9) epsilon=1.e-9;
	if (eps >1.e-1) epsilon=1.e-1;


	status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, epsilon);
	if (status) {
		char* errmsg = new char[1024];
		std::cerr << "Error CPLEX: Could not change the precision, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}


	status = CPXsetintparam(envcplex, CPX_PARAM_ITLIM, max_iter);
	if (status) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not change the maximal number of iteration, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}

	status = CPXsetdblparam(envcplex, CPX_PARAM_TILIM, max_time_out);
	if (status) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not change the time limit "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}

	status = CPXsetintparam(envcplex, CPX_PARAM_PREDUAL, -1);
	if (status) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not impose the preprocessing dual, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}

	status = CPXsetdblparam(envcplex, CPX_PARAM_EPRELAX, epsilon);
	if (status) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not change the precision on the feasible set "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		throw LPException();
	}

	CPXchgobjsen(envcplex, lpcplex, CPX_MIN);
	// Problem is minimization
	//  CPXchgobjsen (envcplex, lpcplex, CPX_MAX);   Problem is maximisation



	/* A returned pointer of NULL may mean that not enough memory
	 was available or there was some other problem.  In the case of
	 failure, an error message will have been written to the error
	 channel from inside CPLEX.  In this example, the setting of
	 the parameter CPX_PARAM_SCRIND causes the error message to
	 appear on stdout.  */

	if (lpcplex == NULL) {
		std::cerr << "Error CPLEX: Failed to create LP." << std::endl;
		throw LPException();
	}

	// Creation des variables

	double* lb = new double[nb_vars];
	double* ub = new double[nb_vars];
	int* c_matbeg = new int[nb_vars*2];
	double * c_matval = new double[nb_vars * 2];
	int * c_matind = new int[nb_vars * 2];
	double* rhs = new double[nb_vars * 2];
	char * sense = new char[nb_vars * 2];
	double * obj = new double[nb_vars];
	int i, j;

	// initialization of the LP

	for (i = 0; i < nb_vars; i++) {
		c_matbeg[i] = i;
		c_matind[i] = i;
		c_matval[i] = -1.0;
	}

	for (i = 0; i < nb_vars; i++) {
		c_matbeg[i+nb_vars] = i+nb_vars;
		c_matind[i+nb_vars] = i;
		c_matval[i+nb_vars] = 1.0;
	}

	//   rmatbeg[nb_vars] = k;

	/* initialisation des bornes des variables duales */
	/* initialisation de la fonction objectif du dual = rhs du primal*/

	for (i = 0; i < nb_vars; ++i) {
		lb[i] = -CPX_INFBOUND;
		ub[i] = CPX_INFBOUND;
		obj[i] = 0.0;

	}
	// initialisation du rhs du dual = objectif du primal */
	for (j = 0; j < nb_vars*2; ++j) {
		sense[j] = 'L';
		rhs[j] = 0;
	}

	//* Creation du probleme et insertion dans CPEX du probleme d'initialisation */

	status = CPXnewcols(envcplex, lpcplex, nb_vars, obj, lb, ub, NULL,NULL);
	if (status) {
		std::cerr << "Error CPLEX: Could not add a new column, error " << status << std::endl;	delete[] lb;
		delete[] ub; delete[] c_matbeg;	delete[] c_matval; delete[] c_matind; delete[] rhs; delete[] sense; delete[] obj;
		throw LPException();
	}

	status = CPXaddrows(envcplex, lpcplex, 0, nb_vars*2, nb_vars * 2, rhs, sense,
			c_matbeg, c_matind, c_matval, NULL, NULL);

	if (status) {
		std::cerr << "Error CPLEX: Could not add a new constraint, error "<< status << std::endl;
		delete[] ub; delete[] c_matbeg;	delete[] c_matval; delete[] c_matind; delete[] rhs; delete[] sense; delete[] obj;
		throw LPException();
	}

	// creation d'un tableau d'indice pour l'initialisation des bornes des variables
	indice = new int[nb_vars * 2];
	for (int i = 0; i < (nb_vars * 2); i++)
		indice[i] = i;

	// creation de l'espace memoire neccessaire
	tmp = new double[nb_vars * 2];
	r_matbeg = new int[1];
	r_matval = new double[nb_vars];
	r_matind = new int[nb_vars];

	r_matbeg[0] = 0;
	//cmatbeg[1] = nb_vars-1;
	for (int i = 0; i < nb_vars; i++)
		r_matind[i] = i;

	nb_rows += 2*nb_vars;

	//* Free */
	delete[] lb;
	delete[] ub;
	delete[] c_matbeg;
	delete[] c_matval;
	delete[] c_matind;
	delete[] rhs;
	delete[] sense;
	delete[] obj;

	return;
}

LinearSolver::~LinearSolver() {
	int status;

	// Free up the problem as allocated by CPXcreateprob, if necessary

	if (lpcplex != NULL) {
		status = CPXfreeprob(envcplex, &lpcplex);
		if (status) {
			std::cerr << "CPXfreeprob failed, error code " << status << std::endl;
		}
	}

	// Free up the CPLEX environment, if necessary
	if (envcplex != NULL) {
		status = CPXcloseCPLEX(&envcplex);

		// Note that CPXcloseCPLEX produces no output,
		//    so the only way to see the cause of the error is to use
		//    CPXgeterrorstring.  For other CPLEX routines, the errors will
		//    be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON.
		if (status) {
			char* errmsg = new char[1024];
			std::cerr << "Could not close CPLEX environment." << std::endl;
			CPXgeterrorstring(envcplex, status, errmsg);
			std::cerr << errmsg << std::endl;
			delete[] errmsg;
		}
	}

	delete[] indice;
	delete[] tmp;
	delete[] r_matbeg;
	delete[] r_matval;
	delete[] r_matind;
}

LinearSolver::Status_Sol LinearSolver::solve() {
	obj_value = Interval::ALL_REALS;

	try {
		// Optimize the problem and obtain solution.

		int status = CPXlpopt(envcplex, lpcplex);

		if (status == 0) {

			status_prim = false;
			status_dual = false;
			int solstat = CPXgetstat(envcplex, lpcplex);

			switch (solstat) {
			case (CPX_STAT_OPTIMAL): {
				double tmp;
				int status = CPXgetobjval(envcplex, lpcplex, &tmp);
				obj_value = tmp;

				if (status) {
					std::cerr << "LinearSolver: Failed to get the objective value." << std::endl;
					throw LPException();
				}
				CPXgetx(envcplex, lpcplex, primal_solution.raw(), 0, nb_vars - 1);
				status_prim = true;
				dual_solution.resize(nb_rows);
				CPXgetpi(envcplex, lpcplex, dual_solution.raw(), 0, nb_rows - 1);
				status_dual = true;
				return OPTIMAL;
			}
			case (CPX_STAT_INFEASIBLE):
				return INFEASIBLE;
			case (CPX_STAT_ABORT_IT_LIM):
				return MAX_ITER;
			case (CPX_STAT_ABORT_TIME_LIM):
				return TIME_OUT;
			default :
				return UNKNOWN;
			}

		} else
			return UNKNOWN;

	} catch (...) {
		return UNKNOWN;
	}

}

void LinearSolver::write_file(const char* name) {
	try {
		int status = CPXwriteprob(envcplex, lpcplex, name, NULL);
		if (status) {
			std::cerr << "Failed to write LP to disk." << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return ;
}


ibex::Vector LinearSolver::get_coef_obj() const {
	ibex::Vector obj(nb_vars);
	try {

		int status = CPXgetobj(envcplex, lpcplex, obj.raw(),0, nb_vars-1);
		if (status) {
			std::cerr<< "Error CPLEX: Could not get the objective function, error "<< status << std::endl;
			throw LPException();
		}

	} catch (...) {
		throw LPException();
	}
	return obj;
}

Matrix LinearSolver::get_rows() const {
	Matrix A(nb_rows, nb_vars);
	try {
		int surplus = 0, nzcnt = 0;
		int * t_Rmatbeg = new int[nb_rows];
		int * t_Rmatind = new int[2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars];
		double * t_Rmatval = new double[2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars];

		int status = CPXgetrows(envcplex, lpcplex, &nzcnt, t_Rmatbeg, t_Rmatind,
				t_Rmatval, 2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars, &surplus, 0,
				nb_rows - 1);

		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				for (int j = 0; j < nb_vars; j++) {
					A[i][j] = 0;
				}
			}
			for (int i = 0; i < nb_rows - 1; i++) {
				for (int j = t_Rmatbeg[i]; j < t_Rmatbeg[i + 1]; j++) {
					A[i][t_Rmatind[j]] = t_Rmatval[j];
				}
			}
			for (int j = t_Rmatbeg[nb_rows - 1]; j < nzcnt; j++) {
				A[nb_rows-1][t_Rmatind[j]] = t_Rmatval[j];
			}
		}
		delete[] t_Rmatbeg;
		delete[] t_Rmatind;
		delete[] t_Rmatval;

		if (status) throw LPException();

	} catch (...) {
		throw LPException();
	}
	return A;
}

Matrix LinearSolver::get_rows_trans() const {
	Matrix A_trans(nb_vars, nb_rows);
	try {
		int surplus = 0, nzcnt = 0;
		int * t_cmatbeg = new int[nb_vars];
		int * t_cmatind = new int[2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars];
		double * t_cmatval = new double[2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars];

		int status = CPXgetcols(envcplex, lpcplex, &nzcnt, t_cmatbeg, t_cmatind,
				t_cmatval, 2*nb_vars + (nb_rows - 2*nb_vars) * nb_vars, &surplus, 0,
				nb_vars - 1);

		if (status == 0) {
			for (int i = 0; i < nb_vars; i++) {
				for (int j = 0; j < nb_rows; j++) {
					A_trans[i][j] = 0;
				}
			}

			for (int i = 0; i < nb_vars - 1; i++) {
				for (int j = t_cmatbeg[i]; j < t_cmatbeg[i + 1]; j++) {
					A_trans[i][t_cmatind[j]] = t_cmatval[j];
				}
			}
			for (int j = t_cmatbeg[nb_vars - 1]; j < nzcnt; j++) {
				A_trans[nb_vars-1][t_cmatind[j]] = t_cmatval[j];
			}
			//std::cout<<A_trans<<std::endl;
		}

		delete[] t_cmatbeg;
		delete[] t_cmatind;
		delete[] t_cmatval;

		if (status) throw LPException();

	} catch (...) {
		throw LPException();
	}
	return A_trans;
}

IntervalVector LinearSolver::get_lhs_rhs() const {
	IntervalVector B(nb_rows);
	try {
		// Get the bounds of the variables
		double * rhs = new double[nb_rows];
		int status = CPXgetrhs(envcplex, lpcplex, rhs, 0, nb_rows - 1);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				B[i] = Interval(-default_max_bound, rhs[i]);
			}
		}
		delete[] rhs;

		if (status) throw LPException();
	} catch (...) {
		throw LPException();
	}
	return B;
}

Vector LinearSolver::get_infeasible_dir() const {
	try {
		Vector sol(nb_rows);
		int status = CPXdualfarkas(envcplex, lpcplex, sol.raw(),NULL);
		if (status == 0) {
//			for (int i = 0; i < nb_rows; i++) {
//				sol[i] = (solution_found[i]< 0) ? solution_found[i] : 0.0; // TODO why?? need to test
//			}
			return sol;
		}
		else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
}

void LinearSolver::clean_ctrs() {

	try {
		status_prim = false;
		status_dual = false;
		int status=0;
		if ((2*nb_vars)<=  (nb_rows - 1))  {
			status = CPXdelrows (envcplex, lpcplex, 2*nb_vars,  nb_rows - 1);
		}
		nb_rows = 2*nb_vars;
		obj_value = POS_INFINITY;
		if (status) throw LPException();

	} catch (...) {
		throw LPException();
	}
	return ;

}


double LinearSolver::get_epsilon() const {
	double epsilon;
	try {
		int status = CPXgetdblparam(envcplex, CPX_PARAM_EPOPT, &epsilon);
		if (status) {
			//std::cerr << "Error CPLEX: Could not get the precision, error "<< status << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return epsilon;
}


void LinearSolver::set_max_iter(int max) {

	try {
		int status = CPXsetintparam(envcplex, CPX_PARAM_ITLIM, max);
		if (status) {
			//std::cerr<< "Error CPLEX: Could not change the maximal number of iteration, error "<< status << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_max_time_out(int time) {

	try {
		double t = time;
		int status = CPXsetdblparam(envcplex, CPX_PARAM_TILIM, t);
		if (status) {
			//std::cerr<< "Error CPLEX: Could not change the maximal number of iteration "<< status << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_sense(Sense s) {

	try {

		if (s == LinearSolver::MINIMIZE) {
			int status = CPXchgobjsen(envcplex, lpcplex, CPX_MIN); // Problem is minimization
			if (status) {
				std::cerr<< "Error CPLEX: Could not change the sense, error "<< status << std::endl;
				throw LPException();
			}
			else  sense = LinearSolver::MINIMIZE;
		} else if (s == LinearSolver::MAXIMIZE) {
			int status = CPXchgobjsen(envcplex, lpcplex, CPX_MAX); //Problem is maximisation
			if (status) {
				std::cerr<< "Error CPLEX: Could not change the sense, error "<< status << std::endl;
				throw LPException();
			}
			else sense = LinearSolver::MAXIMIZE;
		} else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_obj_var(int var, double coef) {

	try {

		int status = CPXchgobj(envcplex, lpcplex, 1, &var, &coef);
		if (status) {
			std::cerr<< "Error CPLEX: Could not change the objective function, error "<< status << std::endl;
			throw LPException();
		}

	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_obj(const Vector& coef) {

	try {

		int status = CPXchgobj(envcplex, lpcplex, nb_vars, indice, (coef.raw_const()));
		if (status) {
			std::cerr<< "Error CPLEX: Could not change the objective function, error "<< status << std::endl;
			throw LPException();
		}

	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds(const IntervalVector& bounds) {

	try {
		// Changement de la fonction objective = rhs du primal (car on est en formulation dual)
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		IntervalVector tmp_bound (bounds.size(),Interval(-default_max_bound,default_max_bound));
		tmp_bound &= bounds;

		for (int i = 0; i < nb_vars; i++)
			tmp[i]			 = -tmp_bound[i].lb();
		for (int i = 0; i < nb_vars; i++)
			tmp[nb_vars + i] =  tmp_bound[i].ub();

		int status = CPXchgrhs(envcplex, lpcplex, (nb_vars * 2), indice, tmp);
		boundvar = tmp_bound;

		if (status) {
			std::cerr<< "Error CPLEX: Could not change the LHS and RHS of each constraint associated to the bounds of the variable, error "
					 << status << std::endl;
			throw LPException();
		}

	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds_var(int var, const Interval& bound) {

	try {
		int * ind = new int[2];
		ind[0] = var;
		ind[1] = var + nb_vars;
		tmp[0] = -bound.lb();
		tmp[1] =  bound.ub();
		int status = CPXchgrhs(envcplex, lpcplex, 2, ind, tmp);
		delete[] ind;
		boundvar[var] = bound;

		if (status) {
			//std::cerr<< "Error CPLEX: Could not change the bound of a variable, error "<< status << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_epsilon(double eps) {

	try {
		double epsilon= eps;
		if (eps <1.e-9) epsilon=1.e-9;
		if (eps >1.e-1) epsilon=1.e-1;

		int status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, epsilon);
		if (status) {
			//std::cerr << "Error CPLEX: Could not change the precision, error "<< status << std::endl;
			throw LPException();
		}
	} catch (...) {
		throw LPException();
	}
	return ;

}

void LinearSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

	try {
		char cc = 'L';
		if (sign == LEQ || sign == LT || sign == GEQ || sign == GT) {

			double * pt_rhs= new double[1];
			if (sign == LEQ || sign == LT) {
				pt_rhs[0] = rhs;
				for (int i = 0; i < nb_vars; i++)
					r_matval[i] = row[i];
			} else {
				pt_rhs[0] = -rhs;
				for (int i = 0; i < nb_vars; i++)
					r_matval[i] = -row[i];
			}

			int status = CPXaddrows(envcplex, lpcplex, 0, 1, nb_vars, pt_rhs, &cc, r_matbeg,
					r_matind, r_matval, NULL, NULL);
			delete[] pt_rhs;

			if (status==0) {
				nb_rows++;
			} else
				throw LPException();

		} else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
	return ;
}

#endif  // END DEF with CPLEX





/////////////////////////////////////////////////////////////////////////////
#ifdef _IBEX_WITH_CLP_


LinearSolver::LinearSolver(int nb_vars1, int max_iter, int max_time_out, double eps) :
			nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1), sense(LinearSolver::MINIMIZE),
			obj_value(0.0), primal_solution(nb_vars1), dual_solution(1 /*tmp*/),
			status_prim(false), status_dual(false)  {


	myclp= new ClpSimplex();

    //myclp->scaling(0);
    //ClpSolve options;
    //options.setSolveType(ClpSolve::useDual);
    //options.setDoDual(true);
    //options.setPresolveType(ClpSolve::presolveOn);
    //options.setSpecialOption(1,3,30);
    //myclp->initialSolve(options);

	/// Direction of optimization (1 - minimize, -1 - maximize, 0 - ignore
	myclp->setOptimizationDirection(1);
	myclp->setMaximumIterations(max_iter);
	myclp->setMaximumSeconds(max_time_out);
	myclp->setPrimalTolerance(eps);
    myclp->setDualTolerance(eps);

	// no log
	myclp->setLogLevel(0);


	// initialize the number of variables of the LP
	myclp->resize(0,nb_vars);
	for (int i = 0; i < nb_vars; i++) {
		myclp->setColumnBounds(i, -DBL_MAX, DBL_MAX);
   }

	// initialize the constraint of the bound of the variable

    int *row2Index = new int[1];
    double *row2Value = new double[1];
    row2Value[0]=1.0;
	for (int j=0; j<nb_vars; j++){
		row2Index[0]=j;
		myclp->addRow(1, row2Index, row2Value, -DBL_MAX, DBL_MAX);
	}

	delete[] row2Index;
	delete[] row2Value;

	nb_rows = nb_vars;

//	_which =new int[10*nb_rows];
//	for (int i=0;i<(10*nb_rows);i++) {
//		_which[i]=i+nb_vars;
//	}

	_col1Index = new int[nb_vars];
	for (int i=0;i<nb_vars;i++) {
		_col1Index[i]=i;
	}

}

LinearSolver::~LinearSolver() {
	delete myclp;
	delete [] _col1Index;
}

LinearSolver::Status_Sol LinearSolver::solve() {
	obj_value = Interval::ALL_REALS;
	//int stat = -1;

	try{
		status_prim = false;
		status_dual = false;

		myclp->dual();
		//stat = myclp->status();
		myclp->status();

		/** Status of problem:
    	         -1 - unknown e.g. before solve or if postSolve says not optimal
    	         0 - optimal
    	         1 - primal infeasible
    	         2 - dual infeasible
    	         3 - stopped on iterations or time
    	         4 - stopped due to errors
    	         5 - stopped by event handler (virtual int ClpEventHandler::event())
		 */
		if (myclp->isProvenOptimal()) {
			obj_value = myclp->getObjValue();

			// the primal solution : used by choose_next_variable
			double * primal= myclp->primalColumnSolution();
			for (int i=0; i< nb_vars ; i++) {
				primal_solution[i]=primal[i];
			}
			status_prim = true;

			// the dual solution ; used by Neumaier Shcherbina test
			double * dual = myclp->dualRowSolution();

			dual_solution.resize(nb_rows);

			for (int i=0; i<nb_rows; i++) {
				if 	( ((myclp->getRowUpper()[i] >=  default_max_bound) && (dual[i]<=0)) ||
						((myclp->getRowLower()[i] <= -default_max_bound) && (dual[i]>=0))   ) {
					dual_solution[i]=0;
				}
				else {
					dual_solution[i]=dual[i];
				}
			}
			status_dual = true;
			return OPTIMAL;
		}
		else if (myclp->isProvenPrimalInfeasible())
			return INFEASIBLE;
		else if (myclp->isIterationLimitReached()){
			if (myclp->secondaryStatus()==9) {
				return TIME_OUT;
			}
			else return MAX_ITER;
			}
		else
			return UNKNOWN;

	}catch(...){
		return UNKNOWN;
	}

}

void LinearSolver::write_file(const char* name) {
	try {
		myclp->writeMps(name);
	}
	catch(...) {
		throw LPException();
	}
	return;
}


double LinearSolver::get_epsilon() const {
	try  {
		return myclp->primalTolerance();
	}
	catch(...) {
		throw LPException();
	}
}


Vector LinearSolver::get_coef_obj() const {
	Vector obj(nb_vars);
	try  {
		const double * newobj =  myclp->getObjCoefficients();
		for (int i=0; i< nb_vars ; i++) {
			obj[i] = newobj[i];
		}
	}
	catch(...) {
		throw LPException();
	}
	return obj;
}


Matrix LinearSolver::get_rows() const {
	Matrix A(nb_rows,nb_vars);
	try {
		CoinPackedMatrix * mat=myclp->matrix();
		// see mat.getCorefficient()
		//A = Matrix::zeros(nb_rows,nb_vars);
		if (mat->isColOrdered()) {
			for (int cc=0;cc<nb_vars; cc++){
				CoinBigIndex j;
				CoinBigIndex end=mat->getVectorStarts()[cc]+mat->getVectorLengths()[cc];;
				for (j=mat->getVectorStarts()[cc];j<end;j++) {
					A.row(mat->getIndices()[j])[cc] = mat->getElements()[j];
				}
			}

		} else {
			for (int rr=0;rr<nb_rows; rr++){
				CoinBigIndex j;
				CoinBigIndex end=mat->getVectorStarts()[rr]+mat->getVectorLengths()[rr];;
				for (j=mat->getVectorStarts()[rr];j<end;j++) {
					A.row(rr)[mat->getIndices()[j]] = mat->getElements()[j];
				}
			}
		}
	} catch(...) {
		throw LPException();
	}
	return A;
}

Matrix LinearSolver::get_rows_trans() const {
	Matrix A_trans(nb_vars,nb_rows);
	try {
		CoinPackedMatrix * mat=myclp->matrix();
		// see mat.getCorefficient()
		//A_trans = Matrix::zeros(nb_vars,nb_rows);
		if (mat->isColOrdered()) {
			for (int cc=0;cc<nb_vars; cc++){
				CoinBigIndex j;
				CoinBigIndex end=mat->getVectorStarts()[cc]+mat->getVectorLengths()[cc];;
				for (j=mat->getVectorStarts()[cc];j<end;j++) {
					int jj = mat->getIndices()[j];
					A_trans.row(cc)[jj] = mat->getElements()[j];
				}
			}

		} else {
			for (int rr=0;rr<nb_rows; rr++){
				CoinBigIndex j;
				CoinBigIndex end=mat->getVectorStarts()[rr]+mat->getVectorLengths()[rr];;
				for (j=mat->getVectorStarts()[rr];j<end;j++) {
					A_trans.row(mat->getIndices()[j])[rr] = mat->getElements()[j];
				}
			}
		}
	}
	catch(...) {
		throw LPException();
	}
	return A_trans;
}

IntervalVector  LinearSolver::get_lhs_rhs() const {
	IntervalVector B(nb_rows);
	try {
		// Get the bounds of the variables
		for (int i=0;i<nb_rows; i++){
			B[i]=Interval( myclp->getRowLower()[i] , myclp->getRowUpper()[i] );
		}

		// Get the bounds of the constraints
//		for (int i=nb_vars;i<nb_rows; i++){
//			B[i]=Interval( 	(myclp->getRowLower()[i]>-default_max_bound)? myclp->getRowLower()[i]:-default_max_bound,
//	TODO why				        (myclp->getRowUpper()[i]< default_max_bound)? myclp->getRowUpper()[i]: default_max_bound   );
			//Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA
			//           cout << B(i+1) << endl;
//		}
	}
	catch(...) {
		throw LPException();
	}
	return B;
}




Vector LinearSolver::get_infeasible_dir() const {
	Vector sol(nb_rows);
	try {

		double * sol_found = myclp->infeasibilityRay();
		if (sol_found) {
			for (int i=0; i<nb_rows; i++) {
//				if (((myclp->getRowLower()[i] <= -default_max_bound) && (sol_found[i]>=0))||
//						(( myclp->getRowUpper()[i] >=  default_max_bound) && (sol_found[i]<=0))	) {
//					sol[i]=0.0;
//				}
//				else {
//					sol[i]=sol_found[i];
//				} //TODO why
				sol[i]=sol_found[i];
			}
			delete [] sol_found;
		}  else	{
			delete [] sol_found;
			throw LPException();
		}
	}
	catch(...) {
		throw LPException();
	}
	return sol;
}

void LinearSolver::clean_ctrs() {
	try {
		status_prim = false;
		status_dual = false;
		if (nb_vars<=(nb_rows - 1))  {
			//myclp->deleteRows(nb_rows -nb_vars,_which);
			myclp->resize(nb_vars,nb_vars);
		}
		nb_rows = nb_vars;
		obj_value = POS_INFINITY;
	}
	catch(...) {
		throw LPException();
	}
	return ;

}


void LinearSolver::set_max_iter(int max) {

	try {
		myclp->setMaximumIterations(max);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_max_time_out(int time) {

	try {
		myclp->setMaximumSeconds((double)time);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_sense(Sense s) {

	try {
			/// Direction of optimization (1 - minimize, -1 - maximize, 0 - ignore
		if (s==LinearSolver::MINIMIZE) {
			myclp->setOptimizationDirection(1);
			sense = LinearSolver::MINIMIZE;
		}
		else if (s==LinearSolver::MAXIMIZE) {
			myclp->setOptimizationDirection(-1);
			sense = LinearSolver::MAXIMIZE;
		}
		else
			throw LPException();

	}
	catch(...) {
		throw LPException();
	}
	return ;
}


void LinearSolver::set_obj(const Vector& coef) {

	try {
		for (int i =0; i< nb_vars; i++)
			myclp->setObjectiveCoefficient(i, coef[i]);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_obj_var(int var, double coef) {

	try {
		myclp->setObjectiveCoefficient(var, coef);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds(const IntervalVector& bounds) {
	try {
		for (int j=0; j<nb_vars; j++){
			// Change the LHS and RHS of each constraint associated to the bounds of the variable
			myclp->setRowBounds(j,bounds[j].lb(),bounds[j].ub());
		}
		boundvar = bounds;
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_bounds_var(int var, const Interval& bound) {

	try {
		myclp->setRowBounds(var,bound.lb(),bound.ub());
		//std::cout << "improve bound var "<<var<< std::endl;
		boundvar[var] = bound;

	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::set_epsilon(double eps) {

	try {
		myclp->setPrimalTolerance(eps);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LinearSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

	try {
		if (sign==LEQ || sign==LT) {
			myclp->addRow(nb_vars,_col1Index,&(row[0]),NEG_INFINITY,rhs);
			nb_rows++;
		}
		else if (sign==GEQ || sign==GT) {
			myclp->addRow(nb_vars,_col1Index,&(row[0]),rhs,POS_INFINITY);
			nb_rows++;
		}
		else
			throw LPException();
	}
	catch(...) {
		throw LPException();
	}

	return ;
}







#endif  // END DEF with CLP





/*
//TODO ////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _IBEX_WITH_ILOCPLEX_

LinearSolver::LinearSolver(int nb_vars1, int max_iter,
		int max_time_out, double eps) :
		nb_vars(nb_vars1), nb_rows(0), obj_value(0.0), epsilon(
				eps) {
	try {
		myenv = new IloEnv();

		// creating a empty model
	    mymodel = new IloModel(*myenv);


	    //	    initialization of the dual variable

	    IloNumVarArray 	col(*myenv);
	    for (int i=0; i<nb_vars*2;i++) {
	    	col.add(IloNumVar(*myenv,0,IloInfinity));
	    }

	    //	    initialization of the fake objective function
	    IloNumArray		num(*myenv,nb_vars*2);
	    for (int i=0; i<nb_vars*2;i++) {
	    	num[i]=i+1;
	    }
	    mymodel->add(IloMinimize(*myenv,IloScalProd(num,col) ));

	    //	    initialization of the constraint
	    IloRangeArray 	lig(*myenv);
	    for (int i=0; i<nb_vars;i++) {
	    	lig.add(-col[i]+col[nb_vars+i] == 0);
	    }

	    mymodel->add(lig);

	    mycplex = new IloCplex(*mymodel);

	    mycplex->setParam(IloCplex::RootAlg, IloCplex::Primal);
	    mycplex->setParam(IloCplex::EpOpt, 	eps);
	    mycplex->setParam(IloCplex::ItLim,	max_iter);
	    mycplex->setParam(IloCplex::TiLim,	max_time_out);
	    mycplex->setParam(IloCplex::PreDual, -1);


	    nb_rows = nb_vars;

	}
	catch (IloException& e) {
		std::cerr << "Concert exception caught: " << e << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception caught" << std::endl;
	}

	return;
}

LinearSolver::~LinearSolver() {
	mymodel->end();
	mycplex->end();
	myenv->end();
	delete mymodel;
	delete mycplex;
	delete myenv;

}

LinearSolver::Status_Sol LinearSolver::solve() {
	obj_value = Interval::ALL_REALS;

	LinearSolver::Status_Sol res = UNKNOWN;
	try {
		// Optimize the problem and obtain solution.
		if ( mycplex->solve() ) {
			IloCplex::CplexStatus solstat =mycplex->getStatus();

			if (solstat == IloCplex::AbortItLim) {
				res = MAX_ITER;
			} else if (solstat ==IloCplex::AbortTimeLim ) {
				res = TIME_OUT;
			} else if (solstat == IloCplex::Optimal) {
				obj_value = mycplex->getObjValue();
				res = OPTIMAL;
			} else if ((solstat == IloCplex::Infeasible )
					|| (solstat == IloCplex::Unbounded )
					|| (solstat == IloCplex::InfOrUnbd )) {
				res = INFEASIBLE_NOTPROVED;
			} else
				res = UNKNOWN;

		}

	} catch (IloException& ) {
		res = UNKNOWN;
	}
	//std::cout <<"	stat cplex  "<<stat<<"   "<<res << std::endl;
	return res;

}

LinearSolver::Status LinearSolver::writeFile(std::string name) {
	LinearSolver::Status res = FAIL;

	try {
		mycplex->exportModel("IBEX.lp");
		res = OK;
	} catch (IloException& e) {
		std::cerr << "Concert exception caught: " << e << std::endl;
		res = FAIL;
	}
	return res;
}


LinearSolver::Status LinearSolver::getCoefConstraint(Matrix &A) {
	LinearSolver::Status res = FAIL;
	try {

		IloModel model =mycplex->getModel();

		myenv->getV

//  TODO still a lot of work to continue....



		int surplus = 0, nzcnt = 0;
		int * t_cmatbeg = new int[nb_rows];
		int * t_cmatind = new int[nb_vars + (nb_rows - nb_vars) * nb_vars];
		double * t_cmatval = new double[nb_vars + (nb_rows - nb_vars) * nb_vars];

		int status = CPXgetcols(envcplex, lpcplex, &nzcnt, t_cmatbeg, t_cmatind,
				t_cmatval, nb_vars + (nb_rows - nb_vars) * nb_vars, &surplus, 0,
				nb_rows - 1);

		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				for (int j = 0; j < nb_vars; j++) {
					A[i][j] = 0;
				}
			}

			for (int i = 0; i < nb_rows - 1; i++) {
				for (int j = t_cmatbeg[i]; j < t_cmatbeg[i + 1]; j++) {
					A[i][t_cmatind[j]] = t_cmatval[j];
				}
			}
			for (int j = t_cmatbeg[nb_rows - 1]; j < nzcnt; j++) {
				A[nb_rows-1][t_cmatind[j]] = t_cmatval[j];
			}

			res = OK;
		} else
			res = FAIL;

		delete[] t_cmatbeg;
		delete[] t_cmatind;
		delete[] t_cmatval;

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getCoefConstraint_trans(Matrix &A_trans) {
	LinearSolver::Status res = FAIL;
	try {
		int surplus = 0, nzcnt = 0;
		int * t_Rmatbeg = new int[nb_vars];
		int * t_Rmatind = new int[nb_vars + (nb_rows - nb_vars) * nb_vars];
		double * t_Rmatval = new double[nb_vars + (nb_rows - nb_vars) * nb_vars];

		int status = CPXgetrows(envcplex, lpcplex, &nzcnt, t_Rmatbeg, t_Rmatind,
				t_Rmatval, nb_vars + (nb_rows - nb_vars) * nb_vars, &surplus, 0,
				nb_rows - 1);

		if (status == 0) {
			for (int i = 0; i < nb_vars; i++) {
				for (int j = 0; j < nb_rows; j++) {
					A_trans[i][j] = 0;
				}
			}

			for (int i = 0; i < nb_vars - 1; i++) {
				for (int j = t_Rmatbeg[i]; j < t_Rmatbeg[i + 1]; j++) {
					A_trans[i][t_Rmatind[j]] = t_Rmatval[j];
				}
			}
			for (int j = t_Rmatbeg[nb_vars - 1]; j < nzcnt; j++) {
				A_trans[nb_vars-1][t_Rmatind[j]] = t_Rmatval[j];
			}

			res = OK;
		} else
			res = FAIL;

		delete[] t_Rmatbeg;
		delete[] t_Rmatind;
		delete[] t_Rmatval;

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getB(IntervalVector& B) {
	LinearSolver::Status res = FAIL;
	try {
		// Get the bounds of the variables
		double * obj = new double[nb_rows];
		int status = CPXgetobj(envcplex, lpcplex, obj, 0, nb_rows - 1);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				B[i] = Interval(NEG_INFINITY, obj[i]);
			}
			res = OK;
		} else
			res = FAIL;

		delete[] obj;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getPrimalSol(Vector & primal_solution) {
	// In this modelization, the dual is directly stored in CPLEX
	// So the Primal solution could not be return
	return LinearSolver::FAIL;
}

LinearSolver::Status LinearSolver::getDualSol(Vector & dual_solution2) {
	// In this modelization, the dual is directly stored in CPLEX
	// So the Dual solution is the primal solution
	LinearSolver::Status res = FAIL;
	try {
		// the dual solution ; used by Neumaier Shcherbina test
		double * solution = new double[nb_rows];
		int status = CPXgetx(envcplex, lpcplex, solution, 0, nb_rows - 1);

		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				dual_solution2[i] = solution[i];
			}
			res = OK;
		} else
			res = FAIL;

		delete[] solution;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getInfeasibleDir(Vector & sol) {
	// In this modelization, the dual is directly stored in CPLEX
	LinearSolver::Status res = FAIL;
	try {
		double * solution = new double[nb_rows];
		int status = CPXgetray(envcplex, lpcplex, solution);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				sol[i] = solution[i];
			}
			res = OK;
		} else
			res = FAIL;

		delete[] solution;
	} catch (Exception&) {
		res = FAIL;
	}

	return res;
}

LinearSolver::Status LinearSolver::cleanConst() {
	LinearSolver::Status res = FAIL;
	try {
		int status=0;
		if ((2*nb_vars)<=  (nb_rows - 1))  {
			status = CPXdelcols (envcplex, lpcplex, nb_vars,  nb_rows - 1);
		}
		nb_rows = nb_vars;
		obj_value = POS_INFINITY;
		 if (status==0) res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;

}
LinearSolver::Status LinearSolver::cleanAll() {
	LinearSolver::Status res = FAIL;
	try {
		int status = CPXdelcols (envcplex, lpcplex, 0,  nb_rows - 1);
		nb_rows = 0;
		obj_value = POS_INFINITY;
		 if (status==0) res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setMaxIter(int max) {
	LinearSolver::Status res = FAIL;
	try {

		int status = CPXsetintparam(envcplex, CPX_PARAM_ITLIM, max);
		if (status) {
			std::cerr
					<< "Error CPLEX: Could not change the maximal number of iteration, error "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setMaxTimeOut(int time) {
	LinearSolver::Status res = FAIL;
	try {
		double t = time;
		int status = CPXsetdblparam(envcplex, CPX_PARAM_TILIM, t);
		if (status) {
			std::cerr
					<< "Error CPLEX: Could not change the maximal number of iteration "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::set_sense(Sense s) {
	LinearSolver::Status res = FAIL;
	try {

		if (s == LinearSolver::MINIMIZE) {
			CPXchgobjsen(envcplex, lpcplex, CPX_MIN); // Problem is minimization
			res = OK;
		} else if (s == LinearSolver::MAXIMIZE) {
			CPXchgobjsen(envcplex, lpcplex, CPX_MAX); //Problem is maximisation
			res = OK;
		} else
			res = FAIL;

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setVarObj(int var, double coef) {
	// In this modelization, the dual is directly stored in CPLEX
	// So the objectif is in the RHS
	LinearSolver::Status res = FAIL;
	try {
		// Changement du rhs du dual = objectif du primal (car on est en formulation dual)
		int status = CPXchgrhs(envcplex, lpcplex, 1, &var, &coef);
		if (status) {
			std::cerr
					<< "Error CPLEX: Could not change the objectif function, error "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::initBoundVar(const IntervalVector& bounds) {
	LinearSolver::Status res = FAIL;
	try {
		// Changement de la fonction objective = rhs du primal (car on est en formulation dual)
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		for (int i = 0; i < nb_vars; i++)
			tmp[i] = -bounds[i].lb();
		for (int i = 0; i < nb_vars; i++)
			tmp[nb_vars + i] = bounds[i].ub();

		int status = CPXchgobj(envcplex, lpcplex, nb_vars * 2, indice, tmp);

		if (status) {
			std::cerr
					<< "Error CPLEX: Could not change the LHS and RHS of each constraint associated to the bounds of the variable, error "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setBoundVar(int var, const Interval& bound) {
	LinearSolver::Status res = FAIL;
	try {
		int * ind = new int[2];
		ind[0] = var;
		ind[1] = var + nb_vars;
		tmp[0] = -bound.lb();
		tmp[1] = bound.ub();
		int status = CPXchgobj(envcplex, lpcplex, 2, indice, tmp);

		if (status) {
			std::cerr
					<< "Error CPLEX: Could not change the bound of a variable, error "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
		delete[] ind;

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setEpsilon(double eps) {
	LinearSolver::Status res = FAIL;
	try {
		epsilon = eps;
		int status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, eps);
		if (status) {
			std::cerr << "Error CPLEX: Could not change the precision, error "
					<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;

}

LinearSolver::Status LinearSolver::addConstraint(const ibex::Vector& row, CmpOp sign, double rhs) {
	LinearSolver::Status res = FAIL;
	try {
		int status;
		double * pt_obj;
		double * pt_lb;


		if (sign == LEQ || sign == LT || sign == GEQ || sign == GT) {

			if (sign == LEQ || sign == LT) {
				*pt_obj = rhs;
				for (int i = 0; i < nb_vars; i++)
					cmatval[i] = row[i];
			} else {
				*pt_obj = -rhs;
				for (int i = 0; i < nb_vars; i++)
					cmatval[i] = -row[i];
			}

			status = CPXaddcols(envcplex, lpcplex, 1, nb_vars, pt_obj, cmatbeg,
					cmatind, cmatval, NULL, NULL, NULL);
			nb_rows++;
			res = OK;
		} else
			res = FAIL;

	} catch (Exception&) {
		res = FAIL;
	}

	return res;
}

#endif  // END DEF with ILOCPLEX
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _IBEX_WITH_NOLP_



LinearSolver::LinearSolver(int nb_vars, int max_iter,
	int max_time_out, double eps):
	nb_vars(0), nb_rows(0), obj_value(POS_INFINITY), sense(LinearSolver::MINIMIZE),
	primal_solution(1), dual_solution(1 /*tmp*/),
	status_prim(false), status_dual(false), boundvar(1)
{
	ibex_warning("No Linear Solver available.");
}

LinearSolver::~LinearSolver() {
}

LinearSolver::Status_Sol LinearSolver::solve() {
	return LinearSolver::UNKNOWN;
}

void LinearSolver::write_file(const char* name) {
	throw LPException();
}


void LinearSolver::get_rows(Matrix& A) const{
	throw LPException();
}

void LinearSolver::get_rows_trans(Matrix& A_trans) const{
	throw LPException();
}

void LinearSolver::get_lhs_rhs(IntervalVector& lhs_rhs) const{
	throw LPException();
}

void LinearSolver::get_coef_obj(Vector& obj) const{
	throw LPException();
}

double LinearSolver::get_epsilon() const{
	throw LPException();
}

void LinearSolver::clean_ctrs(){
	throw LPException();
}

void LinearSolver::set_max_iter(int max){
	throw LPException();
}

void LinearSolver::set_max_time_out(int time){
	throw LPException();
}

void LinearSolver::set_sense(Sense s){
	throw LPException();
}

void LinearSolver::set_obj(const Vector& coef){
	throw LPException();
}

void LinearSolver::set_obj_var(int var, double coef){
	throw LPException();
}

void LinearSolver::set_bounds(const IntervalVector& bounds){
	throw LPException();
}

void LinearSolver::set_bounds_var(int var, const Interval& bound){
	throw LPException();
}

void LinearSolver::set_epsilon(double eps){
	throw LPException();
}

void LinearSolver::add_constraint(const Vector & row, CmpOp sign, double rhs ){
	throw LPException();
}




#endif //






}
 // end namespace ibex

