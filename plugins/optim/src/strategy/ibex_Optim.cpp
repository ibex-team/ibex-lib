//                                  I B E X                                   
// File        : ibex_Optim.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : December 24, 2012
//============================================================================

#include "ibex_Optim.h"
#include <stdio.h>
#include "ibex_Timer.h"

namespace ibex {

const double Optim::default_prec = 1e-07;
const double Optim::default_goal_rel_prec = 1e-07;
const double Optim::default_goal_abs_prec = 1e-07;
const int    Optim::default_sample_size = 10;
const double Optim::default_equ_eps = 1e-08;
const double Optim::default_loup_tolerance = 0.1;
const double Optim::default_timeout = 86400; // timeout = 24h


Optim::Optim (int n, CellDoubleHeap* buffer,  double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size) :
                				n(n), buffer(buffer),
                				prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
                				sample_size(sample_size), trace(false),
                				timeout(default_timeout), time(0),
                				loup(POS_INFINITY),uplo(NEG_INFINITY),
                				loup_point(n), nb_cells(0),
                				loup_changed(false),	initial_loup(POS_INFINITY),
                				uplo_of_epsboxes(POS_INFINITY) {

	if (trace) std::cout.precision(12);
}

Optim::~Optim() {
}


// compute the value ymax (decreasing the loup with the precision)
// the heap and the current box are contracted with y <= ymax
double Optim::compute_ymax() {
	double ymax= loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;
	return ymax;}



void Optim::update_uplo() {
	double new_uplo=POS_INFINITY;

	if (! buffer->empty()) {
		new_uplo= buffer->minimum();
		if (new_uplo > loup) {
			std::cout << " loup = " << loup << " new_uplo=" << new_uplo << std::endl;
			ibex_error("Optim: new_uplo>loup (please report bug)");
		}
		if (new_uplo < uplo) {
			std::cout << "uplo= " << uplo << " new_uplo=" << new_uplo << std::endl;
			ibex_error("Optim: new_uplo<uplo (please report bug)");
		}

		if (new_uplo < uplo_of_epsboxes) {
			if (new_uplo > uplo) {
				//std::cout << "uplo update=" << uplo << std::endl;
				uplo = new_uplo;
			}
		}
		else uplo= uplo_of_epsboxes;
	}
	else if (buffer->empty() && loup != POS_INFINITY) {
		// empty buffer : new uplo is set to ymax (loup - precision) if a loup has been found
		new_uplo=compute_ymax(); // not new_uplo=loup, because constraint y <= ymax was enforced
		//    std::cout << " new uplo buffer empty " << new_uplo << " uplo " << uplo << std::endl;

		double m = (new_uplo< uplo_of_epsboxes)? new_uplo : uplo_of_epsboxes;
		if (uplo < m) uplo = m; // warning: hides the field "m" of the class
		// note: we always have uplo <= uplo_of_epsboxes but we may have uplo > new_uplo, because
		// ymax is strictly lower than the loup.
	}

}



void Optim::update_uplo_of_epsboxes(double ymin) {

	// the current box cannot be bisected.  ymin is a lower bound of the objective on this box
	// uplo of epsboxes can only go down, but not under uplo : it is an upperbound for uplo,
	//that indicates a lowerbound for the objective in all the small boxes
	// found by the precision criterion
	assert (uplo_of_epsboxes >= uplo);
	assert(ymin >= uplo);
	if (uplo_of_epsboxes > ymin) {
		uplo_of_epsboxes = ymin;
		if (trace) {
			std::cout << "uplo_of_epsboxes:" <<  uplo_of_epsboxes << " uplo " << uplo << std::endl;
		}
	}
}



void Optim::report() {

	if (timeout >0 &&  time >=timeout ) {
		std::cout << "time limit " << timeout << "s. reached " << std::endl;
	}
	// No solution found and optimization stopped with empty buffer  before the required precision is reached => means infeasible problem
	if (buffer->empty() && uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0))) {
		std::cout << " infeasible problem " << std::endl;
		std::cout << " cpu time used " << time << "s." << std::endl;
		std::cout << " number of cells " << nb_cells << std::endl;
	}

	else {
		std::cout << " best bound in: [" << uplo << "," << loup << "]" << std::endl;

		double rel_prec;

		if (loup==POS_INFINITY)
			rel_prec= POS_INFINITY;
		else
			rel_prec=(loup-uplo)/(fabs (loup))-1.e-15;

		double abs_prec=loup-uplo-1.e-15;

		std::cout << " Relative precision obtained on objective function: " << rel_prec << " " <<
				(rel_prec <= goal_rel_prec? " [passed]" : " [failed]") << "  " << goal_rel_prec <<  std::endl;

		std::cout << " Absolute precision obtained on objective function: " << abs_prec << " " <<
				(abs_prec <= goal_abs_prec? " [passed]" : " [failed]") << "  " << goal_abs_prec << std::endl;
		if (uplo_of_epsboxes != NEG_INFINITY && uplo_of_epsboxes != POS_INFINITY)
			std::cout << " precision on variable domains obtained " << prec << " "   << " uplo_of_epsboxes " << uplo_of_epsboxes << std::endl;
		else if (uplo_of_epsboxes == NEG_INFINITY)
			std::cout << " small boxes with negative infinity objective :  objective not bound " << std::endl;
		if (loup==initial_loup)
			std::cout << " no feasible point found " << std::endl;
		else
			std::cout << " best feasible point " << loup_point << std::endl;


		std::cout << " cpu time used " << time << "s." << std::endl;
		std::cout << " number of cells " << nb_cells << std::endl;
	}
	/*   // statistics on upper bounding
    if (trace) {
      std::cout << " nbrand " << nb_rand << " nb_inhc4 " << nb_inhc4 << " nb simplex " << nb_simplex << std::endl;
      std::cout << " diam_rand " << diam_rand << " diam_inhc4 " << diam_inhc4 << " diam_simplex " << diam_simplex << std::endl;
    }
	 */
}
/* minimal report for benchmarking */
void Optim::time_cells_report() {
	if (timeout >0 &&  time >=timeout ) {
		std::cout << "timeout" << timeout << "  " << uplo << " " << loup << " ";}
	else
		std::cout << time << " " ;
	std::cout << nb_cells << std::endl;
}


void Optim::report_perf() {

	double rel_prec;
	if (loup==POS_INFINITY)
		rel_prec= POS_INFINITY;
	else
		rel_prec=(loup-uplo)/(fabs(loup))-1.e-15;

	double abs_prec=loup-uplo-1.e-15;

	std::cout << (	((rel_prec <= goal_rel_prec)||
			(abs_prec <= goal_abs_prec)||
			((buffer->empty() && uplo_of_epsboxes == POS_INFINITY && loup==POS_INFINITY))||
			(uplo<-1.e300)
	)? " T & " : " F & " );

	std::cout << uplo << " & " << loup << " & ";
	std::cout <<  time << "  "<< std::endl ;
}

void Optim::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (timeout >0 &&  time >=timeout ) throw TimeOutException();
	Timer::start();
}

} // end namespace ibex
