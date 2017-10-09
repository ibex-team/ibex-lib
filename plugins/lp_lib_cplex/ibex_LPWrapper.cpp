LPSolver::LPSolver(int nb_vars1, int max_iter,
		int max_time_out, double eps) :
		nb_vars(nb_vars1), nb_rows(0), epsilon(eps), boundvar(nb_vars1),
		 obj_value(0.0), primal_solution(nb_vars1), dual_solution(1 /*tmp*/),
		status_prim(-1), status_dual(-1),
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

	if (epsilon <1.e-9) epsilon=1.e-9;
	if (epsilon >1.e-1) epsilon=1.e-1;


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

LPSolver::~LPSolver() {
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

LPSolver::Status_Sol LPSolver::solve() {

	try {
		// Optimize the problem and obtain solution.

		int status = CPXlpopt(envcplex, lpcplex);

		if (status == 0) {
			int solstat = CPXgetstat(envcplex, lpcplex);

			switch (solstat) {
			case (CPX_STAT_OPTIMAL): {
				double tmp;
				int status = CPXgetobjval(envcplex, lpcplex, &tmp);
				obj_value = tmp;

				if (status) {
					std::cerr << "LPSolver: Failed to get the objective value." << std::endl;
					throw LPException();
				}
				status_prim =CPXgetx(envcplex, lpcplex, primal_solution.raw(), 0, nb_vars - 1);

				dual_solution.resize(nb_rows);

				status_dual = CPXgetpi(envcplex, lpcplex, dual_solution.raw(), 0, nb_rows - 1);
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

void LPSolver::write_file(const char* name) {
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


void LPSolver::get_coef_obj(Vector& obj) const {
	//TODO
	return;
}


void LPSolver::get_rows(Matrix &A) const {
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
	return;
}

void LPSolver::get_rows_trans(Matrix &A_trans) const {

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
	return ;
}

void LPSolver::get_lhs_rhs(IntervalVector& B) const {
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
	return ;
}

void LPSolver::get_primal_sol(Vector & primal) const {

	try {
		if (status_prim == 0) {
			for (int i = 0; i < nb_vars; i++) {
				primal[i] = primal_solution[i];
			}
		} else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
	return;
}

void LPSolver::get_dual_sol(Vector & dual) const {

	try {
		// the dual solution ; used by Neumaier Shcherbina test
		if (status_dual == 0) {
			for (int i = 0; i < nb_rows; i++) {
				dual[i] = (dual_solution[i]< 0) ? dual_solution[i] : 0.0;
				//dual[i] = dual_solution[i];
			}
		} else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
	return ;
}

void LPSolver::get_infeasible_dir(Vector & sol) const {

	try {
		double * solution_found = new double[nb_rows];

		int status = CPXdualfarkas(envcplex, lpcplex, solution_found,NULL);
		if (status == 0) {
			for (int i = 0; i < nb_rows; i++) {
				sol[i] = (solution_found[i]< 0) ? solution_found[i] : 0.0;
			}
		}
		delete[] solution_found;

		if (status) 	throw LPException();

	} catch (...) {
		throw LPException();
	}

	return;
}

void LPSolver::clean_ctrs() {

	try {
		status_prim = -1;
		status_dual = -1;
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
void LPSolver::clean_all() {
// TODO
}

void LPSolver::set_max_iter(int max) {

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

void LPSolver::set_max_time_out(int time) {

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

void LPSolver::set_sense(Sense s) {

	try {

		if (s == LPSolver::MINIMIZE) {
			int status = CPXchgobjsen(envcplex, lpcplex, CPX_MIN); // Problem is minimization
			if (status) {
				std::cerr<< "Error CPLEX: Could not change the sense, error "<< status << std::endl;
				throw LPException();
			}
		} else if (s == LPSolver::MAXIMIZE) {
			int status = CPXchgobjsen(envcplex, lpcplex, CPX_MAX); //Problem is maximisation
			if (status) {
				std::cerr<< "Error CPLEX: Could not change the sense, error "<< status << std::endl;
				throw LPException();
			}
		} else
			throw LPException();

	} catch (...) {
		throw LPException();
	}
	return ;
}

void set_obj(const Vector& coef) {
	// TODO
}

void LPSolver::set_obj_var(int var, double coef) {

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

void LPSolver::set_bounds(const IntervalVector& bounds) {

	try {
		// Changement de la fonction objective = rhs du primal (car on est en formulation dual)
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		for (int i = 0; i < nb_vars; i++)
			tmp[i] = -bounds[i].lb();
		for (int i = 0; i < nb_vars; i++)
			tmp[nb_vars + i] = bounds[i].ub();

		int status = CPXchgrhs(envcplex, lpcplex, (nb_vars * 2), indice, tmp);
		boundvar = bounds;

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

void LPSolver::set_bounds_var(int var, const Interval& bound) {

	try {
		int * ind = new int[2];
		ind[0] = var;
		ind[1] = var + nb_vars;
		tmp[0] = -bound.lb();
		tmp[1] = bound.ub();
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

void LPSolver::set_epsilon(double eps) {

	try {

		if (eps <1.e-9) eps=1.e-9;
		if (eps >1.e-1) eps=1.e-1;

		int status = CPXsetdblparam(envcplex, CPX_PARAM_EPOPT, epsilon);
		if (status) {
			//std::cerr << "Error CPLEX: Could not change the precision, error "<< status << std::endl;
			throw LPException();
		} else {
			epsilon = eps;
		}
	} catch (...) {
		throw LPException();
	}
	return ;

}

void LPSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {

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
