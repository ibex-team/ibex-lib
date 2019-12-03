#include "ibex_LPSolver.h"

namespace ibex {

using namespace soplex;

LPSolver::LPSolver(int nb_vars1, int max_iter, double max_time_out, double eps) :
			nb_vars(nb_vars1), nb_rows(0), boundvar(nb_vars1) , sense(LPSolver::MINIMIZE),
			obj_value(0.0), primal_solution(nb_vars1), dual_solution(1 /*tmp*/),
			status_prim(false), status_dual(false){

	mysoplex= new soplex::SoPlex();
	mysoplex->setIntParam(SoPlex::VERBOSITY, SoPlex::VERBOSITY_ERROR);

	mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
	mysoplex->setIntParam(SoPlex::ITERLIMIT, max_iter);
	mysoplex->setRealParam(SoPlex::TIMELIMIT, max_time_out);
	mysoplex->setRealParam(SoPlex::FEASTOL, eps);
	mysoplex->setRealParam(SoPlex::OPTTOL, eps);
	mysoplex->setIntParam(SoPlex::SOLVEMODE, SoPlex::SOLVEMODE_REAL);

	// initialize the number of variables of the LP
	soplex::DSVectorReal dummy(0);
	for (int j=0; j<nb_vars; j++){
		mysoplex->addColReal(soplex::LPColReal(0.0, dummy, soplex::infinity, -soplex::infinity ));
	}

	// initialize the constraint of the bound of the variable
	soplex::DSVectorReal row1(nb_vars);
	for (int j=0; j<nb_vars; j++){
		row1.add (j,1.0);
		mysoplex->addRowReal(soplex::LPRowReal(-soplex::infinity, row1, soplex::infinity));
		row1.clear();
	}

	nb_rows += nb_vars;

}

LPSolver::~LPSolver() {
	delete mysoplex;
}

LPSolver::LPSolverStatus LPSolver::solve() {
	obj_value = Interval::all_reals();


	SPxSolver::Status stat = SPxSolver::UNKNOWN;

	try{
		status_prim = false;
		status_dual = false;
		stat = mysoplex->solve();
		switch (stat) {
		case SPxSolver::OPTIMAL : {
			obj_value = mysoplex->objValueReal();

			// the primal solution : used by choose_next_variable
			DVectorReal primal(nb_vars);
			mysoplex->getPrimalReal(primal);
			status_prim = true;
			for (int i=0; i< nb_vars ; i++) {
				primal_solution[i]=primal[i];
			}
			// the dual solution ; used by Neumaier Shcherbina test
			DVectorReal dual(nb_rows);

			dual_solution.resize(nb_rows);

			mysoplex->getDualReal(dual);
			status_dual = true;
			for (int i=0; i<nb_rows; i++) {
				if 	( ((mysoplex->rhsReal(i) >=  default_max_bound) && (dual[i]<=0)) ||
						((mysoplex->lhsReal(i) <= -default_max_bound) && (dual[i]>=0))   ) {
					dual_solution[i]=0;
				}
				else {
					dual_solution[i]=dual[i];
				}
			}
			return OPTIMAL;
		}
	    case SPxSolver::ABORT_TIME: {
	    	return TIME_OUT;
	    }
	    case SPxSolver::ABORT_ITER:{
	    	return MAX_ITER;
	    }
	    case SPxSolver::INFEASIBLE:{
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
	mysoplex->writeFileReal(name, NULL, NULL, NULL);
	return ;
}

ibex::Vector LPSolver::get_coef_obj() const {
	ibex::Vector obj(nb_vars);
	DVectorReal newobj(nb_vars);
	mysoplex->getObjReal(newobj);
	for (int j=0;j<nb_vars; j++){
		obj[j] = newobj[j];
	}
	return obj;
}

ibex::Matrix LPSolver::get_rows() const {
	ibex::Matrix A(nb_rows, nb_vars);
	DSVectorReal row;
	for (int i=0;i<nb_rows; i++){
		mysoplex->getRowVectorReal(i, row);
		for (int j=0;j<nb_vars; j++){
			A.row(i)[j] = row[j];
		}
	}
	return A;
}


ibex::Matrix LPSolver::get_rows_trans() const {
	ibex::Matrix A_trans(nb_vars,nb_rows);
	DSVectorReal row;
	for (int i=0;i<nb_rows; i++){
		mysoplex->getRowVectorReal(i, row);
		for (int j=0;j<nb_vars; j++){
			A_trans.row(j)[i] = row[j];
		}
	}
	return A_trans;
}

IntervalVector  LPSolver::get_lhs_rhs() const{
	IntervalVector B(nb_rows);
	for (int i=0;i<nb_rows; i++){
		B[i]=Interval( mysoplex->lhsReal(i) , mysoplex->rhsReal(i) );
	}
	return B;
}

ibex::Vector LPSolver::get_infeasible_dir() const {
	DVector sol_found(nb_rows);
	bool found = mysoplex->getDualFarkasReal(sol_found);
	if(!found) {
		throw LPException();
	}
	ibex::Vector sol(nb_rows);
	for(int i=0; i<nb_rows; i++) {
		sol[i]=sol_found[i];
	}
	return sol;
}

double LPSolver::get_epsilon() const {
	return mysoplex->realParam(SoPlex::FEASTOL);
}

void LPSolver::clear_ctrs() {
	status_prim = false;
	status_dual = false;
	if (nb_rows > nb_vars)  {
		mysoplex->removeRowRangeReal(nb_vars, nb_rows-1);
	}
	nb_rows = nb_vars;
	obj_value = POS_INFINITY;
}


void LPSolver::set_max_iter(int max) {
	mysoplex->setIntParam(SoPlex::ITERLIMIT, max);
}

void LPSolver::set_max_time_out(double time) {
	double t = time;
	mysoplex->setRealParam(SoPlex::TIMELIMIT, t);
}

void LPSolver::set_sense(Sense s) {
	if (s == LPSolver::MINIMIZE) {
		mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MINIMIZE);
		sense = LPSolver::MINIMIZE;
	}
	else if (s == LPSolver::MAXIMIZE) {
		mysoplex->setIntParam(SoPlex::OBJSENSE, SoPlex::OBJSENSE_MAXIMIZE);
		sense = LPSolver::MAXIMIZE;
	}
	else {
		throw LPException();
	}
}

void LPSolver::set_obj(const ibex::Vector& coef) {
	for (int i = 0; i < nb_vars; i++) {
		mysoplex->changeObjReal(i, coef[i]);
	}
}

void LPSolver::set_obj_var(int var, double coef) {
	mysoplex->changeObjReal(var, coef);
}

void LPSolver::set_bounds(const IntervalVector& bounds) {
	for (int j=0; j<nb_vars; j++){
		// Change the LHS and RHS of each constraint associated to the bounds of the variable
		mysoplex->changeRangeReal(j, bounds[j].lb(), bounds[j].ub());
		mysoplex->changeBoundsReal(j ,bounds[j].lb(),bounds[j].ub());
	}
	boundvar = bounds;
}

void LPSolver::set_bounds_var(int var, const Interval& bound) {
	mysoplex->changeRangeReal(var, bound.lb(), bound.ub());
	mysoplex->changeBoundsReal(var ,bound.lb(),bound.ub());
	boundvar[var] = bound;
}

void LPSolver::set_epsilon(double eps) {
	mysoplex->setRealParam(SoPlex::FEASTOL, eps);
	mysoplex->setRealParam(SoPlex::OPTTOL, eps);
}

void LPSolver::add_constraint(const ibex::Vector& row, CmpOp sign, double rhs) {
	DSVectorReal row1(nb_vars);
	for (int i = 0; i < nb_vars ; i++) {
		row1.add(i, row[i]);
	}
	if (sign == LEQ || sign == LT) {
		mysoplex->addRowReal(LPRowReal(-soplex::infinity, row1, rhs));
		nb_rows++;
	}
	else if (sign == GEQ || sign == GT) {
		mysoplex->addRowReal(LPRowReal(rhs, row1, soplex::infinity));
		nb_rows++;
	} else if(sign == EQ) {
		mysoplex->addRowReal(LPRowReal(rhs, row1, rhs));
		nb_rows++;
	}
	else {
		throw LPException();
	}
}

/*int LPSolver::add_column(double obj, ibex::Vector& col, const Interval& bounds) {
	soplex::DSVectorReal soplex_col(nb_rows());
	for(int i = 0; i < col.size(); ++i) {
		soplex_col.add(i, col[i]);
	}
	mysoplex->addColReal(LPColReal(obj, soplex_col, bounds.lb(), bounds.ub()));
	return mysoplex->numColsReal()-1;
}*/

} /* end namespace ibex */
