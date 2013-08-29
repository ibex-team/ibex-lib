//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : December 24, 2012
//============================================================================

#include "ibex_Optimizer.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Timer.h"
#include "ibex_OptimProbing.cpp_"
#include "ibex_OptimSimplex.cpp_"
#include "ibex_CtcFwdBwd.h"
#include "ibex_ExprCopy.h"
#include "ibex_Function.h"
#include "ibex_NoBisectableVariableException.h"
//#include "ibex_Multipliers.h"

#include <float.h>

using namespace std;

namespace ibex {

const double Optimizer::default_prec = 1e-07;
const double Optimizer::default_goal_rel_prec = 1e-07;
const double Optimizer::default_goal_abs_prec = 1e-07;
const int Optimizer::default_sample_size = 10;


void Optimizer::write_ext_box(const IntervalVector& box, IntervalVector& ext_box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==ext_sys.goal_var()) i2++; // skip goal variable
		ext_box[i2]=box[i];
	}
}

void Optimizer::read_ext_box(const IntervalVector& ext_box, IntervalVector& box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==ext_sys.goal_var()) i2++; // skip goal variable
		box[i]=ext_box[i2];
	}
}

Optimizer::Optimizer(System& user_sys, Bsc& bsc, Ctc& ctc, double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size) :
		n(user_sys.nb_var), m(user_sys.nb_ctr),
		sys(user_sys,System::NORMALIZE), ext_sys(user_sys),
		bsc(bsc), ctc(ctc), buffer(n),
		prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
		sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
		timeout(1e08), loup(POS_INFINITY), uplo(NEG_INFINITY), loup_point(n),
		df(*user_sys.goal,Function::DIFF), 
		uplo_of_epsboxes(POS_INFINITY), nb_cells(0) {

	// ====== build the reversed inequalities g_i(x)>0 ===============
	if(m>0) {
		Array<Ctc> ng(m);
		for (int i=0; i<m; i++) {
			ng.set_ref(i, *new CtcFwdBwd(sys.f[i],GT));
		}
		is_inside=new CtcUnion(ng);
	}
	else
		is_inside=NULL;
	// =============================================================

	if (trace) cout.precision(12);

	nb_simplex=0;
	diam_simplex=0;
	nb_rand=0;
	diam_rand=0;
	int niter=100;
	if (niter < 3*n) niter=3*n;
	//====================================
	mylp = new LinearSolver(n+1,m,niter );
}

Optimizer::~Optimizer() {
	if (is_inside) {
		for (int i=0; i<m; i++) {
			delete &(is_inside->list[i]);
		}

		delete is_inside;
	}
	buffer.flush();

	delete mylp;
}

bool Optimizer::update_loup(const IntervalVector& box) {
	//  if (trace) cout << "box update loup " << box << endl;
	bool box_loup_changed = update_loup_probing (box);
	box_loup_changed |= update_loup_simplex(box);
	return box_loup_changed;
}

void Optimizer::update_entailed_ctr(const IntervalVector& box) {
	for (int j=0; j<m; j++) {
		if ((*entailed)[j]) {
			continue;
		}
		Interval y=sys.f[j].eval(box);
		if (y.lb()>0) throw EmptyBoxException();
		else if (y.ub()<=0) {
			(*entailed)[j]=true;
		}
	}
}

  bool Optimizer::contract_and_bound(Cell& c, const IntervalVector& init_box) {
	//         cout << "box " <<c.box << endl;
	/*======================== contract y with y<=loup ========================*/
	Interval& y=c.box[ext_sys.goal_var()];

	//cout << "loup=" << loup << endl;

	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax=loup - goal_rel_prec*fabs(loup);

	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;

	//cout << "ymax=" << ymax << endl;

	if (y.lb() > ymax) {
		c.box.set_empty();
		throw EmptyBoxException();
	}
	if (y.ub() > ymax) y = Interval(y.lb(),ymax);

	/*================ contract x with f(x)=y and g(x)<=0 ================*/
	//	cout << "y=f(x) and g(x)<=0 " << endl;
	//	cout << "   x before=" << c.box << endl;
	//	cout << "   y before=" << y << endl;

	ctc.contract(c.box); // may throw EmptyBoxException

	//	cout << "   x after=" << c.box << endl;
	//	cout << "   y after=" << y << endl;
	// TODO: no more cell in argument here (just a box). Does it matter?
	/*====================================================================*/

	// there is still something left to be contracted in the box
	/*========================= update loup ==============================*/
	IntervalVector tmp_box(n);
	read_ext_box(c.box,tmp_box);

	entailed = &c.get<EntailedCtr>();
	update_entailed_ctr(tmp_box);

	bool loup_changed = update_loup(tmp_box);
	/*====================================================================*/
	if (tmp_box.max_diam()<=prec || !c.box.is_bisectable()) {
		// rem1: tmp_box and not c.box because y is handled with goal_prec_rec and goal_abs_prec
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)
		// rem 3 : the extended  boxes with no bisectable  domains  should be catched for avoiding infinite bisections

		// the box is a "solution"
		// uplo of epsboxes can only go down, but not under uplo : it is an upperbound for uplo, that indicates a lowerbound for the objective in all the small boxes
		// found by the precision criterion
		//	  cout << " small box " << tmp_box <<  "  " << c.box <<  endl;
		if (uplo_of_epsboxes > y.lb() && uplo_of_epsboxes > uplo) {
			if (y.lb() > uplo)
				uplo_of_epsboxes = y.lb();
			else
				uplo_of_epsboxes = uplo;
			if (trace) {
				cout << "uplo_of_epsboxes:" << setprecision(12) <<  uplo_of_epsboxes << " uplo " << uplo << endl;
			}

		}
		throw EmptyBoxException();
	}
	//  gradient test for unconstrained optimization 
	if (m==0 
			//	    || is_inner(tmp_box)   does not improve the solving in constrained optimization
	) {
		if (tmp_box.is_strict_subset(init_box)) {
			// may throw an EmptyBoxException:
			if (n==1)
				df.backward(Interval::ZERO,tmp_box);
			else
				df.backward(IntervalVector(n,Interval::ZERO),tmp_box);
			write_ext_box(tmp_box,c.box);
		}
	}

	return loup_changed;
}




void Optimizer::optimize(const IntervalVector& init_box) {

	buffer.flush();

	Cell* root=new Cell(IntervalVector(n+1));

	write_ext_box(init_box,root->box);

	// add data required by the contractor
	//ctc.init_root(*root); // we know there is none (not incremental HC4).

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	// add data required by optimizer + Fritz John contractor
	root->add<EntailedCtr>();
	//root->add<Multipliers>();
	entailed=&root->get<EntailedCtr>();
	entailed->init_root(m);
	//root->get<Multipliers>().init_root(m);

	bool loup_changed=0;
	time=0;
	Timer::start();
	try {
	  loup_changed=contract_and_bound(*root, init_box);
	}
	catch(EmptyBoxException&) {
		delete root;
		uplo=uplo_of_epsboxes;
		return;
	}

	buffer.push(root);
	if (((CellHeap&) buffer).minimum() > uplo)
		uplo= ((CellHeap&) buffer ).minimum() ;
	if (trace) cout  << " uplo= " << uplo << endl;


	try {
		while (!buffer.empty()) {

			if (trace >= 2) cout << ((CellBuffer&) buffer) << endl;

			Cell* c=buffer.top();

			//	    cout << " box before bisection " <<  c->box << endl;

			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);


				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);


				delete buffer.pop();
				loup_changed=0;

				try{
				        loup_changed = contract_and_bound(*new_cells.first, init_box);
					buffer.push(new_cells.first);
					nb_cells++;
				}
				catch(EmptyBoxException&) {
					delete new_cells.first;
				}
				try {
				        loup_changed |= contract_and_bound(*new_cells.second, init_box);
					buffer.push(new_cells.second);
					nb_cells++;
				}
				catch(EmptyBoxException&) {
					delete new_cells.second;
				}

				if (loup_changed) {
					// In case of a new upper bound (loup_changed == true), all the boxes
					// with a lower bound greater than (loup - goal_prec) are removed and deleted.
					double ymax=loup - goal_rel_prec*fabs(loup);
					if (loup - goal_abs_prec < ymax)
						ymax = loup - goal_abs_prec;
					((CellHeap&) buffer ).contract_heap(ymax);
					if (ymax <=-DBL_MAX) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) cout << setprecision(12) << "ymax=" << ymax << " uplo= " <<  uplo << endl;
				}


				double new_uplo=POS_INFINITY;

				if (! buffer.empty())
					new_uplo= ((CellHeap&) buffer).minimum();
				else if (buffer.empty() && loup != POS_INFINITY) {
					// empty heap : uplo is set to loup - precision if a loup has been found
					if (fabs (loup) < 1)
						new_uplo=loup-goal_abs_prec;
					else
						new_uplo=loup - fabs(loup)* goal_rel_prec;
					//		    cout << " new uplo buffer empty " << new_uplo << " uplo " << uplo << endl;
				}

				if (new_uplo < uplo_of_epsboxes && new_uplo > uplo) {
					// cout << " new uplo " << new_uplo << " uplo " << uplo << endl;
					uplo=new_uplo;
				}
				else if (new_uplo >=   uplo_of_epsboxes && uplo_of_epsboxes != POS_INFINITY) {
					uplo=uplo_of_epsboxes;
					//cout << " uplo " << uplo << endl;
				}

				// Note: if contraction was before bisection, we could have the problem
				// that the current cell is removed by contract_heap. See comments in
				// older version of the code (before revision 284).
				time_limit_check();

			}
			catch (NoBisectableVariableException& ) {
				delete buffer.pop();
			}
		}
	}
	catch (TimeOutException& ) {
	  return;
	}
	

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();
}



void Optimizer::report() {


	if (timeout >0 &&  time >=timeout ) {
		cout << "time limit " << timeout << "s. reached " << endl;
	}
	// No solution found and optimization stopped with empy buffer  before the required precision is reached => means infeasible problem
	if (buffer.empty() && uplo_of_epsboxes == POS_INFINITY && loup==POS_INFINITY) {
		cout << " infeasible problem " << endl;
		cout << " cpu time used " << time << "s." << endl;
		cout << " number of cells " << nb_cells << endl;
	}

	else {
		cout << " best bound in: [" << uplo << "," << loup << "]" << endl;

		double rel_prec;

		if (loup==POS_INFINITY)
			rel_prec= POS_INFINITY;
		else
			rel_prec=(loup-uplo)/(fabs (loup))-1.e-15;

		double abs_prec=loup-uplo-1.e-15;

		cout << " Relative precision obtained on objective function: " << rel_prec << " " <<
				(rel_prec <= goal_rel_prec? " [passed]" : " [failed]") << "  " << goal_rel_prec <<  endl;

		cout << " Absolute precision obtained on objective function: " << abs_prec << " " <<
				(abs_prec <= goal_abs_prec? " [passed]" : " [failed]") << "  " << goal_abs_prec << endl;
		if (uplo_of_epsboxes != NEG_INFINITY && uplo_of_epsboxes != POS_INFINITY)
			cout << " precision on variable domains obtained " << prec << " "   << " uplo_of_epsboxes " << uplo_of_epsboxes << endl;
		else if (uplo_of_epsboxes == NEG_INFINITY)
			cout << " small boxes with negative infinity objective :  objective not bound " << endl;
		if (loup==POS_INFINITY)
			cout << " no feasible point found " << endl;
		else
			cout << " best feasible point " << loup_point << endl;


		cout << " cpu time used " << time << "s." << endl;
		cout << " number of cells " << nb_cells << endl;
	}
	/*   // statistics on upper bounding
    if (trace) {
      cout << " nbrand " << nb_rand << " nb_inhc4 " << nb_inhc4 << " nb simplex " << nb_simplex << endl;
      cout << " diam_rand " << diam_rand << " diam_inhc4 " << diam_inhc4 << " diam_simplex " << diam_simplex << endl;
    }
	*/
}

void Optimizer::report_perf() {

	double rel_prec;
	if (loup==POS_INFINITY)
		rel_prec= POS_INFINITY;
	else
		rel_prec=(loup-uplo)/(fabs(loup))-1.e-15;

	double abs_prec=loup-uplo-1.e-15;

	cout << (	((rel_prec <= goal_rel_prec)||
				(abs_prec <= goal_abs_prec)||
				((buffer.empty() && uplo_of_epsboxes == POS_INFINITY && loup==POS_INFINITY))||
				(uplo<-1.e300)
			)? " T & " : " F & " );

	cout << uplo << " & " << loup << " & ";
	cout <<  time << "  "<< endl ;


}

void Optimizer::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (timeout >0 &&  time >=timeout ) throw TimeOutException();
	Timer::start();
}

} // end namespace ibex
