//============================================================================
//                                  I B E X
// Linear Relaxation Contractor
// File        : ibex_CtcLinearRelaxation.cpp
// Author      : Bertrand Neveu , Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 14, 2012
// Last Update : Jul 02, 2013 (Gilles Chabert)
//============================================================================

#include "ibex_CtcLinearRelaxationIter.h"
#include "ibex_ExtendedSystem.h"

using namespace std;

namespace ibex {

CtcLinearRelaxationIter::CtcLinearRelaxationIter(const System& sys,
		ctc_mode cmode, int max_iter, int time_out, double eps, double max_diam_box1, bool init_lp):
								 Ctc(sys.nb_var), sys(sys), goal_var(-1), cmode(cmode), max_diam_box(max_diam_box1) {

	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		(int&) goal_var=((const ExtendedSystem&) sys).goal_var();
	}

	mylinearsolver = NULL;
	if (init_lp) mylinearsolver = new LinearSolver(nb_var, sys.nb_ctr, max_iter, time_out, eps);
}

CtcLinearRelaxationIter::~CtcLinearRelaxationIter () {
	if (mylinearsolver!=NULL) delete mylinearsolver;
}

int CtcLinearRelaxationIter::linearization(IntervalVector& box) {
	return linearization(box, mylinearsolver);
}

void CtcLinearRelaxationIter::contract (IntervalVector & box){
	if (box.max_diam() > max_diam_box) return; // is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
	//		cout << " box before XNewtonIter " << box << endl;
	try {

		// Update the bounds the variables
		mylinearsolver->initBoundVar(box);

		//returns the number of constraints in the linearized system
		int cont = linearization(box);

		if(cont<1)  return;
		optimizer(box);

		//	mylinearsolver->writeFile("LP.lp");
		//		system ("cat dump.lp");
		//cout << " box after  LR " << box << endl;
		mylinearsolver->cleanConst();

	}
	catch(EmptyBoxException&) {
		box.set_empty(); // empty the box before exiting in case of EmptyBoxException
		mylinearsolver->cleanConst();
		throw EmptyBoxException();
	}

}


void CtcLinearRelaxationIter::optimizer(IntervalVector& box) {

	Interval opt(0.0);
	int* inf_bound = new int[nb_var]; // indicator inf_bound = 1 means the inf bound is feasible or already contracted, call to simplex useless (cf Baharev)
	int* sup_bound = new int[nb_var]; // indicator sup_bound = 1 means the sup bound is feasible or already contracted, call to simplex useless

	if (cmode==ONLY_Y) {
		for (int i=0; i<nb_var; i++) {
			// in the case of lower_bounding, only the left bound of y is contracted
			inf_bound[i]=(i==goal_var?0:1);
			sup_bound[i]=1;
		}
	}
	else {
		for (int i=0; i<nb_var; i++) {
			// in case of optimization, the right bound of y is not contracted
			inf_bound[i]=0;
			sup_bound[i]=(i==goal_var?1:0);
		}
	}

	int nexti=-1;   // the next variable to be contracted
	int infnexti=0; // the bound to be contracted contract  infnexti=0 for the lower bound, infnexti=1 for the upper bound
	LinearSolver::Status_Sol stat=LinearSolver::UNKNOWN;

	for(int ii=0; ii<(2*nb_var); ii++) {  // at most 2*n calls
		int i= ii/2;
		if (nexti != -1) i=nexti;
		//		cout << " i "<< i << " infnexti " << infnexti << " infbound " << inf_bound[i] << " supbound " << sup_bound[i] << endl;
		//			cout << " box avant simplex " << box << endl;
		if (infnexti==0 && inf_bound[i]==0)  // computing the left bound : minimizing x_i
		{
			inf_bound[i]=1;
			stat = run_simplex(box, LinearSolver::MINIMIZE, i, opt,box[i].lb());
			//			cout << " stat " << stat <<  " opt " << opt << endl;
			if (stat == LinearSolver::OPTIMAL) {
				if(opt.lb()>box[i].ub())
					throw EmptyBoxException();

				if(opt.lb() > box[i].lb()) {
					box[i]=Interval(opt.lb(),box[i].ub());
					mylinearsolver->setBoundVar(i,box[i]);
				}

				if (!choose_next_variable(box,nexti,infnexti, inf_bound, sup_bound)) {
					break;
				}
			}
			else if (stat == LinearSolver::INFEASIBLE) {
				// the infeasibility is proved, the EmptyBox exception is raised
				throw EmptyBoxException();
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
			stat= run_simplex(box, LinearSolver::MAXIMIZE, i, opt, box[i].ub());
			//			cout << " stat " << stat <<  " opt " << opt << endl;
			if( stat == LinearSolver::OPTIMAL) {
				if(opt.ub() <box[i].lb())
					throw EmptyBoxException();

				if (opt.ub() < box[i].ub()) {
					box[i] =Interval( box[i].lb(), opt.ub());
					mylinearsolver->setBoundVar(i,box[i]);
				}

				if (!choose_next_variable(box,nexti,infnexti, inf_bound, sup_bound)) {
					break;
				}
			}
			else if(stat == LinearSolver::INFEASIBLE) {
				// the infeasibility is proved,  the EmptyBox exception is raised
				throw EmptyBoxException();
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
	}

	delete[] inf_bound;
	delete[] sup_bound;
}


LinearSolver::Status_Sol CtcLinearRelaxationIter::run_simplex(IntervalVector& box,
		LinearSolver::Sense sense, int var, Interval& obj, double bound) {
	int nvar=nb_var;
	int nctr=mylinearsolver->getNbRows();
	if(sense==LinearSolver::MINIMIZE)
		mylinearsolver->setVarObj(var, 1.0);
	else
		mylinearsolver->setVarObj(var, -1.0);

	//mylinearsolver->setSense(LinearSolver::MINIMIZE);
	//	mylinearsolver->writeFile("dump.lp");
	//	system("cat dump.lp");
	LinearSolver::Status_Sol stat = mylinearsolver->solve();
	//	cout << " stat solver " << stat << endl;

	if(stat == LinearSolver::OPTIMAL) {
		if( ((sense==LinearSolver::MINIMIZE) && (  mylinearsolver->getObjValue() <=bound)) ||
				((sense==LinearSolver::MAXIMIZE) && ((-mylinearsolver->getObjValue())>=bound))) {
			stat = LinearSolver::UNKNOWN;
		}
	}

	// Neumaier - Shcherbina postprocessing
	if(stat == LinearSolver::OPTIMAL) {

		// the dual solution : used to compute the bound
		Vector dual_solution(mylinearsolver->getNbRows());
		LinearSolver::Status stat_dual = mylinearsolver->getDualSol(dual_solution);

		Matrix A_trans (nb_var,mylinearsolver->getNbRows()) ;
		LinearSolver::Status stat_A = mylinearsolver->getCoefConstraint_trans(A_trans);

		/*	IntervalMatrix IA_trans (nb_var,mylinearsolver->getNbRows());
		for (int i=0;i<nvar; i++){
		  for(int j=0; j<nctr; j++)
		    IA_trans[i][j]= A_trans[i][j];
		}*/
		IntervalVector B(mylinearsolver->getNbRows());
		LinearSolver::Status stat_B = mylinearsolver->getB(B);

		bool minimization=false;
		if (sense==LinearSolver::MINIMIZE) 
			minimization=true;
		//	  cout << "B " << B << endl;
		//	  cout << "A_trans " << IA_trans << endl;

		if ((stat_dual==LinearSolver::OK) && (stat_A==LinearSolver::OK) && (stat_B==LinearSolver::OK))
			NeumaierShcherbina_postprocessing( mylinearsolver->getNbRows(), var, obj, box, A_trans, B, dual_solution, minimization);
		else
			stat = LinearSolver::UNKNOWN;

	}

	// infeasibility test  cf Neumaier Shcherbina paper
	if(stat == LinearSolver::INFEASIBLE_NOTPROVED) {

		Vector infeasible_dir(mylinearsolver->getNbRows());
		LinearSolver::Status stat1 = mylinearsolver->getInfeasibleDir(infeasible_dir);

		Matrix A_trans (nb_var,mylinearsolver->getNbRows()) ;
		LinearSolver::Status stat2 = mylinearsolver->getCoefConstraint_trans(A_trans);

		/*IntervalMatrix IA_trans (nb_var,mylinearsolver->getNbRows());
		for (int i=0;i<nvar; i++){
		  for(int j=0; j<nctr; j++)
		    IA_trans[i][j]= A_trans[i][j];
		}*/
		IntervalVector B(mylinearsolver->getNbRows());
		LinearSolver::Status stat3 = mylinearsolver->getB(B);

		if ((stat1==LinearSolver::OK) && (stat2==LinearSolver::OK) && (stat3==LinearSolver::OK) &&
				(NeumaierShcherbina_infeasibilitytest (mylinearsolver->getNbRows(), box, A_trans, B, infeasible_dir)))
			stat = LinearSolver::INFEASIBLE;

	}

	// Reset the objective of the LP solver
	mylinearsolver->setVarObj(var, 0.0);

	return stat;

}

void CtcLinearRelaxationIter::NeumaierShcherbina_postprocessing ( int nr, int var, Interval & obj, IntervalVector& box,
		Matrix & A_trans, IntervalVector& B, Vector & dual_solution, bool minimization) {

	//std::cout <<" BOUND_test "<<std::endl;
	IntervalVector Rest(nb_var);

	IntervalVector Lambda(nr);

	for (int i=0; i<nr; i++) {
		if( (B[i].ub()==LinearSolver::default_max_bound && dual_solution[i]<0) ||
		    (B[i].lb() ==-LinearSolver::default_max_bound && dual_solution[i]>0   )) //Modified by IA
		Lambda[i]=0;
	else
		Lambda[i]=dual_solution[i];
	}

	Rest = A_trans * Lambda ;   // Rest = Transpose(As) * Lambda
	if (minimization==true)
		Rest[var] -=1; // because C is a vector of zero except for the coef "var"
	else
		Rest[var] +=1;

	//	cout << " Rest " << Rest << endl;
	//	cout << " dual " << Lambda << endl;
	//	cout << " dual B " << Lambda * B << endl;
	//	cout << " rest box " << Rest * box  << endl;
	if(minimization==true) 
		obj = Lambda * B - Rest * box;
	else
		obj = -(Lambda * B - Rest * box);
}

bool CtcLinearRelaxationIter::NeumaierShcherbina_infeasibilitytest(int nr, IntervalVector& box,
		Matrix& A_trans, IntervalVector& B, Vector& infeasible_dir) {

	IntervalVector Lambda(nr);

	for (int i =0; i<nr; i++) {
		if( (B[i].ub()==LinearSolver::default_max_bound  && infeasible_dir[i]<0 ) ||
			(B[i].lb() ==-LinearSolver::default_max_bound  && infeasible_dir[i]>0 ))
			// blind copy from OPTIMAL case useful in this case ?? BN
		Lambda[i]=0;
	else
		Lambda[i]=infeasible_dir[i];
	}

	IntervalVector Rest(nb_var);
	Rest = A_trans * infeasible_dir ;
	Interval d= Rest *box - Lambda * B;

	// if 0 does not belong to d, the infeasibility is proved

	if (d.contains(0.0))
		return false;
	else
		return true;

}

bool CtcLinearRelaxationIter::isInner(IntervalVector & box, const System& sys, int j) {
	Interval eval=sys.ctrs[j].f.eval(box);

	if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
			(sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
		return false;
	else
		return true;
}

bool CtcLinearRelaxationIter::choose_next_variable(IntervalVector & box,
		int & nexti, int & infnexti, int* inf_bound, int* sup_bound) {

	bool found = false;

	// the primal solution : used by choose_next_variable
	Vector primal_solution(nb_var);
	LinearSolver::Status stat_prim = mylinearsolver->getPrimalSol(primal_solution);
	//	cout << " primal " << primal_solution << endl;
	if (stat_prim==LinearSolver::OK) {
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
	}
	else {
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

}// end namespace ibex

