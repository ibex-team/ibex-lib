//                                  I B E X                                   
// File        : ibex_MainOpti.cpp
// Author      : Gilles Chabert, Bertrand Neveu, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// Copyright   : ENSTA-Bretagne (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : June 15, 2015
//============================================================================

#include "ibex_MainOpti.h"
#include "ibex_Timer.h"

#include <float.h>
#include <stdlib.h>

using namespace std;

namespace ibex {

const double MainOpti::default_prec = 1e-07;
const double MainOpti::default_goal_rel_prec = 1e-07;
const double MainOpti::default_goal_abs_prec = 1e-07;
const double MainOpti::default_loup_tolerance = 0.1;

MainOpti::MainOpti( Function& f_cost, Bsc& bsc, CellDoubleHeap* buffer, double prec, double goal_rel_prec, double goal_abs_prec):
                				n(f_cost.nb_var()),bsc(bsc),
                				buffer(buffer),  // first buffer with LB, second buffer with ct (default UB))
                				prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
                				trace(false), timeout(1e08), time(0),
                				loup(POS_INFINITY), pseudo_loup(POS_INFINITY), uplo(NEG_INFINITY),
                				loup_point(n), loup_box(n), nb_cells(0),
                				loup_changed(false),initial_loup(POS_INFINITY),
                				f_cost(f_cost), uplo_of_epsboxes(POS_INFINITY),
                				localopti(f_cost, IntervalVector(f_cost.nb_var())){

}

MainOpti::~MainOpti() {

	buffer->flush();
	delete &buffer->cost1();
	delete &buffer->cost2();
	delete buffer;
	//	delete &(objshaver->ctc);
	//	delete objshaver;
}



// compute the value ymax (decreasing the loup with the precision)
// the heap and the current box are contracted with y <= ymax
double MainOpti::compute_ymax()  const {
	double ymax= loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;
	return ymax;
}



/* last update: GCH  */
bool MainOpti::check_candidate(const Vector& pt, bool _is_inner) {

	// "res" will contain an upper bound of the criterion
	double res = goal(pt);

	// check if f(x) is below the "loup" (the current upper bound).
	//
	// The "loup" and the corresponding "loup_point" (the current minimizer)
	// will be updated if the constraints are satisfied.
	// The test of the constraints is done only when the evaluation of the criterion
	// is better than the loup (a cheaper test).

	//        cout << " res " <<  res << " loup " <<  pseudo_loup <<  " is_inner " << _is_inner << endl;
	if (res<loup) {
		if (_is_inner || is_inner(IntervalVector(pt))) {
			pseudo_loup = res;
			loup_point = pt;
			return true;
		}
	}

	return false;
}


void MainOpti::update_uplo() {
	double new_uplo=POS_INFINITY;

	if (! buffer->empty()) {
		new_uplo= buffer->minimum();
		if (new_uplo > loup) {
			cout << " loup = " << loup << " new_uplo=" << new_uplo << endl;
			ibex_error("MainOpti: new_uplo>loup (please report bug)");
		}
		if (new_uplo < uplo_of_epsboxes) uplo = new_uplo;
		else uplo= uplo_of_epsboxes;
	}
	else if (buffer->empty() && loup != POS_INFINITY) {
		// empty buffer : new uplo is set to ymax (loup - precision) if a loup has been found
		new_uplo=compute_ymax(); // not new_uplo=loup, because constraint y <= ymax was enforced
		//    cout << " new uplo buffer empty " << new_uplo << " uplo " << uplo << endl;

		double m = (new_uplo > uplo_of_epsboxes)? uplo_of_epsboxes : new_uplo;
		if (uplo < m) uplo = m; // warning: hides the field "m" of the class
		// note: we always have uplo <= uplo_of_epsboxes but we may have uplo > new_uplo, because
		// ymax is strictly lower than the loup.
	}

}


void MainOpti::update_uplo_of_epsboxes(double ymin) {

	// the current box cannot be bisected.  ymin is a lower bound of the objective on this box
	// uplo of epsboxes can only go down, but not under uplo : it is an upperbound for uplo,
	//that indicates a lowerbound for the objective in all the small boxes
	// found by the precision criterion
	assert (uplo_of_epsboxes >= uplo);
	assert(ymin >= uplo);
	if (uplo_of_epsboxes > ymin) {
		uplo_of_epsboxes = ymin;
		if (trace) {
			// it is hard to prove the feasability of a point. So there a lot of small boxes.
			cout << "uplo_of_epsboxes: " <<  uplo_of_epsboxes << " | uplo: " << uplo << " | loup: " << loup << " |"<< endl;
		}
	}
}



// 2 methods for searching a better feasible point and a better loup

bool MainOpti::localsearch(const IntervalVector* box, int nb) {
	if (nb<=0) return false;

	bool loup_change=false;
	{
		Vector v_tmp(box[0].size());
		for (int i=0; i<nb;i++) {
			localopti.set_box(box[i]);
			UnconstrainedLocalSearch::ReturnCode code =
					localopti.minimize(box[i].random(),v_tmp,goal_rel_prec,100);
			if (code != UnconstrainedLocalSearch::INVALID_POINT) {
				Interval tmp = f_cost.eval(v_tmp);
				if (tmp.ub()<loup) {
					//update the loup
					pseudo_loup = tmp.ub();
					loup_point = v_tmp;
					loup_change = true;
				}
			}
		}
	}
	if (trace && loup_change) {
		int prec1=cout.precision();
		cout.precision(12);
		cout << "[localsearch]"  << " loup update " << pseudo_loup  << " loup point  " << loup_point << endl;
		cout.precision(prec1);
	}
	return loup_change;

}

bool MainOpti::localsearch(const IntervalVector box) {
	return localsearch(&box,1);
}

void MainOpti::report() const {

	if (timeout >0 &&  time >=timeout ) {
		cout << "time limit " << timeout << "s. reached " << endl;
	}
	// No solution found and optimization stopped with empty buffer  before the required precision is reached => means infeasible problem
	if (buffer->empty() && uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0))) {
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
void MainOpti::time_cells_report()  const{
	if (timeout >0 &&  time >=timeout ) {
		cout << "timeout" << timeout << "  " << uplo << " " << loup << " ";}
	else
		cout << time << " " ;
	cout << nb_cells << endl;
}


void MainOpti::report_perf()  const{

	double rel_prec;
	if (loup==POS_INFINITY)
		rel_prec= POS_INFINITY;
	else
		rel_prec=(loup-uplo)/(fabs(loup))-1.e-15;

	double abs_prec=loup-uplo-1.e-15;

	cout << (	((rel_prec <= goal_rel_prec)||
			(abs_prec <= goal_abs_prec)||
			((buffer->empty() && uplo_of_epsboxes == POS_INFINITY && loup==POS_INFINITY))||
			(uplo<-1.e300)
	)? " T & " : " F & " );

	cout << uplo << " & " << loup << " & ";
	cout <<  time << "  "<< endl ;
}

void MainOpti::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (timeout >0 &&  time >=timeout ) throw TimeOutException();
	Timer::start();
}

} // end namespace ibex
