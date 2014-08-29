//                                  I B E X                                   
// File        : ibex_OptimCtc.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : December 24, 2012
//============================================================================

#include "ibex_OptimCtc.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Timer.h"

#include "ibex_NoBisectableVariableException.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

const double OptimCtc::default_prec = 1e-07;
const double OptimCtc::default_goal_rel_prec = 1e-07;
const double OptimCtc::default_goal_abs_prec = 1e-07;



OptimCtc::OptimCtc( Ctc& ctc_out, Ctc&  ctc_in, Function& f_cost, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec, int critpr,CellHeapOptim::criterion crit) :
			n(f_cost.nb_var()),
			_ctc_out(ctc_out), _ctc_in(ctc_in), _f_cost(f_cost),
			_localopti(f_cost, IntervalVector(f_cost.nb_var())),
			bsc(bsc), buffer(n), buffer2(n,crit),  // first buffer with LB, second buffer with crit (default UB))
			prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
			trace(false),
			critpr(critpr), timeout(1e08), time(0), nb_cells(0),
			loup(POS_INFINITY), uplo(NEG_INFINITY),	loup_point(n),
			loup_changed(false),  uplo_of_epsboxes(POS_INFINITY) {


	// =============================================================

	if (trace) cout.precision(12);

	if ((crit==CellHeapOptim::C5)||(crit==CellHeapOptim::C7)||(crit==CellHeapOptim::PU))
		ibex_error("OptimCtc : Criterion option not available choose UB, or C3");

}

OptimCtc::~OptimCtc() {

	buffer.flush();
	if (critpr > 0) buffer2.flush();

}

// compute the value ymax (decreasing the loup with the precision)
// the heap and the current box are contracted with y <= ymax
double OptimCtc::compute_ymax() {
	double ymax= loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;
	return ymax;
}


// 2 methods for searching a better feasible point and a better loup

bool OptimCtc::localsearch(const IntervalVector* box, int nb) {
	if (nb<=0) return false;

	bool loup_change=false;
	{
		Vector v_tmp(box[0].size());
		for (int i=0; i<nb;i++) {
			_localopti.set_box(box[i]);
			UnconstrainedLocalSearch::ReturnCode code =
					_localopti.minimize(box[i].random(),v_tmp,goal_rel_prec,100);
			if (code != UnconstrainedLocalSearch::INVALID_POINT) {
				Interval tmp = _f_cost.eval(v_tmp);
				if (tmp.ub()<loup) {
					//update the loup
					loup = tmp.ub();
					loup_point = v_tmp;
					loup_change = true;
					{
						int prec=cout.precision();
						cout.precision(12);
						cout << "[localsearch]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
						cout.precision(prec);
					}
				}
			}
		}
	}
	return loup_change;

}

bool OptimCtc::direct_try( const Vector point) {
	bool loup_change=false;
	try {
		IntervalVector tmp(point);
		_ctc_in.contract(tmp);
	} catch (EmptyBoxException &) {
		Interval tmp = _f_cost.eval(point);
		if (tmp.ub()<loup) {
			//update the loup
			loup = tmp.ub();
			loup_point = point;
			loup_change = true;
		    {
				int prec=cout.precision();
				cout.precision(12);
				cout << "[random]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
				cout.precision(prec);
		    }
		}
	}
	return loup_change;
}


double minimum (double a, double b) {
	if(a<=b) return a;
	else return b;
}


void OptimCtc::update_uplo() {
	double new_uplo=POS_INFINITY;

	if (! buffer.empty()){
		new_uplo= buffer.minimum();
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

void OptimCtc::handle_cell(OptimCell& c, const IntervalVector& init_box ){
	try{
		compute_pf(c);
		contract_and_bound(c, init_box);  // may throw EmptyBoxException

		if (loup < 1.e8)
			c.loup=loup;
		else
			c.loup=1.e8;

		// the cell is put into the 2 heaps
		buffer.push_costpf(&c);
		if (critpr > 0)      buffer2.push_costpf(&c);

		nb_cells++;
		// reconstruction of the 2 heaps every heap_build_period nodes
		// TODO not necessary I think ??
		int heap_build_period=10000;
		if (nb_cells% heap_build_period ==0) {
			buffer.makeheap();
			if (critpr > 0) buffer2.makeheap();
		}

	}
	catch(EmptyBoxException&) {
		delete &c;
	}
}

void OptimCtc::compute_pf(OptimCell& c) {
	//OR 	c.pf &=_f_cost.eval(c.box);
	c.pf &= _f_cost.eval_affine2(c.box);
}


void OptimCtc::add_buffer_pf(IntervalVector* list, int size) {
	OptimCell* cell;
	double ymax;
	Interval tmp;

	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;

	for (int i=0; i<size;i++) {

		tmp = _f_cost.eval(list[i]);
		try {
			if (ymax <=tmp.ub()) {
				// Contract with  f_cost(list[i]) <= ymax
				tmp &= Interval(NEG_INFINITY,ymax);
				HC4Revise(AFFINE_MODE).proj(_f_cost,Domain(tmp),list[i]); /// equivalent to :_f_cost.backward(tmp,list[i]);
			}
			cell=new OptimCell(list[i]);
			cell->pu  = 1;
			cell->pf  = tmp;
			cell->loup= loup;
			// add data required by the bisector
			bsc.add_backtrackable(*cell);

			// the cell is put into the 2 heaps with the cost stored in pf
			buffer.push_costpf(cell);
			if (critpr > 0)      buffer2.push_costpf(cell);

		} catch (EmptyBoxException& ) {/* nothing to do */}
	}

}



void OptimCtc::contract_and_bound(OptimCell& c, const IntervalVector& init_box) {

	/*======================== contract y with y<=loup ========================*/

	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;


	// Contract with  f_cost(c.box) <= loup
	if (ymax <=c.pf.ub()) {
		try {
			c.pf &= Interval(NEG_INFINITY,ymax);
			HC4Revise(AFFINE_MODE).proj(_f_cost,Domain(c.pf),c.box); /// equivalent to :_f_cost.backward(c.pf,c.box);
		} catch (EmptyBoxException& e) {
			c.box.set_empty();
			throw e;
		}
	}

	/*================ contract x with Ctc_out ================*/
	//cout << " [contract_out]  x before=" << c.box << endl;
	//cout << " [contract_out]  y before=" << y << endl;

	// Contract only if all the constraint are not satisfied
	if (c.pu ==0)	contract(c.box);

	//cout << " [contract_out]  x after=" << c.box << endl;
	//cout << " [contract_out]  y after=" << y << endl;
	/*====================================================================*/


	/*================ contract x with Ctc_in ================*/
	//cout << " [contract_in]  x before=" << c.box << endl;
	//cout << " [contract_in]  y before=" << y << endl;

	// Contract only if all the constraint are not satisfied

	IntervalVector * box_ok;
	int size_box_ok = 0;
	if (c.pu ==0)	{
		try {
			IntervalVector tmp(c.box);
			_ctc_in.contract(tmp);
			size_box_ok = c.box.diff(tmp,box_ok);
			if ((size_box_ok==1)&&(box_ok[0].is_empty())) {
				size_box_ok=0;
				delete[] box_ok;
			}
			else {
				c.box = tmp;
			}
		} catch (EmptyBoxException &) {
			c.pu=1;
		}
	}

	//cout << " [contract_in]  x after=" << c.box << endl;
	//cout << " [contract_in]  y after=" << y << endl;
	/*====================================================================*/

	/*========================= update loup =============================*/

	bool loup_ch = localsearch(box_ok,size_box_ok);
	if (c.pu==1) {
		loup_ch = (loup_ch || localsearch(&c.box,1));
	}
	else  {
		// try on the middle point
		loup_ch = (loup_ch || direct_try(c.box.random()));
	}

	c.pf &=_f_cost.eval_affine2(c.box);
	// update of the upper bound of y in case of a new loup found
	if (loup_ch)  	c.pf &= Interval(NEG_INFINITY,compute_ymax());
	loup_changed |= loup_ch;

	if (c.pf.is_empty()) {
		c.box.set_empty();
		throw EmptyBoxException();
	}
	/*====================================================================*/

	/*=====================add box_ok in the buffer=======================*/
	if (size_box_ok>0) {
		add_buffer_pf(box_ok,size_box_ok);
		delete [] box_ok;
	}
	/*====================================================================*/

	if ((c.box.max_diam()<=prec) || !c.box.is_bisectable()) {
		// rem1: tmp_box and not c.box because y is handled with goal_rel_prec and goal_abs_prec
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)
		// rem 3 : the extended  boxes with no bisectable  domains  should be catched for avoiding infinite bisections

//if ((c.box.max_diam()<=prec && c.pf.diam() <=goal_abs_prec) || !c.box.is_bisectable()) {
		cout.precision(12);
		cout << direct_try(c.box.mid());
cout<< " ici contract : "<<c.pu<<"  " <<c.pf<<"  "<<c.box<<endl;
			try{
				_ctc_out.contract(c.box);
cout<< " ici contract : "<<c.box<<endl;
			} catch (EmptyBoxException &) {
				cout << "OUT"<<endl;
			}
			try{
				_ctc_in.contract(c.box);
cout<< " ici contract : "<<c.box<<endl;
			} catch (EmptyBoxException &) {
				cout << "IN "<<endl;
			}
			//compute_pf(*c);
			update_uplo_of_epsboxes(c.pf.lb());

		throw EmptyBoxException();
	}

	// monotonicity_analysis
	if (c.pu ==1) {
		IntervalVector g(n);
		_f_cost.gradient(c.box,g);
		for (int j=0; j<n; j++) {
			if      ((g[j].lb()>=0)&&(c.box[j].lb()!=NEG_INFINITY)) c.box[j]=c.box[j].lb();
			else if ((g[j].ub()<=0)&&(c.box[j].ub()!=POS_INFINITY)) c.box[j]=c.box[j].ub();
		}

	}

}



void OptimCtc::contract ( IntervalVector& box) {
	_ctc_out.contract(box);
}

void OptimCtc::optimize(const IntervalVector& init_box, double obj_init_bound) {
	loup=obj_init_bound;

	uplo=NEG_INFINITY;
	uplo_of_epsboxes=POS_INFINITY;

	nb_cells=0;

	buffer.flush();
	if (critpr > 0) buffer2.flush();

	OptimCell* root=new OptimCell(init_box);
	root->pu = 0;
	root->pf = _f_cost.eval_affine2(init_box);
	root->loup=loup;

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	loup_changed=false;
	loup_point=init_box.mid();
	time=0;
	Timer::start();
	handle_cell(*root,init_box);
	int indbuf=0;

	try {
		while (!buffer.empty()) {
			if (trace >= 2) cout << " buffer " << ((CellBuffer&) buffer) << endl;
			if (critpr> 0 && trace >= 2) cout << "  buffer2 " << ((CellBuffer&) buffer2) << endl;
			//		  cout << "buffer size "  << buffer.size() << " " << buffer2.size() << endl;
			// removes from the heap buffer, the cells already chosen in the other buffer
			if (critpr >0)	{
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
			}
			else {
				indbuf=1;
				c=buffer2.top();  // the second heap is used
			}

			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<OptimCell*,OptimCell*> new_cells=c->bisect(boxes.first,boxes.second);

				// Transfer the information to the sons.
				new_cells.first->pf = c->pf;
				new_cells.second->pf= c->pf;
				new_cells.first->pu = c->pu;
				new_cells.second->pu= c->pu;
				new_cells.first->loup = c->loup;
				new_cells.second->loup= c->loup;

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
					if (critpr > 0) buffer2.contract_heap(ymax);

					if (ymax <=NEG_INFINITY) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) { //setprecision(12);
						cout  << "ymax=" << ymax << " uplo= " <<  uplo << endl;
					}
				}
				update_uplo();
				time_limit_check();

			}
			catch (NoBisectableVariableException& ) {
			//	cout<< " ici opti : "<<c->pu<<"  " <<c->pf<<"  "<<c->box<<endl;
				try{
					_ctc_out.contract(c->box);
				} catch (EmptyBoxException &) {
					cout << "OUT"<<endl;
				}
				try{
					_ctc_in.contract(c->box);
				} catch (EmptyBoxException &) {
					cout << "IN "<<endl;
				}
				//compute_pf(*c);

				update_uplo_of_epsboxes (c->pf.lb());

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

void OptimCtc::update_uplo_of_epsboxes(double ymin) {

	// the current box cannot be bisected.  ymin is a lower bound of the objective on this box
	// uplo of epsboxes can only go down, but not under uplo : it is an upperbound for uplo,
	//that indicates a lowerbound for the objective in all the small boxes
	// found by the precision criterion
	assert (uplo_of_epsboxes >= uplo);
	assert(ymin >= uplo);
	if (uplo_of_epsboxes > ymin) 	{
		uplo_of_epsboxes = ymin;
		if (trace) { // << setprecision(12)
			cout << "uplo_of_epsboxes: " <<  uplo_of_epsboxes << " | uplo: " << uplo << " | loup: " << loup << " |"<< endl;
		exit(-1);}
	}
}



void OptimCtc::report() {

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

}

/* minimal report for benchmarking */
void OptimCtc::time_cells_report() {
	if (timeout >0 &&  time >=timeout ) {
		cout << "timeout" << timeout << "  " << uplo << " " << loup << " ";}
	else
		cout << time << " " ;
	cout << nb_cells << endl;
}


void OptimCtc::report_perf() {

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

void OptimCtc::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (timeout >0 &&  time >=timeout ) throw TimeOutException();
	Timer::start();
}

} // end namespace ibex
