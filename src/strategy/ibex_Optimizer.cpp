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
#include "ibex_EmptySystemException.h"
#include "ibex_Timer.h"
#include "ibex_OptimProbing.cpp_"
#include "ibex_OptimSimplex.cpp_"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcOptimShaving.h"
#include "ibex_CtcHC4.h"

#include "ibex_ExprCopy.h"
#include "ibex_Function.h"
#include "ibex_NoBisectableVariableException.h"
//#include "ibex_Multipliers.h"
#include "ibex_PdcFirstOrder.h"

#include <float.h>
#include <stdlib.h>

using namespace std;

namespace ibex {

const double Optimizer::default_prec = 1e-07;
const double Optimizer::default_goal_rel_prec = 1e-07;
const double Optimizer::default_goal_abs_prec = 1e-07;
const int    Optimizer::default_sample_size = 10;
const double Optimizer::default_equ_eps = 1e-08;
const double Optimizer::default_loup_tolerance = 0.1;

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




Optimizer::Optimizer(System& user_sys, Ctc& ctc, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size, double equ_eps,
		bool rigor,  int critpr,CellHeapOptim::criterion crit) :
                				user_sys(user_sys), sys(user_sys,equ_eps),
                				n(user_sys.nb_var), m(sys.nb_ctr) /* (warning: not user_sys.nb_ctr) */,
                				ext_sys(user_sys,equ_eps),
                				ctc(ctc),bsc(bsc),
                				buffer(n),buffer2(n,crit),  // first buffer with LB, second buffer with ct (default UB))
                				prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
                				sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
                				critpr(critpr), timeout(1e08),
                				loup(POS_INFINITY), pseudo_loup(POS_INFINITY),uplo(NEG_INFINITY),
                				loup_point(n), loup_box(n), nb_cells(0),
                				df(*user_sys.goal,Function::DIFF), loup_changed(false),	rigor(rigor),
                				uplo_of_epsboxes(POS_INFINITY) {

	// ==== build the system of equalities only ====
	try {
		equs= new System(user_sys,System::EQ_ONLY);
	} catch(EmptySystemException&) {
		equs= NULL;
	}

	// ====== build the reversed inequalities g_i(x)>0 ===============
	if(m>0) {
		Array<Ctc> ng(m);
		for (int i=0; i<m; i++) {
			ng.set_ref(i, *new CtcFwdBwd(sys.ctrs[i].f,GT));
		}
		is_inside=new CtcUnion(ng);
	}
	else
		is_inside=NULL;
	// =============================================================

	if (trace) cout.precision(12);

	//	objshaver= new CtcOptimShaving (*new CtcHC4 (ext_sys.ctrs,0.1,true),20,1,1.e-11);

	int niter=100;
	// int niter=1000;
	if (niter < 3*n) niter=3*n;

	//====================================
	mylp = new LinearSolver(n+1,m,niter );
	//	cout << "sys " << sys << endl;
}

Optimizer::~Optimizer() {
	if (is_inside) {
		for (int i=0; i<m; i++) {
			delete &(is_inside->list[i]);
		}

		delete is_inside;
	}
	buffer.flush();
	if (critpr > 0) buffer2.flush();

	delete mylp;
	//	delete &(objshaver->ctc);
	//	delete objshaver;
}

// compute the value ymax (decreasing the loup with the precision)
// the heap and the current box are contracted with y <= ymax
double Optimizer::compute_ymax() {
	double ymax= loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;
	return ymax;}


// launch Hansen test
bool Optimizer::update_real_loup() {

	IntervalVector epsbox(loup_point);

	// ====================================================
	// solution #1: we inflate the loup-point and
	//              call Hansen test in contracting mode.
	// TODO: replace default_equ_eps by something else!
	//	epsbox.inflate(default_equ_eps);
	//	PdcHansenFeasibility pdc(equs->f, false);
	// ====================================================

	// ====================================================
	// solution #2: we call Hansen test in inflating mode.
	PdcHansenFeasibility pdc(equs->f, true);
	// ====================================================

	// TODO: maybe we should check first if the epsbox is inner...
	// otherwise the probability to get a feasible point is
	// perhaps too small?

	// TODO: HansenFeasibility uses midpoint
	//       but maybe we should use random

	if (pdc.test(epsbox)==YES) {
		Interval resI = sys.goal->eval(pdc.solution());
		if (!resI.is_empty()) {
			double res=resI.ub();
			if (res<loup) {
				//TODO : in is_inner, we check again all equalities,
				// it's useless in this case!
				if (is_inner(pdc.solution())) {
					loup = res;
					loup_box = pdc.solution();

					cout << setprecision (12) << " *real* loup update " << loup  << " loup box: " << loup_box << endl;
					return true;
				}
			}
		}
	}
	//===========================================================
	return false;
}

// 2 methods for searching a better feasible point and a better loup

bool Optimizer::update_loup(const IntervalVector& box) {
	bool loup_change=false;
	if (rigor && equs!=NULL) { // a loup point will not be safe (pseudo loup is not the real loup)
		double old_pseudo_loup=pseudo_loup;
		if (update_loup_probing(box) && pseudo_loup < old_pseudo_loup + default_loup_tolerance*fabs(loup-pseudo_loup)) {
			// update pseudo_loup
			loup_change |= update_real_loup();
			old_pseudo_loup=pseudo_loup; // because has changed
		}
		if (update_loup_simplex(box) && pseudo_loup < old_pseudo_loup + default_loup_tolerance*fabs(loup-pseudo_loup)) {
			loup_change |= update_real_loup();
		}
	} else {
		loup_change |= update_loup_probing(box); // update pseudo_loup
		// the loup point is safe: the pseudo loup is the real loup.
		loup=pseudo_loup;
		loup_change |= update_loup_simplex(box);  // update pseudo_loup
		loup = pseudo_loup;
	}
	return loup_change;

}

void Optimizer::update_entailed_ctr(const IntervalVector& box) {
	for (int j=0; j<m; j++) {
		if (entailed->normalized(j)) {
			continue;
		}
		Interval y=sys.ctrs[j].f.eval(box);
		if (y.lb()>0) throw EmptyBoxException();
		else if (y.ub()<=0) {
			entailed->set_normalized_entailed(j);
		}
	}
}

double minimum (double a, double b) {
	if(a<=b) return a;
	else return b;
}

void Optimizer::update_uplo() {
	double new_uplo=POS_INFINITY;

	if (! buffer.empty()) {
		new_uplo= buffer.minimum();
		if (new_uplo > loup) {
			cout << " loup = " << loup << " new_uplo=" << new_uplo << endl;
			ibex_error("optimizer: new_uplo>loup (please report bug)");
		}
		if (new_uplo < uplo_of_epsboxes) uplo = new_uplo;
		else uplo= uplo_of_epsboxes;
	}
	else if (buffer.empty() && loup != POS_INFINITY) {
		// empty buffer : new uplo is set to ymax (loup - precision) if a loup has been found
		new_uplo=compute_ymax(); // not new_uplo=loup, because constraint y <= ymax was enforced
		//    cout << " new uplo buffer empty " << new_uplo << " uplo " << uplo << endl;

		double m = minimum(new_uplo, uplo_of_epsboxes);
		if (uplo < m) uplo = m; // warning: hides the field "m" of the class
		// note: we always have uplo <= uplo_of_epsboxes but we may have uplo > new_uplo, because
		// ymax is strictly lower than the loup.
	}

}



/* contract the box of the cell c , try to find a new loup :;
     push the cell  in the 2 heaps or if the contraction makes the box empty, delete the cell. For diversification, rebuild the 2 heaps
 */

void Optimizer::handle_cell(OptimCell& c, const IntervalVector& init_box ){
	try {
		contract_and_bound(c, init_box);  // may throw EmptyBoxException
		//       objshaver->contract(c.box);


		// Computations for the Casado C3, C5, C7 criteria

		if ((buffer2.crit==CellHeapOptim::C3)||(buffer2.crit==CellHeapOptim::C5)||(buffer2.crit==CellHeapOptim::C7)) {

			compute_pf(c);

			if (loup < 1.e8)
				c.loup=loup;
			else
				c.loup=1.e8;
		}

		// computations for C5, C7 and PU criteria
		if ((buffer2.crit==CellHeapOptim::C5)||(buffer2.crit==CellHeapOptim::C7)||(buffer2.crit==CellHeapOptim::PU))
			compute_pu(c);

		// the cell is put into the 2 heaps
		buffer.push(&c);
		if (critpr > 0)      buffer2.push(&c);

		nb_cells++;
		// reconstruction of the 2 heaps every heap_build_period nodes
		int heap_build_period=100;
		if (nb_cells% heap_build_period ==0) {
			buffer.makeheap();
			if (critpr > 0) buffer2.makeheap();
		}

	}
	catch(EmptyBoxException&) {
		delete &c;
	}
}

void Optimizer::compute_pf(OptimCell& c) {
	c.pf=(sys.goal)->eval(c.box);
}

void Optimizer::compute_pu(OptimCell& c) {
	double pu=1;

	for (int j=1; j<m;j++) {
		if (entailed->normalized(j)) continue;
		Interval eval=sys.f[j].eval(c.box);
		double pui=1;
		if (eval.diam()>0)
			pui= -eval.lb()/eval.diam();
		pu=pu*pui;
	}
	c.pu=pu;
}

void Optimizer::contract_and_bound(OptimCell& c, const IntervalVector& init_box) {

	/*======================== contract y with y<=loup ========================*/
	Interval& y=c.box[ext_sys.goal_var()];

	//	IntervalVector tmp_box(n);
	//	read_ext_box(c.box,tmp_box);

	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;
	//	else ymax= compute_ymax();
	/*
	if (!(y.ub() == ymax))
		  y &= Interval(NEG_INFINITY,ymax+1.e-15);
		else if (y.ub()==ymax && !(tmp_box.contains(loup_point)))
		  y = Interval(y.lb(), ymax+1.e-15);
	 */
	y &= Interval(NEG_INFINITY,ymax);
	if (y.is_empty()) {
		c.box.set_empty();
		throw EmptyBoxException();
	}

	/*================ contract x with f(x)=y and g(x)<=0 ================*/
	//cout << " [contract]  x before=" << c.box << endl;
	//cout << " [contract]  y before=" << y << endl;

	contract(c.box, init_box);

	//cout << " [contract]  x after=" << c.box << endl;
	//cout << " [contract]  y after=" << y << endl;
	// TODO: no more cell in argument here (just a box). Does it matter?
	/*====================================================================*/


	/*========================= update loup =============================*/

	IntervalVector tmp_box(n);
	read_ext_box(c.box,tmp_box);

	entailed = &c.get<EntailedCtr>();
	update_entailed_ctr(tmp_box);

	bool loup_ch=update_loup(tmp_box);

	// update of the upper bound of y in case of a new loup found
	if (loup_ch) y &= Interval(NEG_INFINITY,compute_ymax());

	loup_changed |= loup_ch;

	if (y.is_empty()) { // fix issue #44
		throw EmptyBoxException();
	}

	/*====================================================================*/
	// [gch] TODO: the case (!c.box.is_bisectable()) seems redundant
	// with the case of a NoBisectableVariableException in
	// optimize(). Is update_uplo_of_epsboxes called twice in this case?
	// (bn] NO , the NoBisectableVariableException is raised by the bisector, there are 2 different cases of a non bisected box that may cause an update of uplo_of_epsboxes
	if ((tmp_box.max_diam()<=prec && y.diam() <=goal_abs_prec) || !c.box.is_bisectable()) {
		// rem1: tmp_box and not c.box because y is handled with goal_rel_prec and goal_abs_prec
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)
		// rem 3 : the extended  boxes with no bisectable  domains  should be catched for avoiding infinite bisections
		update_uplo_of_epsboxes(y.lb());
		throw EmptyBoxException();
	}

	//gradient=0 contraction for unconstrained optimization ; 
	//first order test for constrained optimization (useful only when there are no equations replaced by inequalities) 
	//works with the box without the objective (tmp_box)
	firstorder_contract(tmp_box,init_box);
	// the current extended box in the cell is updated
	write_ext_box(tmp_box,c.box);


}


// called with the box without the objective
void Optimizer::firstorder_contract(  IntervalVector& box, const  IntervalVector& init_box) {
	if (m==0) {
		// for unconstrained optimization  contraction with gradient=0
		if (box.is_strict_subset(init_box)) {
			// may throw an EmptyBoxException:
			if (n==1)
				df.backward(Interval::ZERO,box);
			else
				df.backward(IntervalVector(n,Interval::ZERO),box);
		}
	}



	//	else if (equs==NULL)	  {
	//	else {
	//
	//		PdcFirstOrder p(user_sys,init_box);
	//
	//		p.set_entailed(entailed);
	//		if (p.test(box)==NO) throw EmptyBoxException();
	//	  }


}

void Optimizer::contract ( IntervalVector& box, const IntervalVector& init_box) {
	ctc.contract(box);
}

void Optimizer::optimize(const IntervalVector& init_box, double obj_init_bound) {
	loup=obj_init_bound;
	pseudo_loup=obj_init_bound;

	uplo=NEG_INFINITY;
	uplo_of_epsboxes=POS_INFINITY;

	nb_cells=0;
	nb_simplex=0;
	diam_simplex=0;
	nb_rand=0;
	diam_rand=0;

	buffer.flush();
	if (critpr > 0) buffer2.flush();

	OptimCell* root=new OptimCell(IntervalVector(n+1));

	write_ext_box(init_box,root->box);

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	// add data required by optimizer + Fritz John contractor
	root->add<EntailedCtr>();
	//root->add<Multipliers>();
	entailed=&root->get<EntailedCtr>();
	entailed->init_root(user_sys,sys);

	loup_changed=false;
	loup_point=init_box.mid();
	time=0;
	Timer::start();
	handle_cell(*root,init_box);
	int indbuf=0;

	try {
		while (!buffer.empty()) {
			if (trace >= 2) cout << " buffer " << ((CellBuffer&) buffer) << endl;
			if (critpr > 0 && trace >= 2) cout << "  buffer2 " << ((CellBuffer&) buffer2) << endl;
			//		  cout << "buffer size "  << buffer.size() << " " << buffer2.size() << endl;
			// removes from the heap buffer, the cells already chosen in the other buffer
			if (critpr > 0) {
				buffer.cleantop();
				buffer2.cleantop();
			}

			update_uplo();

			if (buffer.empty() || (critpr > 0 && buffer2.empty())) {
				//cout << " buffer empty " << buffer.empty() << " " << buffer2.empty() << endl;
				break;
			}

			loup_changed=false;
			OptimCell *c;
			// random choice between the 2 buffers corresponding to two criteria implemented in two heaps)
			// critpr chances over 100 to choose the second heap
			if (rand() % 100 >=critpr) {
				indbuf=0;
				c=buffer.top();  // the first heap is used
			} else {
				indbuf=1;
				c=buffer2.top();  // the second heap is used
			}

			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);

				pair<OptimCell*,OptimCell*> new_cells=c->bisect(boxes.first,boxes.second);
				if (indbuf ==0) 
					buffer.pop();
				else  
					buffer2.pop();
				if (c->heap_present==0) delete c; // deletes the cell if it is no more present in a heap.

				handle_cell(*new_cells.first, init_box);
				handle_cell(*new_cells.second, init_box);

				if (uplo_of_epsboxes == NEG_INFINITY) {
					cout << " possible infinite minimum " << endl;
					break;
				}
				if (loup_changed) {
					// In case of a new upper bound (loup_changed == true), all the boxes
					// with a lower bound greater than (loup - goal_prec) are removed and deleted.
					// Note: if contraction was before bisection, we could have the problem
					// that the current cell is removed by contract_heap. See comments in
					// older version of the code (before revision 284).

					double ymax= compute_ymax();

					buffer.contract_heap(ymax);
					//cout << " now buffer is contracted and min=" << buffer.minimum() << endl;
					if (critpr > 0) buffer2.contract_heap(ymax);

					if (ymax <=NEG_INFINITY) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) cout << setprecision(12) << "ymax=" << ymax << " uplo= " <<  uplo<< endl;
				}
				update_uplo();
				time_limit_check();

			}
			catch (NoBisectableVariableException& ) {
				update_uplo_of_epsboxes ((c->box)[ext_sys.goal_var()].lb());
				if (indbuf ==0)
					buffer.pop();
				else  
					buffer2.pop();
				if (c->heap_present==0) delete c;


			}
		}
	}
	catch (TimeOutException& ) {
		return;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();
}

void Optimizer::update_uplo_of_epsboxes(double ymin) {

	// the current box cannot be bisected.  ymin is a lower bound of the objective on this box
	// uplo of epsboxes can only go down, but not under uplo : it is an upperbound for uplo,
	//that indicates a lowerbound for the objective in all the small boxes
	// found by the precision criterion
	assert (uplo_of_epsboxes >= uplo);
	assert(ymin >= uplo);
	if (uplo_of_epsboxes > ymin) {
		uplo_of_epsboxes = ymin;
		if (trace) {
			cout << "uplo_of_epsboxes:" << setprecision(12) <<  uplo_of_epsboxes << " uplo " << uplo << endl;
		}
	}
}



void Optimizer::report() {

	if (timeout >0 &&  time >=timeout ) {
		cout << "time limit " << timeout << "s. reached " << endl;
	}
	// No solution found and optimization stopped with empty buffer  before the required precision is reached => means infeasible problem
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
/* minimal report for benchmarking */
void Optimizer::time_cells_report() {
	if (timeout >0 &&  time >=timeout ) {
		cout << "timeout" << timeout << "  " << uplo << " " << loup << " ";}
	else
		cout << time << " " ;
	cout << nb_cells << endl;
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
