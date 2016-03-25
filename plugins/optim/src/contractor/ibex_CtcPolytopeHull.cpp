//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPolytopeHull.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2013
// Last Update : Oct 31, 2013
//============================================================================

#include "ibex_CtcPolytopeHull.h"
#include "ibex_LinearRelaxFixed.h"

using namespace std;

namespace ibex {

namespace {
class PolytopeHullEmptyBoxException { };
}

CtcPolytopeHull::CtcPolytopeHull(LinearRelax& lr, ctc_mode cmode, int max_iter, int time_out, double eps, Interval limit_diam) :
		Ctc(lr.nb_var()), lr(lr), goal_var(lr.goal_var()), cmode(cmode),
		limit_diam_box(eps>limit_diam.lb()? eps : limit_diam.lb(), limit_diam.ub()), own_lr(false) {

	 mylinearsolver = new LinearSolver(nb_var, lr.nb_ctr(), max_iter, time_out, eps);

}

CtcPolytopeHull::CtcPolytopeHull(const Matrix& A, const Vector& b, int max_iter, int time_out, double eps, Interval limit_diam) :
		Ctc(A.nb_cols()), lr(*new LinearRelaxFixed(A,b)), goal_var(lr.goal_var()), cmode(ALL_BOX),
		limit_diam_box(eps>limit_diam.lb()? eps : limit_diam.lb(), limit_diam.ub()), own_lr(true) {

	 mylinearsolver = new LinearSolver(nb_var, lr.nb_ctr(), max_iter, time_out, eps);

}

CtcPolytopeHull::~CtcPolytopeHull() {
	if (mylinearsolver!=NULL) delete mylinearsolver;
	if (own_lr) delete &lr;
}

#ifndef  _IBEX_WITH_NOLP_

void CtcPolytopeHull::contract(IntervalVector& box) {

	if (!(limit_diam_box.contains(box.max_diam()))) return;
	// is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
	//cout << "[polytope-hull] box before LR (linear relaxation): " << box << endl;

	try {
		// Update the bounds the variables
		mylinearsolver->initBoundVar(box);

		//returns the number of constraints in the linearized system
		int cont = lr.linearization(box, *mylinearsolver);

		//cout << "[polytope-hull] end of LR" << endl;

		if (cont==-1) throw PolytopeHullEmptyBoxException();

		if (cont==0) return;

		optimizer(box);

		//mylinearsolver->writeFile("LP.lp");
		//system ("cat LP.lp");
		//cout << "[polytope-hull] box after LR: " << box << endl;
		mylinearsolver->cleanConst();
	}
	catch(LPException&) {
		mylinearsolver->cleanConst();
	}
	catch(PolytopeHullEmptyBoxException& e) {
		box.set_empty(); // empty the box before exiting
		mylinearsolver->cleanConst();
	}

}

void CtcPolytopeHull::optimizer(IntervalVector& box) {

	Interval opt(0.0);
	int* inf_bound = new int[nb_var]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted, call to simplex useless (cf Baharev)
	int* sup_bound = new int[nb_var]; // indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

	if (cmode==ONLY_Y) {
		for (int i=0; i<nb_var; i++) {
			// in the case of lower_bounding, only the left bound of y is contracted
			inf_bound[i]=1;
			sup_bound[i]=1;
		}
		if (goal_var>-1) inf_bound[goal_var]=0;
	}
	else {
		for (int i=0; i<nb_var; i++) {
			// in case of optimization, the right bound of y is not contracted
			inf_bound[i]=0;
			sup_bound[i]=0;
		}
		if (goal_var>-1) sup_bound[goal_var]=1;
	}

	int nexti=-1;   // the next variable to be contracted
	int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound
	LinearSolver::Status_Sol stat=LinearSolver::UNKNOWN;

	for(int ii=0; ii<(2*nb_var); ii++) {  // at most 2*n calls

		int i= ii/2;
		if (nexti != -1) i=nexti;
		//cout << "[polytope-hull]->[optimize] var n°"<< i << " infnexti=" << infnexti << " infbound=" << inf_bound[i] << " supbound=" << sup_bound[i] << endl;
		//cout << "[polytope-hull]->[optimize] box before simplex: " << box << endl;
		if (infnexti==0 && inf_bound[i]==0)  // computing the left bound : minimizing x_i
		{
			inf_bound[i]=1;
			stat = mylinearsolver->run_simplex(box, LinearSolver::MINIMIZE, i, opt,box[i].lb());
			//cout << "[polytope-hull]->[optimize] simplex for left bound returns stat:" << stat <<  " opt: " << opt << endl;
			if (stat == LinearSolver::OPTIMAL) {
				if(opt.lb()>box[i].ub()) {
					delete[] inf_bound;
					delete[] sup_bound;
					throw PolytopeHullEmptyBoxException();
				}

				if(opt.lb() > box[i].lb()) {
					box[i]=Interval(opt.lb(),box[i].ub());
					mylinearsolver->setBoundVar(i,box[i]);
				}

				if (!choose_next_variable(box,nexti,infnexti, inf_bound, sup_bound)) {
					break;
				}
			}
			else if (stat == LinearSolver::INFEASIBLE) {
				delete[] inf_bound;
				delete[] sup_bound;
				// the infeasibility is proved, the EmptyBox exception is raised
				throw PolytopeHullEmptyBoxException();
			}

			else if (stat == LinearSolver::INFEASIBLE_NOTPROVED) {
				// the infeasibility is found but not proved, no other call is needed
				break;
			}

			else if (stat == LinearSolver::UNKNOWN) {
				int next=-1;
				for (int j=0;j<nb_var;j++) {
					if (inf_bound[j]==0) {
						nexti=j;  next=0;  infnexti=0;
						break;
					}
					else if  (sup_bound[j]==0) {
						nexti=j;  next=0;  infnexti=1;
						break;
					}
				}
				if (next==-1)  break;
			}

		}
		else if (infnexti==1 && sup_bound[i]==0) { // computing the right bound :  maximizing x_i
			sup_bound[i]=1;
			stat= mylinearsolver->run_simplex(box, LinearSolver::MAXIMIZE, i, opt, box[i].ub());
			//cout << "[polytope-hull]->[optimize] simplex for right bound returns stat=" << stat << " opt=" << opt << endl;
			if( stat == LinearSolver::OPTIMAL) {
				if(opt.ub() <box[i].lb()) {
					delete[] inf_bound;
					delete[] sup_bound;
					throw PolytopeHullEmptyBoxException();
				}

				if (opt.ub() < box[i].ub()) {
					box[i] =Interval( box[i].lb(), opt.ub());
					mylinearsolver->setBoundVar(i,box[i]);
				}

				if (!choose_next_variable(box,nexti,infnexti, inf_bound, sup_bound)) {
					break;
				}
			}
			else if(stat == LinearSolver::INFEASIBLE) {
				delete[] inf_bound;
				delete[] sup_bound;
				// the infeasibility is proved,  the EmptyBox exception is raised
				throw PolytopeHullEmptyBoxException();
			}
			else if (stat == LinearSolver::INFEASIBLE_NOTPROVED) {
				// the infeasibility is found but not proved, no other call is needed
				break;
			}

			else if (stat == LinearSolver::UNKNOWN) {
				int next=-1;
				for (int j=0;j<nb_var;j++) {
					if (inf_bound[j]==0) {
						nexti=j;  next=0;  infnexti=0;
						break;
					}
					else if (sup_bound[j]==0) {
						nexti=j;  next=0;  infnexti=1;
						break;
					}
				}
				if (next==-1) break;
			}
		}
		else break; // in case of stat==MAX_ITER  we do not recall the simplex on a another variable  (for efficiency reason)
	}
	delete[] inf_bound;
	delete[] sup_bound;


}


bool CtcPolytopeHull::choose_next_variable(IntervalVector & box, int & nexti, int & infnexti, int* inf_bound, int* sup_bound) {

	bool found = false;

	try {
		// the primal solution : used by choose_next_variable
		Vector primal_solution(nb_var);
		mylinearsolver->getPrimalSol(primal_solution);
		//cout << " primal " << primal_solution << endl;

		// The Achterberg heuristic for choosing the next variable (nexti) and its bound (infnexti) to be contracted (cf Baharev paper)
		// and updating the indicators if a bound has been found feasible (with the precision prec_bound)
		// called only when a primal solution is found by the LP solver (use of primal_solution)

		// double prec_bound = mylinearsolver->getEpsilon(); // relative precision for the indicators TODO change with the precision of the optimizer ??
		double prec_bound = 1.e-8; // relative precision for the indicators      :  compatibility for testing  BNE
		double delta=1.e100;
		double deltaj=delta;

		for (int j=0; j<nb_var; j++)	{

			if (inf_bound[j]==0) {
				deltaj= fabs(primal_solution[j]- box[j].lb());
				if ((fabs (box[j].lb()) < 1 && deltaj < prec_bound) ||
						(fabs (box[j].lb()) >= 1 && fabs (deltaj /(box[j].lb())) < prec_bound))	{
					inf_bound[j]=1;
				}
				if (inf_bound[j]==0 && deltaj < delta) 	{
					nexti=j; infnexti=0;delta=deltaj; found =true;
				}
			}

			if (sup_bound[j]==0) {
				deltaj = fabs (primal_solution[j]- box[j].ub());


				if ((fabs (box[j].ub()) < 1 && deltaj < prec_bound) 	||
						(fabs (box[j].ub()) >= 1 && fabs (deltaj/(box[j].ub())) < prec_bound)) {
					sup_bound[j]=1;
				}
				if (sup_bound[j]==0 && deltaj < delta) {
					nexti=j; infnexti=1;delta=deltaj;  found =true;
				}

			}


		}
	} catch (LPException& ) {
		// Default if the primal solution is not available.
		for (int j=0; j<nb_var; j++) {
			if (inf_bound[j]==0) {
				nexti=j;   infnexti=0; found = true;
				break;
			}
			else if  (sup_bound[j]==0) {
				nexti=j;  infnexti=1; found = true;
				break;
			}
		}
	}
	return found;

}

#else /// end _IBEX_WITH_NOLP_


void CtcPolytopeHull::contract(IntervalVector& box) { }


#endif /// end _IBEX_WITH_NOLP_




} // end namespace ibex
