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


namespace ibex {

const double LinearSolver::default_eps = 1e-10;
const double LinearSolver::default_max_bound = 1e20;
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
	case(LinearSolver::INFEASIBLE_NOTPROVED) :{
			os << "INFEASIBLE_NOTPROVED";
			break;
	}
	case(LinearSolver::INFEASIBLE) :{
			os << "INFEASIBLE";
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




#ifdef _IBEX_WITH_SOPLEX_


LinearSolver::LinearSolver(int nb_vars1, int nb_ctr, int max_iter, int max_time_out, double eps) :
			nb_ctrs(nb_ctr), nb_vars(nb_vars1), nb_rows(0), obj_value(0.0), epsilon(eps),
			primal_solution(new double[nb_vars1]), dual_solution(NULL),
			status_prim(0), status_dual(0)  {


	mysoplex= new soplex::SoPlex();

	mysoplex->changeSense(soplex::SPxLP::MINIMIZE);
	mysoplex->setTerminationIter(max_iter);
	mysoplex->setTerminationTime(max_time_out);
	mysoplex->setDelta(epsilon);

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
	delete [] primal_solution;
	if (dual_solution!=NULL) delete [] dual_solution;
	delete mysoplex;
}

LinearSolver::Status_Sol LinearSolver::solve() {

	soplex::SPxSolver::Status stat = soplex::SPxSolver::UNKNOWN;
	LinearSolver::Status_Sol res= UNKNOWN;

	try{
	    stat = mysoplex->solve();
		if (stat==soplex::SPxSolver::OPTIMAL) {
		  obj_value = mysoplex->objValue();

		  // the primal solution : used by choose_next_variable
		  soplex::DVector primal(nb_vars);
		  status_prim = mysoplex->getPrimal(primal);
		  for (int i=0; i< nb_vars ; i++) {
			  primal_solution[i]=primal[i];
		  }
		  // the dual solution ; used by Neumaier Shcherbina test
		  soplex::DVector dual(nb_rows);
		  if (dual_solution != NULL) delete [] dual_solution;
		  dual_solution = new double[nb_rows];
		  status_dual = mysoplex->getDual(dual);
		  for (int i=0; i<nb_rows; i++) {
			  if 	( ((mysoplex->rhs(i) >=  default_max_bound) && (dual[i]<=0)) ||
					  ((mysoplex->lhs(i) <= -default_max_bound) && (dual[i]>=0))   ) {
				  dual_solution[i]=0;
			  }
			  else {
				  dual_solution[i]=dual[i];
			  }
		  }
		  res= OPTIMAL;
		}
		else if (stat==soplex::SPxSolver::ABORT_TIME)
			res = TIME_OUT;
		else if (stat==soplex::SPxSolver::ABORT_ITER)
			res = MAX_ITER;
		else if (stat==soplex::SPxSolver::INFEASIBLE)
			res = INFEASIBLE_NOTPROVED;
		else
			res = UNKNOWN;

	}catch(soplex::SPxException&){
		res = UNKNOWN;
	}
	//	std::cout <<"	stat soplex  "<<stat<<"   "<<res << std::endl;
	return res;

}

LinearSolver::Status LinearSolver::writeFile(const char* name) {
	LinearSolver::Status res= FAIL;
	try {
		mysoplex->writeFile(name, NULL, NULL, NULL);
		res = OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

int LinearSolver::getNbRows() const {
	return nb_rows;
}

double LinearSolver::getObjValue() const {
	return obj_value;
}


double LinearSolver::getEpsilon() const {
	return epsilon;
}

LinearSolver::Status LinearSolver::getCoefConstraint(Matrix &A) {
	LinearSolver::Status res= FAIL;
	try {
		for (int i=0;i<nb_rows; i++){
			for (int j=0;j<nb_vars; j++){
				A.row(i)[j] =  mysoplex->rowVector(i)[j];
			}
		}
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getCoefConstraint_trans(Matrix &A_trans) {
	LinearSolver::Status res= FAIL;
	try {
		for (int i=0;i<nb_rows; i++){
			for (int j=0;j<nb_vars; j++){
				A_trans.row(j)[i] =  mysoplex->rowVector(i)[j];
			}
		}
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status  LinearSolver::getB(IntervalVector& B) {
	LinearSolver::Status res= FAIL;
	try {
		// Get the bounds of the variables
		for (int i=0;i<nb_vars; i++){
			B[i]=Interval( mysoplex->lhs(i) , mysoplex->rhs(i) );
		}

		// Get the bounds of the constraints
		for (int i=nb_vars;i<nb_rows; i++){
			B[i]=Interval( 	(mysoplex->lhs(i)>-default_max_bound)? mysoplex->lhs(i):-default_max_bound,
					        (mysoplex->rhs(i)< default_max_bound)? mysoplex->rhs(i): default_max_bound   );
			//	B[i]=Interval( 	mysoplex->lhs(i),mysoplex->rhs(i));
			//Idea: replace 1e20 (resp. -1e20) by Sup([g_i]) (resp. Inf([g_i])), where [g_i] is an evaluation of the nonlinear function <-- IA
			//           cout << B(i+1) << endl;
		}
		res = OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}


LinearSolver::Status LinearSolver::getPrimalSol(Vector & solution_primal) {
	LinearSolver::Status res= FAIL;
	try {
		if (status_prim == soplex::SPxSolver::OPTIMAL) {
			for (int i=0; i< nb_vars ; i++) {
				solution_primal[i] = primal_solution[i];
			}
			res = OK;
		}
	}
	catch(soplex::SPxException&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getDualSol(Vector & solution_dual) {
	LinearSolver::Status res= FAIL;
	try {
		if (status_dual == soplex::SPxSolver::OPTIMAL) {
			for (int i=0; i<nb_rows; i++) {
				solution_dual[i] = dual_solution[i];
			}
			res =OK;
		}
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getInfeasibleDir(Vector & sol) {
	LinearSolver::Status res= FAIL;
	try {
		soplex::SPxSolver::Status stat1;
		soplex::DVector sol_found(nb_rows);
		mysoplex->getDualfarkas(sol_found);
		//stat1 = mysoplex->getDualfarkas(sol_found);
		// if (stat1==soplex::SPxSolver::OPTIMAL) // TODO I'm not sure of the value that return getDualfarkas : this condition does not work BNE


		for (int i=0; i<nb_rows; i++) {
			if (((mysoplex->lhs(i) <= -default_max_bound) && (sol_found[i]>=0))||
				((mysoplex->rhs(i) >=  default_max_bound) && (sol_found[i]<=0))	) {
				sol[i]=0.0;
				}
			else {
				sol[i]=sol_found[i];
			}
		}

		res =OK;
		//	else	res = FAIL; this condition does not work BNE


	}
	catch(soplex::SPxException& ) {
		res =  FAIL;
	}

	return res;
}

LinearSolver::Status LinearSolver::cleanConst() {
	LinearSolver::Status res= FAIL;
	try {
		if (dual_solution!=NULL) delete[] dual_solution;
		dual_solution=NULL;
		status_prim = 0;
		status_dual = 0;
		mysoplex->removeRowRange(nb_vars, nb_rows-1);
		nb_rows = nb_vars;
		obj_value = POS_INFINITY;
		res= OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;

}
LinearSolver::Status LinearSolver::cleanAll() {
	LinearSolver::Status res= FAIL;
	try {
		if (dual_solution!=NULL) delete[] dual_solution;
		dual_solution=NULL;
		status_prim = 0;
		status_dual = 0;
		mysoplex->removeRowRange(0, nb_rows-1);
		nb_rows = 0;
		obj_value = POS_INFINITY;
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}


LinearSolver::Status LinearSolver::setMaxIter(int max) {
	LinearSolver::Status res= FAIL;
	try {
		mysoplex->setTerminationIter(max);
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setMaxTimeOut(int time) {
	LinearSolver::Status res= FAIL;
	try {
		double t = time;
		mysoplex->setTerminationTime(t);
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setSense(Sense s) {
	LinearSolver::Status res= FAIL;
	try {
		if (s==LinearSolver::MINIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MINIMIZE);
			res = OK;
		}
		else if (s==LinearSolver::MAXIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MAXIMIZE);
			res =OK;
		}
		else
			res = FAIL;

	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}



LinearSolver::Status LinearSolver::setVarObj(int var, double coef) {
	LinearSolver::Status res= FAIL;
	try {
		mysoplex->changeObj(var, coef);
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::initBoundVar(IntervalVector bounds) {
	LinearSolver::Status res= FAIL;
	try {
		for (int j=0; j<nb_vars; j++){
			// Change the LHS and RHS of each constraint associated to the bounds of the variable
			mysoplex->changeRange(j ,bounds[j].lb(),bounds[j].ub());
		}
		res = OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setBoundVar(int var, Interval bound) {
	LinearSolver::Status res= FAIL;
	try {
		mysoplex->changeRange(var ,bound.lb(),bound.ub());

		//std::cout << "improve bound var "<<var<< std::endl;
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setEpsilon(double eps) {
	LinearSolver::Status res= FAIL;
	try {
		epsilon = eps;
		mysoplex->setDelta(eps);
		res =OK;
	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::addConstraint(ibex::Vector& row, CmpOp sign, double rhs) {
	LinearSolver::Status res= FAIL;
	try {
		soplex::DSVector row1(nb_vars);
		for (int i=0; i< nb_vars ; i++) {
			row1.add(i, row[i]);
		}

		if (sign==LEQ || sign==LT) {
			mysoplex->addRow(soplex::LPRow(-soplex::infinity, row1, rhs));
			nb_rows++;
			res=  OK;
		}
		else if (sign==GEQ || sign==GT) {
			mysoplex->addRow(soplex::LPRow(rhs, row1, soplex::infinity));
			nb_rows++;
			res = OK;
		}
		else
			res = FAIL;

	}
	catch(soplex::SPxException& ) {
		res = FAIL;
	}

	return res;
}







#endif  // END DEF with SOPLEX






////////////////////////////////////////////////////////////////////////////////////////////////////////// TODO
#ifdef _IBEX_WITH_CPLEX_

LinearSolver::LinearSolver(int nb_vars1, int nb_ctr1, int max_iter,
		int max_time_out, double eps) :
		nb_ctrs(nb_ctr1), nb_vars(nb_vars1), nb_rows(0), obj_value(0.0),
		epsilon(eps), envcplex(NULL), lpcplex(NULL),
		primal_solution(new double[nb_vars1]), dual_solution(NULL),
		status_prim(0), status_dual(0) {

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
		return;
	}

	if (epsilon <1.e-9) epsilon=1.e-9;
	if (epsilon >1.e-1) epsilon=1.e-1;


	status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, epsilon);
	if (status != 0) {
		char* errmsg = new char[1024];
		std::cerr << "Error CPLEX: Could not change the precision, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		return;
	}


	status = CPXsetintparam(envcplex, CPX_PARAM_ITLIM, max_iter);
	if (status != 0) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not change the maximal number of iteration, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		return;
	}

	status = CPXsetdblparam(envcplex, CPX_PARAM_TILIM, max_time_out);
	if (status != 0) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not change the maximal number of iteration "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		return;
	}

	status = CPXsetintparam(envcplex, CPX_PARAM_PREDUAL, -1);
	if (status != 0) {
		char* errmsg = new char[1024];
		std::cerr<< "Error CPLEX: Could not impose the preprocessing dual, error "<< status << std::endl;
		CPXgeterrorstring(envcplex, status, errmsg);
		std::cerr << errmsg << std::endl;
		delete[] errmsg;
		return;
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
		return;
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
	if (status != 0) {
		std::cerr << "Error CPLEX: Could not add a new column, error " << status
				<< std::endl;
		return;
	}

	status = CPXaddrows(envcplex, lpcplex, 0, nb_vars*2, nb_vars * 2, rhs, sense,
			c_matbeg, c_matind, c_matval, NULL, NULL);

	if (status != 0) {
		std::cerr << "Error CPLEX: Could not add a new constraint, error "<< status << std::endl;
		return;
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
			std::cerr << "CPXfreeprob failed, error code " << status
					<< std::endl;
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

	delete[] primal_solution;
	if (dual_solution!= NULL) delete[] dual_solution;
	delete[] indice;
	delete[] tmp;
	delete[] r_matbeg;
	delete[] r_matval;
	delete[] r_matind;
}

LinearSolver::Status_Sol LinearSolver::solve() {

	LinearSolver::Status_Sol res = UNKNOWN;
	try {
		// Optimize the problem and obtain solution.

		int status = CPXlpopt(envcplex, lpcplex);

		if (status == 0) {
			int solstat = CPXgetstat(envcplex, lpcplex);

			if (solstat == CPX_STAT_ABORT_IT_LIM) {
				res = MAX_ITER;
			} else if (solstat == CPX_STAT_ABORT_TIME_LIM) {
				res = TIME_OUT;
			} else if (solstat == CPX_STAT_OPTIMAL) {
				CPXgetobjval(envcplex, lpcplex, &obj_value);
				status_prim =CPXgetx(envcplex, lpcplex, primal_solution, 0, nb_vars - 1);
				if (dual_solution != NULL) delete [] dual_solution;
				dual_solution = new double[nb_rows];
				status_dual = CPXgetpi(envcplex, lpcplex, dual_solution, 0, nb_rows - 1);
				res = OPTIMAL;
			} else if ((solstat == CPX_STAT_INFEASIBLE)
					|| (solstat == CPX_STAT_UNBOUNDED)
					|| (solstat == CPX_STAT_OPTIMAL_INFEAS)) {
				res = INFEASIBLE_NOTPROVED;
			} else
				res = UNKNOWN;
		}

	} catch (Exception&) {
		res = UNKNOWN;
	}
	//std::cout <<"	stat cplex  "<<stat<<"   "<<res << std::endl;
	return res;

}

LinearSolver::Status LinearSolver::writeFile(const char* name) {
	LinearSolver::Status res = FAIL;

	try {
		int status = CPXwriteprob(envcplex, lpcplex, name, NULL);
		if (status != 0) {
			std::cerr << "Failed to write LP to disk." << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

int LinearSolver::getNbRows() const {
	return nb_rows;
}

double LinearSolver::getObjValue() const {
	return obj_value;
}

double LinearSolver::getEpsilon() const {
	return epsilon;
}

LinearSolver::Status LinearSolver::getCoefConstraint(Matrix &A) {
	LinearSolver::Status res = FAIL;
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

LinearSolver::Status LinearSolver::getCoefConstraint_trans(Matrix &A_trans) {
	LinearSolver::Status res = FAIL;
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

LinearSolver::Status LinearSolver::getB(IntervalVector& B) {
	LinearSolver::Status res = FAIL;
	try {
		// Get the bounds of the variables
		double * rhs = new double[nb_rows];
		int status = CPXgetrhs(envcplex, lpcplex, rhs, 0, nb_rows - 1);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				B[i] = Interval(-default_max_bound, rhs[i]);
			}
			res = OK;
		} else
			res = FAIL;

		delete[] rhs;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getPrimalSol(Vector & primal) {
	// In this modelization, the dual is directly stored in CPLEX
	// So the Primal solution could not be return
	LinearSolver::Status res = FAIL;
	try {
		if (status_prim == 0) {
			for (int i = 0; i < nb_vars; i++) {
				primal[i] = primal_solution[i];
			}
			res = OK;
		} else {
			res = FAIL;
		}

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getDualSol(Vector & dual) {
	// In this modelization, the dual is directly stored in CPLEX
	// So the Dual solution is the primal solution of the problem in cplex
	LinearSolver::Status res = FAIL;
	try {
		// the dual solution ; used by Neumaier Shcherbina test

		if (status_dual == 0) {

			for (int i = 0; i < nb_rows; i++) {
				dual[i] = (dual_solution[i]< 0) ? dual_solution[i] : 0.0;
				//dual[i] = dual_solution[i];
			}

			res = OK;
		} else
			res = FAIL;

	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::getInfeasibleDir(Vector & sol) {
	// In this modelization, the dual is directly stored in CPLEX
	LinearSolver::Status res = FAIL;
	try {
		double * solution_found = new double[nb_rows];

		int status = CPXdualfarkas(envcplex, lpcplex, solution_found,NULL);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				sol[i] = (solution_found[i]< 0) ? solution_found[i] : 0.0;
			}
			res = OK;
		} else
			res = FAIL;

		delete[] solution_found;

	} catch (Exception&) {
		res = FAIL;
	}

	return res;
}

LinearSolver::Status LinearSolver::cleanConst() {
	LinearSolver::Status res = FAIL;
	try {
		if (dual_solution!=NULL) delete[] dual_solution;
		dual_solution=NULL;
		int status = CPXdelrows (envcplex, lpcplex, 2*nb_vars,  nb_rows - 1);
		nb_rows = 2*nb_vars;
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
		if (dual_solution!=NULL) delete[] dual_solution;
		dual_solution=NULL;
		int status = CPXdelrows (envcplex, lpcplex, 0,  nb_rows - 1);
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
		if (status != 0) {
			//std::cerr<< "Error CPLEX: Could not change the maximal number of iteration, error "<< status << std::endl;
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
		if (status != 0) {
			//std::cerr<< "Error CPLEX: Could not change the maximal number of iteration "<< status << std::endl;
			res = FAIL;
		} else
			res = OK;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

LinearSolver::Status LinearSolver::setSense(Sense s) {
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

		/* Changement du rhs du dual = objectif du primal (car on est en formulation dual) */
		int status = CPXchgobj(envcplex, lpcplex, 1, &var, &coef);
		if (status != 0) {
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

LinearSolver::Status LinearSolver::initBoundVar(IntervalVector bounds) {
	LinearSolver::Status res = FAIL;
	try {
		// Changement de la fonction objective = rhs du primal (car on est en formulation dual)
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		for (int i = 0; i < nb_vars; i++)
			tmp[i] = -bounds[i].lb();
		for (int i = 0; i < nb_vars; i++)
			tmp[nb_vars + i] = bounds[i].ub();

		int status = CPXchgrhs(envcplex, lpcplex, (nb_vars * 2), indice, tmp);

		if (status != 0) {
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

LinearSolver::Status LinearSolver::setBoundVar(int var, Interval bound) {
	LinearSolver::Status res = FAIL;
	try {
		int * ind = new int[2];
		ind[0] = var;
		ind[1] = var + nb_vars;
		tmp[0] = -bound.lb();
		tmp[1] = bound.ub();
		int status = CPXchgrhs(envcplex, lpcplex, 2, ind, tmp);

		if (status != 0) {
			//std::cerr<< "Error CPLEX: Could not change the bound of a variable, error "<< status << std::endl;
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

		if (eps <1.e-9) eps=1.e-9;
		if (eps >1.e-1) eps=1.e-1;

		int status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, epsilon);
		if (status != 0) {
			//std::cerr << "Error CPLEX: Could not change the precision, error "<< status << std::endl;
			res = FAIL;
		} else {
			epsilon = eps;
			res = OK;
		}
	} catch (Exception&) {
		res = FAIL;
	}
	return res;

}

LinearSolver::Status LinearSolver::addConstraint(ibex::Vector& row, CmpOp sign, double rhs) {
	LinearSolver::Status res = FAIL;
	try {
		double * pt_rhs= new double[1];
		char cc = 'L';
		if (sign == LEQ || sign == LT || sign == GEQ || sign == GT) {

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
			if (status==0) {
				nb_rows++;
				res = OK;
			} else {
				res = FAIL;
			}
		} else
			res = FAIL;

	delete[] pt_rhs;
	} catch (Exception&) {
		res = FAIL;
	}
	return res;
}

#endif  // END DEF with CPLEX






////////////////////////////////////////////////////////////////////////////////////////////////////////// TODO
#ifdef _IBEX_WITH_ILOCPLEX_

LinearSolver::LinearSolver(int nb_vars1, int nb_ctr1, int max_iter,
		int max_time_out, double eps) :
		nb_ctrs(nb_ctr1), nb_vars(nb_vars1), nb_rows(0), obj_value(0.0), epsilon(
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
					|| (solstat == IloCplex::InfOrUnbd )
					|| (solstat == IloCplex::OptimalInfeas)) {
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

int LinearSolver::getNbRows() const {
	return nb_rows;
}

double LinearSolver::getObjValue() const {
	return obj_value;
}

double LinearSolver::getEpsilon() const {
	return epsilon;
}

LinearSolver::Status LinearSolver::getCoefConstraint(Matrix &A) {
	LinearSolver::Status res = FAIL;
	try {

		IloModel model =mycplex->getModel();

		myenv->getV

//  TODO



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
		int status = CPXdelcols (envcplex, lpcplex, nb_vars,  nb_rows - 1);
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
		if (status != 0) {
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
		if (status != 0) {
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

LinearSolver::Status LinearSolver::setSense(Sense s) {
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
		/* Changement du rhs du dual = objectif du primal (car on est en formulation dual) */
		int status = CPXchgrhs(envcplex, lpcplex, 1, &var, &coef);
		if (status != 0) {
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

LinearSolver::Status LinearSolver::initBoundVar(IntervalVector bounds) {
	LinearSolver::Status res = FAIL;
	try {
		// Changement de la fonction objective = rhs du primal (car on est en formulation dual)
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		for (int i = 0; i < nb_vars; i++)
			tmp[i] = -bounds[i].lb();
		for (int i = 0; i < nb_vars; i++)
			tmp[nb_vars + i] = bounds[i].ub();

		int status = CPXchgobj(envcplex, lpcplex, nb_vars * 2, indice, tmp);

		if (status != 0) {
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

LinearSolver::Status LinearSolver::setBoundVar(int var, Interval bound) {
	LinearSolver::Status res = FAIL;
	try {
		int * ind = new int[2];
		ind[0] = var;
		ind[1] = var + nb_vars;
		tmp[0] = -bound.lb();
		tmp[1] = bound.ub();
		int status = CPXchgobj(envcplex, lpcplex, 2, indice, tmp);

		if (status != 0) {
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
		if (status != 0) {
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

LinearSolver::Status LinearSolver::addConstraint(ibex::Vector& row, CmpOp sign, double rhs) {
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






} // end namespace ibex


