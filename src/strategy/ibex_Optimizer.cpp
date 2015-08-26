//                                  I B E X                                   
// File        : ibex_Optimizer.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : December 24, 2012
//============================================================================

#include "ibex_Optimizer.h"
#include "ibex_EmptySystemException.h"
#include "ibex_Timer.h"
#include "ibex_OptimProbing.cpp_"
#include "ibex_OptimSimplex.cpp_"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcOptimShaving.h"
#include "ibex_CtcHC4.h"
#include "ibex_Random.h"

#include "ibex_ExprCopy.h"
#include "ibex_Function.h"
#include "ibex_NoBisectableVariableException.h"
//#include "ibex_Multipliers.h"
#include "ibex_PdcFirstOrder.h"
#include "ibex_OptimData.h"

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
		bool rigor,  int critpr,CellCostFunc::criterion crit2) :
                				user_sys(user_sys), sys(user_sys,equ_eps),
                				n(user_sys.nb_var), m(sys.nb_ctr) /* (warning: not user_sys.nb_ctr) */,
                				ext_sys(user_sys,equ_eps),
                				ctc(ctc),bsc(bsc),
                				buffer(*new CellCostVarLB(n), *CellCostFunc::get_cost(crit2, n), critpr),  // first buffer with LB, second buffer with ct (default UB))
                				prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
                				sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
                				timeout(1e08),
                				loup(POS_INFINITY), pseudo_loup(POS_INFINITY),uplo(NEG_INFINITY),
                				loup_point(n), loup_box(n), nb_cells(0),
                				df(*user_sys.goal,Function::DIFF), loup_changed(false),	initial_loup(POS_INFINITY), rigor(rigor),
                				uplo_of_epsboxes(POS_INFINITY) {

	// ==== build the system of equalities only ====
	try {
		equs= new System(user_sys,System::EQ_ONLY);
	} catch(EmptySystemException&) {
		equs= NULL;
	}

	// ====== build the reversed inequalities g_i(x)>0 ===============
	is_inside=m>0? new CtcUnion(sys) : NULL;
	// =============================================================

	if (trace) cout.precision(12);

	//	objshaver= new CtcOptimShaving (*new CtcHC4 (ext_sys.ctrs,0.1,true),20,1,1.e-11);

	int niter=100;
	// int niter=1000;
	if (niter < 3*n) niter=3*n;

	//====================================
#ifdef _IBEX_WITH_NOLP_
	mylp = NULL;
#else
	//lr = new LinearRelaxCombo(sys, LinearRelaxCombo::XNEWTON);
	//mylp = new LinearSolver(sys.nb_var,sys.nb_ctr,niter);
	lr = new LinearRelaxCombo(ext_sys, LinearRelaxCombo::XNEWTON);
	mylp = new LinearSolver(ext_sys.nb_var,ext_sys.nb_ctr,niter);
	//	cout << "sys " << sys << endl;
#endif // _IBEX_WITH_NOLP_
}

Optimizer::~Optimizer() {
	if (is_inside) {
		for (int i=0; i<m; i++) {
			delete &(is_inside->list[i]);
		}

		delete is_inside;
	}
	buffer.flush();
	if (equs) delete equs;
	delete mylp;
	delete lr;
	delete &buffer.cost1();
	delete &buffer.cost2();
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

bool Optimizer::update_entailed_ctr(const IntervalVector& box) {
	for (int j=0; j<m; j++) {
		if (entailed->normalized(j)) {
			continue;
		}
		Interval y=sys.ctrs[j].f.eval(box);
		if (y.lb()>0) return false;
		else if (y.ub()<=0) {
			entailed->set_normalized_entailed(j);
		}
	}
	return true;
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
		if (new_uplo < uplo) {
			cout << "uplo= " << uplo << " new_uplo=" << new_uplo << endl;
			ibex_error("optimizer: new_uplo<uplo (please report bug)");
		}

		if (new_uplo < uplo_of_epsboxes) {
			if (new_uplo > uplo) {
				//cout << "uplo update=" << uplo << endl;
				uplo = new_uplo;
			}
		}
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



/* contract the box of the cell c , try to find a new loup ;
     push the cell  in the 2 heaps or if the contraction makes the box empty, delete the cell.
 */

void Optimizer::handle_cell(Cell& c, const IntervalVector& init_box ){

	contract_and_bound(c, init_box);

	if (c.box.is_empty()) {
		delete &c;
	} else {
		//       objshaver->contract(c.box);

		// we know cost1() does not require OptimData
		buffer.cost2().set_optim_data(c,sys);

		// the cell is put into the 2 heaps
		buffer.push(&c);

		nb_cells++;
	}
}

void Optimizer::contract_and_bound(Cell& c, const IntervalVector& init_box) {

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
		return;
	}

	/*================ contract x with f(x)=y and g(x)<=0 ================*/
	//cout << " [contract]  x before=" << c.box << endl;
	//cout << " [contract]  y before=" << y << endl;

	ctc.contract(c.box);

	if (c.box.is_empty()) return;

	//cout << " [contract]  x after=" << c.box << endl;
	//cout << " [contract]  y after=" << y << endl;
	// TODO: no more cell in argument here (just a box). Does it matter?
	/*====================================================================*/

	/*========================= update loup =============================*/

	IntervalVector tmp_box(n);
	read_ext_box(c.box,tmp_box);

	entailed = &c.get<EntailedCtr>();
	if (!update_entailed_ctr(tmp_box)) {
		c.box.set_empty();
		return;
	}

	bool loup_ch=update_loup(tmp_box);

	// update of the upper bound of y in case of a new loup found
	if (loup_ch) y &= Interval(NEG_INFINITY,compute_ymax());

	loup_changed |= loup_ch;

	if (y.is_empty()) { // fix issue #44
		c.box.set_empty();
		return;
	}

	/*====================================================================*/
	// [gch] The case (!c.box.is_bisectable()) seems redundant
	// with the case of a NoBisectableVariableException in
	// optimize(). Is update_uplo_of_epsboxes called twice in this case?
	// (bn] NO , the NoBisectableVariableException is raised by the bisector, there are 2 different cases of a non bisected box that may cause an update of uplo_of_epsboxes
	if ((tmp_box.max_diam()<=prec && y.diam() <=goal_abs_prec) || !c.box.is_bisectable()) {
		// rem1: tmp_box and not c.box because y is handled with goal_rel_prec and goal_abs_prec
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)
		// rem 3 : the extended  boxes with no bisectable domains should be caught for avoiding infinite bisections
		update_uplo_of_epsboxes(y.lb());
		c.box.set_empty();
		return;
	}

	//gradient=0 contraction for unconstrained optimization ; 
	//first order test for constrained optimization (useful only when there are no equations replaced by inequalities) 
	//works with the box without the objective (tmp_box)
	firstorder_contract(tmp_box,init_box);

	if (tmp_box.is_empty()) {
		c.box.set_empty();
	} else {
		// the current extended box in the cell is updated
		write_ext_box(tmp_box,c.box);
	}
}


// called with the box without the objective
void Optimizer::firstorder_contract(IntervalVector& box, const IntervalVector& init_box) {
	if (m==0) {
		// for unconstrained optimization  contraction with gradient=0
		if (box.is_strict_interior_subset(init_box)) {
			if (n==1)
				df.backward(Interval::ZERO,box);
			else
				df.backward(IntervalVector(n,Interval::ZERO),box);
		}
	}

//	else {
//
//		PdcFirstOrder p(user_sys,init_box);
//
//		p.set_entailed(entailed);
//		if (p.test(box)==NO) {
//			box.set_empty();
//		}
//	}
}

Optimizer::Status Optimizer::optimize(const IntervalVector& init_box, double obj_init_bound) {
	loup=obj_init_bound;
	pseudo_loup=obj_init_bound;
	buffer.contract(loup);

	uplo=NEG_INFINITY;
	uplo_of_epsboxes=POS_INFINITY;

	nb_cells=0;
	nb_simplex=0;
	diam_simplex=0;
	nb_rand=0;
	diam_rand=0;

	buffer.flush();

	Cell* root=new Cell(IntervalVector(n+1));

	write_ext_box(init_box,root->box);

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	// add data "pu" and "pf" (if required)
	buffer.cost2().add_backtrackable(*root);

	// add data required by optimizer + Fritz John contractor
	root->add<EntailedCtr>();
	//root->add<Multipliers>();
	entailed=&root->get<EntailedCtr>();
	entailed->init_root(user_sys,sys);

	loup_changed=false;
	initial_loup=obj_init_bound;
	loup_point=init_box.mid();
	time=0;
	Timer::start();
	handle_cell(*root,init_box);

	update_uplo();

	try {
		while (!buffer.empty()) {
		  //			if (trace >= 2) cout << " buffer " << buffer << endl;
		  if (trace >= 2) buffer.print(cout);
			//		  cout << "buffer size "  << buffer.size() << " " << buffer2.size() << endl;

			loup_changed=false;

			Cell *c = buffer.top();

			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);

				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				buffer.pop();
				delete c; // deletes the cell.

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
					// that the current cell is removed by contractHeap. See comments in
					// older version of the code (before revision 284).

					double ymax=compute_ymax();

					buffer.contract(ymax);
					//cout << " now buffer is contracted and min=" << buffer.minimum() << endl;


					if (ymax <= NEG_INFINITY) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) cout << setprecision(12) << "ymax=" << ymax << " uplo= " <<  uplo<< endl;
				}
				update_uplo();
				time_limit_check();

			}
			catch (NoBisectableVariableException& ) {
				update_uplo_of_epsboxes((c->box)[ext_sys.goal_var()].lb());
				buffer.pop();
				delete c; // deletes the cell.
				//if (trace>=1) cout << "epsilon-box found: uplo cannot exceed " << uplo_of_epsboxes << endl;
				update_uplo(); // the heap has changed -> recalculate the uplo

			}
		}
	}
	catch (TimeOutException& ) {
		return TIME_OUT;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	if (uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0)))
		return INFEASIBLE;
	else if (loup==initial_loup)
		return NO_FEASIBLE_FOUND;
	else if (uplo_of_epsboxes == NEG_INFINITY)
		return UNBOUNDED_OBJ;
	else
		return SUCCESS;
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
	if (buffer.empty() && uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0))) {
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
		if (loup==initial_loup)
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
