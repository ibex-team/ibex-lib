//                                  I B E X                                   
// File        : ibex_OptimCtc.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : May 14, 2014
// Last Update : May 14, 2015
//============================================================================

#include "ibex_OptimCtc.h"
#include "ibex_Timer.h"
#include "ibex_OptimData.h"
#include "ibex_CellCostFunc.h"
#include "ibex_HC4Revise.h"
#include "ibex_BitSet.h"

#include "ibex_NoBisectableVariableException.h"

#include <stdlib.h>

using namespace std;

namespace ibex {




OptimCtc::OptimCtc( Ctc& ctc_out, Ctc&  ctc_in, Function& f_cost, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec) :
				MainOpti( f_cost, bsc, *new CellDoubleHeap(*new CellCostPFlb(), *new CellCostPFub()), prec, goal_rel_prec,  goal_abs_prec),
				_ctc_out(ctc_out), _ctc_in(ctc_in) {

}

OptimCtc::~OptimCtc() { }



// a feasible box is a box contracted by ctc_in or a point INACTIVE with ctc_out
bool OptimCtc::is_inner( const IntervalVector& box) {
	BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));

	IntervalVector tmpv(box);
	_ctc_out.contract(tmpv, BitSet::all(_ctc_in.nb_var),flags);  //  <---

	if (flags[Ctc::INACTIVE]) {
		//update the loup
//		if (trace) {
//			int prec1=cout.precision();
//			cout.precision(12);
//			cout << "[direct-out]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
//			cout.precision(prec1);
//		}
		return true;

	} else {
		IntervalVector tmpv(box);
		_ctc_in.contract(tmpv);  //  <---

		if (tmpv.is_empty()) {
			//update the loup
//			if (trace) {
//				int prec1=cout.precision();
//				cout.precision(12);
//				cout << "[direct-in] "  << " loup update " << loup  << " loup point  " << loup_point << endl;
//				cout.precision(prec1);
//			}
			return true;
		}
	}

	return false;
}




/* contract the box of the cell c , try to find a new loup :;
     push the cell  in the 2 heaps or if the contraction makes the box empty, delete the cell. For diversification, rebuild the 2 heaps
 */

void OptimCtc::handle_cell(Cell& c){

	contract_and_bound(c);

	if (c.box.is_empty()) {
		delete &c;
	} else {
		//       objshaver->contract(c.box);

		// we know cost1() does not require OptimData
		c.get<OptimData>().compute_pf(f_cost, c.box);

		double ymax;
		if (loup==POS_INFINITY) ymax=POS_INFINITY;
		else ymax= compute_ymax();

		if (c.get<OptimData>().pf.lb()<ymax) {
			// the cell is put into the 2 heaps
			buffer.push(&c);
			nb_cells++;
		} else {
			delete &c;
		}
	}
}


void OptimCtc::add_buffer(IntervalVector* list, int size) {
	Cell* cell;
	double ymax;
	Interval tmp;

	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;

	for (int i=0; i<size;i++) {

		tmp = f_cost.eval(list[i]);

		if (ymax <=tmp.ub()) {
			// Contract with  f_cost(list[i]) <= ymax
			tmp &= Interval(NEG_INFINITY,ymax);
			HC4Revise().proj(f_cost,Domain(tmp),list[i]); /// equivalent to :_f_cost.backward(tmp,list[i]);
		}
		if (!list[i].is_empty())  {
			cell=new Cell(list[i]);
			// add data required by the bisector
			bsc.add_backtrackable(*cell);

			// add data "pu" and "pf"
			buffer.cost1().add_backtrackable(*cell);

			cell->get<OptimData>().compute_pf(f_cost, cell->box);
			cell->get<OptimData>().pu =1;
			if (cell->get<OptimData>().pf.lb()<ymax) {
				// the cell is put into the 2 heaps with the cost stored in pf
				buffer.push(cell);
			}
		}
	}

}

// monotonicity_analysis
void OptimCtc::monotonicity_analysis(IntervalVector& box, bool inner_found){

	if (inner_found) {

		IntervalVector g(n);
		f_cost.gradient(box,g);
		//if box is unbounded, we have a problem here. DONE by JN
		for (int j=0; j<n; j++) {
			if      ((g[j].lb()>=0)&&(box[j].lb()!=NEG_INFINITY)) box[j]=box[j].lb();
			if ((g[j].ub()<=0)&&(box[j].ub()!=POS_INFINITY)) box[j]=box[j].ub();
		}
	}
	return;
}


void OptimCtc::contract_and_bound(Cell& c) {

	OptimData *celldata = &(c.get<OptimData>());

	/*================ contract x with Ctc_out ================*/
	//cout << " [contract_out]  x before=" << c.box << endl;
	//cout << " [contract_out]  y before=" << y << endl;

	// Contract only if all the constraint are not satisfied
	if (celldata->pu <1)	{

		BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));
		_ctc_out.contract(c.box, BitSet::all(_ctc_out.nb_var),flags); // no need to catch, it is done in handle_cell

		if (flags[Ctc::INACTIVE]) {
			// all the constraint are inactive ( = the entire box is feasible)
			celldata->pu =1;
		}
		if (c.box.is_empty()) return;
	}
	//cout << " [contract_out]  x after=" << c.box << endl;
	//cout << " [contract_out]  y after=" << y << endl;
	/*====================================================================*/


	/*================ contract x with Ctc_in ================*/
	//cout << " [contract_in]  x before=" << c.box << endl;
	//cout << " [contract_in]  y before=" << y << endl;


	// Contract only if all the constraint are not satisfied
	if (celldata->pu <1)	{

		IntervalVector tmp(c.box);
		BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));
		_ctc_in.contract(tmp, BitSet::all(_ctc_in.nb_var),flags);

		if (tmp==c.box) {
			if (flags[Ctc::INACTIVE]) {
				// all the constraint are active ( = the entire box is unfeasible)
				c.box.set_empty();
				return;
			}
		} else {
			IntervalVector * box_ok;
			int size_box_ok = 0;
			size_box_ok = c.box.diff(tmp,box_ok);
			if ((size_box_ok==1)&&(box_ok[0].is_empty())) {
				delete[] box_ok;
			}
			else {
				c.box = tmp;
				// Local Optimization on all the feasible box stored in box_ok
				loup_changed |= localsearch(box_ok,size_box_ok);
				loup=pseudo_loup;
				/*=====================add box_ok in the buffer=======================*/
				add_buffer(box_ok,size_box_ok);
				delete [] box_ok;
			}
		}
	}

	//cout << " [contract_in]  x after=" << c.box << endl;
	//cout << " [contract_in]  y after=" << y << endl;


	/*========================= update loup =============================*/

	// the loup point is safe: the pseudo loup is the real loup.
	loup=pseudo_loup;

	if (c.box.is_empty()) 	return;

	if (celldata->pu==1) {
		// c.box is feasible
		// Local Optimization
		loup_changed |= localsearch(c.box);
		loup=pseudo_loup;
	}

	// c.box is not entirely feasible
	// try on the middle point
	loup_changed |= check_candidate(c.box.mid(),celldata->pu==1);
	loup=pseudo_loup;



	/*======================== contract box with f_cost<=loup ========================*/
	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;

	// Contract with  f_cost(c.box) <= loup
	celldata->compute_pf(f_cost, c.box);
	if (ymax <= celldata->pf.lb())  {
		c.box.set_empty();
		return;
	}
	else if (ymax <=celldata->pf.ub()) {
		IntervalVector save_box(c.box);
		celldata->pf &= Interval(NEG_INFINITY,ymax);
		HC4Revise().proj(f_cost,Domain(celldata->pf),c.box); /// equivalent to :_f_cost.backward(c.pf,c.box);

		if (c.box.is_empty()) 	return;
	}

	/*====================================================================*/


	/*====================================================================*/
	/*====================================================================*/
	// [gch] The case (!c.box.is_bisectable()) seems redundant
	// with the case of a NoBisectableVariableException in
	// optimize(). Is update_uplo_of_epsboxes called twice in this case?
	// (bn] NO , the NoBisectableVariableException is raised by the bisector, there are 2 different cases of a non bisected box that may cause an update of uplo_of_epsboxes
	if ((c.box.max_diam()<=prec && celldata->pf.diam() <=goal_abs_prec) || !c.box.is_bisectable()) {
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)
		// rem 3 : the boxes with no bisectable  domains  should be catched for avoiding infinite bisections
		loup_changed |= check_candidate(c.box.mid(),false);
		update_uplo_of_epsboxes(celldata->pf.lb());
		c.box.set_empty();
		return;
	}

	// monotonicity_analysis
	monotonicity_analysis(c.box, (celldata->pu ==1));


	return;

}



Status_Opti OptimCtc::optimize(const IntervalVector& init_box, double obj_init_bound) {
	start_box = init_box;

	loup=obj_init_bound;
	pseudo_loup=loup;

	uplo=NEG_INFINITY;
	uplo_of_epsboxes=POS_INFINITY;

	nb_cells=0;

	buffer.flush();

	Cell* root=new Cell(init_box);

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	// add data "pu" and "pf" (if required)
	buffer.cost1().add_backtrackable(*root);


	loup_changed=false;
	initial_loup=obj_init_bound;
	loup_point=init_box.mid();
	time=0;
	Timer::start();
	handle_cell(*root);

	if (loup_changed) {
		loup_changed=false;
		buffer.contract(compute_ymax());
	}
	update_uplo();

	try {
		while (!buffer.empty()) {
			//			if (trace >= 2) cout << " buffer " << buffer << endl;
			if (trace >= 2) buffer.print(cout);
			//		  cout << "buffer size "  << buffer.size() << " " << buffer2.size() << endl;
			// removes from the heap buffer, the cells already chosen in the other buffer

			if (buffer.empty()) {
				//cout << " buffer empty " << buffer.empty() << " " << buffer2.empty() << endl;
				// this update is only necessary when buffer was not
				// initially empty
				update_uplo();
				break;
			}

			loup_changed=false;
			Cell *c;

			// random choice between the 2 buffers corresponding to two criteria implemented in two heaps)
			// critpr chances over 100 to choose the second heap (see CellDoubleHeap)
			c=buffer.top();


			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				buffer.pop();
				delete c; // deletes the cell.

				handle_cell(*new_cells.first);
				handle_cell(*new_cells.second);

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

					buffer.contract(ymax);

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
				update_uplo_of_epsboxes(c->get<OptimData>().pf.lb());
				buffer.pop();
				delete c; // deletes the cell.

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





} // end namespace ibex
