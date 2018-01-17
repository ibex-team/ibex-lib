
LPSolver::LPSolver(int nb_vars1, int max_iter, int max_time_out, double eps) :
			nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1), sense(LPSolver::MINIMIZE),
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

LPSolver::~LPSolver() {
	delete myclp;
	delete [] _col1Index;
}

LPSolver::Status_Sol LPSolver::solve() {
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

void LPSolver::write_file(const char* name) {
	try {
		myclp->writeMps(name);
	}
	catch(...) {
		throw LPException();
	}
	return;
}


double LPSolver::get_epsilon() const {
	try  {
		return myclp->primalTolerance();
	}
	catch(...) {
		throw LPException();
	}
}


Vector LPSolver::get_coef_obj() const {
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


Matrix LPSolver::get_rows() const {
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

Matrix LPSolver::get_rows_trans() const {
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

IntervalVector  LPSolver::get_lhs_rhs() const {
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




Vector LPSolver::get_infeasible_dir() const {
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

void LPSolver::clean_ctrs() {
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


void LPSolver::set_max_iter(int max) {

	try {
		myclp->setMaximumIterations(max);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_max_time_out(int time) {

	try {
		myclp->setMaximumSeconds((double)time);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_sense(Sense s) {

	try {
			/// Direction of optimization (1 - minimize, -1 - maximize, 0 - ignore
		if (s==LPSolver::MINIMIZE) {
			myclp->setOptimizationDirection(1);
			sense = LPSolver::MINIMIZE;
		}
		else if (s==LPSolver::MAXIMIZE) {
			myclp->setOptimizationDirection(-1);
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


void LPSolver::set_obj(const Vector& coef) {

	try {
		for (int i =0; i< nb_vars; i++)
			myclp->setObjectiveCoefficient(i, coef[i]);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::set_obj_var(int var, double coef) {

	try {
		myclp->setObjectiveCoefficient(var, coef);
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
			myclp->setRowBounds(j,bounds[j].lb(),bounds[j].ub());
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
		myclp->setRowBounds(var,bound.lb(),bound.ub());
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
		myclp->setPrimalTolerance(eps);
	}
	catch(...) {
		throw LPException();
	}
	return ;
}

void LPSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

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





