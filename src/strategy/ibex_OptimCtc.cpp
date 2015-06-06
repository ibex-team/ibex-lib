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

#include "ibex_NoBisectableVariableException.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

const double OptimCtc::default_prec = 1e-07;
const double OptimCtc::default_goal_rel_prec = 1e-07;
const double OptimCtc::default_goal_abs_prec = 1e-07;



OptimCtc::OptimCtc( Ctc& ctc_out, Ctc&  ctc_in, Function& f_cost, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec) :
							n(f_cost.nb_var()),
							_ctc_out(ctc_out), _ctc_in(ctc_in), _f_cost(f_cost),
							_localopti(f_cost, IntervalVector(f_cost.nb_var())),
							bsc(bsc),
							buffer(*new CellCostPFlb(), *new CellCostPFub()),
							prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
							trace(false), timeout(1e08), time(0), nb_cells(0),
							loup(POS_INFINITY), uplo(NEG_INFINITY),	loup_point(n),initial_loup(POS_INFINITY),
							loup_changed(false),  uplo_of_epsboxes(POS_INFINITY) {


	// =============================================================

	if (trace) cout.precision(12);
}

OptimCtc::~OptimCtc() {

	buffer.flush();

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
				}
			}
		}
	}
	if (trace && loup_change) {
		int prec1=cout.precision();
		cout.precision(12);
		cout << "[localsearch]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
		cout.precision(prec1);
	}
	return loup_change;

}

// a feasible point is a point contracted by ctc_in or a point INACTIVE with ctc_out
bool OptimCtc::direct_try( const Vector point) {
	bool loup_change=false;
	Interval tmp = _f_cost.eval(IntervalVector(point));
	if (tmp.ub()<loup) {
		BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));

		IntervalVector tmpv(point);
		_ctc_out.contract(tmpv, BitSet::all(_ctc_in.nb_var),flags);  //  <---

		if (flags[Ctc::INACTIVE]) {
			//update the loup
			loup = tmp.ub();
			loup_point = point;
			loup_change = true;

			if (trace) {
				int prec1=cout.precision();
				cout.precision(12);
				cout << "[direct-out]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
				cout.precision(prec1);
			}
		} else {
			IntervalVector tmpv(point);
			_ctc_in.contract(tmpv);  //  <---

			if (tmpv.is_empty()) {
				//update the loup
				loup = tmp.ub();
				loup_point = point;
				loup_change = true;

				if (trace) {
					int prec1=cout.precision();
					cout.precision(12);
					cout << "[direct-in] "  << " loup update " << loup  << " loup point  " << loup_point << endl;
					cout.precision(prec1);
				}
			}
		}
	}
	return loup_change;
}



void OptimCtc::update_uplo() {
	double new_uplo=POS_INFINITY;

	if (! buffer.empty()){
		new_uplo= buffer.minimum1();

		if (new_uplo > loup) {
			cout.precision(12);
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

		double m = (new_uplo > uplo_of_epsboxes)? uplo_of_epsboxes : new_uplo;
		if (uplo < m) uplo = m; // warning: hides the field "m" of the class
		// note: we always have uplo <= uplo_of_epsboxes but we may have uplo > new_uplo, because
		// ymax is strictly lower than the loup.
	}

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
		c.get<OptimData>().compute_pf(_f_cost, c.box);

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

		tmp = _f_cost.eval(list[i]);

		if (ymax <=tmp.ub()) {
			// Contract with  f_cost(list[i]) <= ymax
			tmp &= Interval(NEG_INFINITY,ymax);
			HC4Revise(AFFINE_MODE).proj(_f_cost,Domain(tmp),list[i]); /// equivalent to :_f_cost.backward(tmp,list[i]);
		}
		if (!list[i].is_empty())  {
			cell=new Cell(list[i]);
			// add data required by the bisector
			bsc.add_backtrackable(*cell);

			// add data "pu" and "pf"
			buffer.cost1().add_backtrackable(*cell);

			cell->get<OptimData>().compute_pf(_f_cost, cell->box);
			cell->get<OptimData>().pu =1;
			if (cell->get<OptimData>().pf.lb()<ymax) {
				// the cell is put into the 2 heaps with the cost stored in pf
				buffer.push(cell);
			}
		}
	}

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
				/*=====================add box_ok in the buffer=======================*/
				add_buffer(box_ok,size_box_ok);
				delete [] box_ok;
			}
		}
	}

	//cout << " [contract_in]  x after=" << c.box << endl;
	//cout << " [contract_in]  y after=" << y << endl;


	/*========================= update loup =============================*/
	// necessary after the local search
	if (c.box.is_empty()) 	return;

	if (celldata->pu==1) {
		// c.box is feasible
		// Local Optimization
		loup_changed |= localsearch(&c.box,1);
	}
	else  {
		// c.box is not entirely feasible
		// try on the middle point
		loup_changed |= direct_try(c.box.mid());
	}


	/*======================== contract box with f_cost<=loup ========================*/
	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax= compute_ymax()+1.e-15;

	// Contract with  f_cost(c.box) <= loup
	celldata->compute_pf(_f_cost, c.box);
	if (ymax <= celldata->pf.lb())  {
		c.box.set_empty();
		return;
	}
	else if (ymax <=celldata->pf.ub()) {
		IntervalVector save_box(c.box);
		celldata->pf &= Interval(NEG_INFINITY,ymax);
		HC4Revise(AFFINE_MODE).proj(_f_cost,Domain(celldata->pf),c.box); /// equivalent to :_f_cost.backward(c.pf,c.box);
		//draw_vibes(save_box,c.box,"[g]");
		if (c.box.is_empty()) {
			//draw_vibes(c.box,IntervalVector(1,Interval::EMPTY_SET),"[g]");
			return;
		}
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
		loup_changed |= direct_try(c.box.mid());
		update_uplo_of_epsboxes(celldata->pf.lb());
		c.box.set_empty();
		return;
	}

	// monotonicity_analysis
	if (celldata->pu ==1) {
		IntervalVector g(n);
		_f_cost.gradient(c.box,g);
		for (int j=0; j<n; j++) {
			if      ((g[j].lb()>=0)&&(c.box[j].lb()!=NEG_INFINITY)) c.box[j]=c.box[j].lb();
			else if ((g[j].ub()<=0)&&(c.box[j].ub()!=POS_INFINITY)) c.box[j]=c.box[j].ub();
		}
	}
	return;

}


Optimizer::Status OptimCtc::optimize(const IntervalVector& init_box, double obj_init_bound) {
	loup=obj_init_bound;

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
		return Optimizer::TIME_OUT;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	if (uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0)))
		return Optimizer::INFEASIBLE;
	else if (loup==initial_loup)
		return Optimizer::NO_FEASIBLE_FOUND;
	else if (uplo_of_epsboxes == NEG_INFINITY)
		return Optimizer::UNBOUNDED_OBJ;
	else
		return Optimizer::SUCCESS;
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
		if (trace) {
			// it is hard to prove the feasability of a point. So there a lot of small boxes.
			cout << "uplo_of_epsboxes: " <<  uplo_of_epsboxes << " | uplo: " << uplo << " | loup: " << loup << " |"<< endl;
		}
	}
}



void OptimCtc::report() {

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

/*
void OptimCtc::draw_vibes( const IntervalVector& X0, const IntervalVector& X,const string color) {
	if (X.is_empty()) {
		//		vibes::drawBox(X0,color);
		return;
	}
	if (X==X0) return;     // nothing to draw.
	IntervalVector* rest;
	int n=X0.diff(X,rest); // calculate the set difference
	for (int i=0; i<n; i++) {     // display the boxes
		//		vibes::drawBox(rest[i],color);
	}
	delete[] rest;
	return;
}
 */

} // end namespace ibex
