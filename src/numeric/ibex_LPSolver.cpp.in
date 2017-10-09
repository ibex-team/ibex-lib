//============================================================================
//                                  I B E X
// Interface with the linear solver
// File        : ibex_LPSolver.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 15, 2013
// Last Update : May 15, 2013
//============================================================================

#include  <cfloat>
#include "ibex_LPSolver.h"

namespace ibex {

const double LPSolver::default_eps = @IBEX_LP_LIB_DEFAULT_EPS@;

const double LPSolver::default_max_bound = 1e20;
const int LPSolver::default_max_time_out=100;
const int LPSolver::default_max_iter=100;
const Interval LPSolver::default_limit_diam_box = Interval(1.e-14,1.e6);



/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const LPSolver::Status_Sol x){

	switch(x) {
	case(LPSolver::OPTIMAL) :{
			os << "OPTIMAL";
			break;
	}
	case(LPSolver::INFEASIBLE) :{
			os << "INFEASIBLE";
			break;
	}
	case(LPSolver::OPTIMAL_PROVED) :{
			os << "OPTIMAL_PROVED";
			break;
	}
	case(LPSolver::INFEASIBLE_PROVED) :{
			os << "INFEASIBLE_PROVED";
			break;
	}
	case(LPSolver::TIME_OUT) :{
			os << "TIME_OUT";
			break;
	}
	case(LPSolver::MAX_ITER) :{
			os << "MAX_ITER";
			break;
	}
	case(LPSolver::UNKNOWN) :{
		os << "UNKNOWN";
		break;
	}
	}
	return os;

}


int LPSolver::get_nb_rows() const {
	return nb_rows;
}

Interval LPSolver::get_obj_value() const {
	return obj_value;
}

/*
 * TODO: get the actual internal precision of the LP solver
 */
double LPSolver::get_epsilon() const {
	return epsilon;
}

///////////////////////////////////////////////////////////////////////////////////

LPSolver::Status_Sol LPSolver::solve_var(LPSolver::Sense sense, int var, Interval& obj) {
	assert((0<=var)&&(var<=nb_vars));

	LPSolver::Status_Sol stat = LPSolver::UNKNOWN;

	try {
		// the linear solver is always called in a minimization mode : in case of maximization of var , the opposite of var is minimized
		if(sense==LPSolver::MINIMIZE)
			set_obj_var(var, 1.0);
		else
			set_obj_var(var, -1.0);

		//	mylinearsolver->writeFile("coucou.lp");
		//	system("cat coucou.lp");
		stat = solve();
		// std::cout << "[polytope-hull]->[run_simplex] solver returns " << stat << std::endl;
		switch (stat) {
		case LPSolver::OPTIMAL : {
			// Neumaier - Shcherbina postprocessing
			obj_value = neumaier_shcherbina_postprocessing_var(var, sense);
			obj = obj_value;
			stat = LPSolver::OPTIMAL_PROVED;
			break;
		}
		case  LPSolver::INFEASIBLE: {
			// infeasibility test  cf Neumaier Shcherbina paper
			if (neumaier_shcherbina_infeasibilitytest()) {
				stat = LPSolver::INFEASIBLE_PROVED;
			}
			break;
		}
		default:
			stat = LPSolver::UNKNOWN;
			break;
		}
		// Reset the objective of the LP solver
		set_obj_var(var, 0.0);
	}
	catch (LPException&) {
		stat = LPSolver::UNKNOWN;
		// Reset the objective of the LP solver
		set_obj_var(var, 0.0);
	}

	return stat;

}

Interval LPSolver::neumaier_shcherbina_postprocessing_var (int var, LPSolver::Sense sense) {
	try {
		// the dual solution : used to compute the bound
		Vector dual(nb_rows);
		get_dual_sol(dual);

		Matrix A_trans (nb_vars,nb_rows) ;
		get_rows_trans(A_trans);

		IntervalVector B(nb_rows);
		get_lhs_rhs(B);

		//cout <<" BOUND_test "<< endl;
		IntervalVector Rest(nb_vars);
		IntervalVector Lambda(dual);

		Rest = A_trans * Lambda ;   // Rest = Transpose(As) * Lambda
		if (sense==LPSolver::MINIMIZE) {
			Rest[var] -=1; // because C is a vector of zero except for the coef "var"
			return (Lambda * B - Rest * boundvar);
		} else {
			Rest[var] +=1;
			return -(Lambda * B - Rest * boundvar);
		}

		//cout << " Rest " << Rest << endl;
		//cout << " dual " << Lambda << endl;
		//cout << " dual B " << Lambda * B << endl;
		//cout << " rest box " << Rest * box  << endl;


	} catch (...) {
		throw LPException();
	}
}


Interval LPSolver::neumaier_shcherbina_postprocessing() {
	try {
		// the dual solution : used to compute the bound
		Vector dual(nb_rows);
		get_dual_sol(dual);

		Matrix A_trans (nb_vars,nb_rows) ;
		get_rows_trans(A_trans);

		IntervalVector B(nb_rows);
		get_lhs_rhs(B);

		Vector obj(nb_vars);
		get_coef_obj(obj);

		//cout <<" BOUND_test "<< endl;
		IntervalVector Rest(nb_vars);
		IntervalVector Lambda(dual);

		Rest = A_trans * Lambda ;   // Rest = Transpose(As) * Lambda
		Rest -= obj; // TODO
		return (Lambda * B - Rest * boundvar);



	} catch (...) {
		throw LPException();
	}
}

bool LPSolver::neumaier_shcherbina_infeasibilitytest() {
	try {
		Vector infeasible_dir(nb_rows);
		get_infeasible_dir(infeasible_dir);

		Matrix A_trans (nb_vars,nb_rows) ;
		get_rows_trans(A_trans);

		IntervalVector B(nb_rows);
		get_lhs_rhs(B);

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



LPSolver::Status_Sol LPSolver::solve_proved() {
	LPSolver::Status_Sol stat = solve();
	switch (stat) {
	case LPSolver::OPTIMAL : {
		// Neumaier - Shcherbina postprocessing
		obj_value = neumaier_shcherbina_postprocessing();
		stat = LPSolver::OPTIMAL_PROVED;
		break;
	}
	case  LPSolver::INFEASIBLE: {
		// infeasibility test  cf Neumaier Shcherbina paper
		if (neumaier_shcherbina_infeasibilitytest()) {
			stat = LPSolver::INFEASIBLE_PROVED;
		}
		break;
	}
	default:
		stat = LPSolver::UNKNOWN;
		break;
	}
	return stat;

}


void LPSolver::add_constraint(const Matrix & A, CmpOp sign, const Vector& rhs ) {
	for (int i=0; i<A.nb_rows(); i++) {
		try {
			add_constraint(A[i],sign,rhs[i]);
		} catch (LPException&) { }
	}
}

#include "ibex_LPWrapper.cpp"
}
 // end namespace ibex
