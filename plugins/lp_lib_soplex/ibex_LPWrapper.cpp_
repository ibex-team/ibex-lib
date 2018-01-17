LPSolver::LPSolver(int nb_vars1, int max_iter, int max_time_out, double eps) :
			nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1) , sense(LPSolver::MINIMIZE),
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

LPSolver::~LPSolver() {
	delete mysoplex;
}

LPSolver::Status_Sol LPSolver::solve() {
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

void LPSolver::write_file(const char* name) {
	try {
		mysoplex->writeFile(name, NULL, NULL, NULL);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

ibex::Vector LPSolver::get_coef_obj() const {
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

ibex::Matrix LPSolver::get_rows() const {
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


ibex::Matrix LPSolver::get_rows_trans() const {
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

IntervalVector  LPSolver::get_lhs_rhs() const{
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

ibex::Vector LPSolver::get_infeasible_dir() const {

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

double LPSolver::get_epsilon() const {
	return mysoplex->delta();
}

void LPSolver::clean_ctrs() {

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


void LPSolver::set_max_iter(int max) {

	try {
		mysoplex->setTerminationIter(max);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_max_time_out(int time) {

	try {
		double t = time;
		mysoplex->setTerminationTime(t);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_sense(Sense s) {

	try {
		if (s==LPSolver::MINIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MINIMIZE);
			sense = LPSolver::MINIMIZE;
		}
		else if (s==LPSolver::MAXIMIZE) {
			mysoplex->changeSense(soplex::SPxLP::MAXIMIZE);
			sense = LPSolver::MAXIMIZE;
		}
		else
			throw LPException();

	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_obj(const ibex::Vector& coef) {

	try {
		for (int i=0;i< nb_vars; i++) {
			mysoplex->changeObj(i,coef[i]);
		}
	}
	catch(...) {
		throw LPException();
	}

	return;
}

void LPSolver::set_obj_var(int var, double coef) {

	try {
		mysoplex->changeObj(var, coef);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_bounds(const IntervalVector& bounds) {

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

void LPSolver::set_bounds_var(int var, const Interval& bound) {

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

void LPSolver::set_epsilon(double eps) {

	try {
		mysoplex->setDelta(eps);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

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


