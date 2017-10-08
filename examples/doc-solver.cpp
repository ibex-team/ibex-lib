//============================================================================
//                                  I B E X                                   
// File        : doc-solver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Jan 17, 2013
//============================================================================

#include "ibex.h"

#ifndef _IBEX_WITH_OPTIM_
#error "You need the plugin Optim to run this example."
#endif

#ifndef IBEX_BENCHS_DIR
  #define IBEX_BENCHS_DIR "../plugins/solver/benchs"
#endif

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of IbexOpt.
 */

int main() {

	ofstream output;
	output.open ("doc-solver.txt");

	output << "================= this file is generated ==============" << endl;

	{
	output << "! [basic-solver-O]" << endl;
	//! [basic-solver-C]

	/* Create the function (x,y)->( ||(x,y)||-d,  ||(x,y)-(1,0)||-d ) */
	Variable x,y;
	double d=1.0;
	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d,
		                  sqrt(sqr(x-1.0)+sqr(y))-d));

	/* Create the system f(x,y)=0. */
	SystemFactory factory;
	factory.add_var(x);
	factory.add_var(y);
	factory.add_ctr(f(x,y)=0);
	System system(factory);

	/* Create the domain of variables */
	double init_box[][2] = { {-10,10},{-10,10} };
	IntervalVector box(2,init_box);

	/* Create a first contractor w.r.t f(x,y)=0 (forward-backward) */
	CtcFwdBwd fwdBwd(f);

	/* Create a second contractor (interval Newton) */
	CtcNewton newton(f);

	/* Compose the two contractors */
	CtcCompo compo(fwdBwd,newton);

	/* Create a round-robin bisection heuristic and set the
	 * precision of boxes to 0. */
	RoundRobin bisector(0);

	/* Create a "stack of boxes" (CellStack), which has the effect of
	 * performing a depth-first search. */
	CellStack buff;

	/* Vector precisions required on variables */
	Vector prec(2, 1e-07);

	/* Create a solver with the previous objects */
	Solver s(system, compo, bisector, buff, prec, prec);

	/* Run the solver */
	s.solve(box);

	/* Display the solutions */
	output << s.get_manifold() << endl;
	//! [basic-solver-C]
	output << "! [basic-solver-O]" << endl;
	}

	{
	output << "! [default-solver-O]" << endl;
	//! [default-solver-C]

	System system(IBEX_BENCHS_DIR "/others/kolev36.bch");

	/* ============================ building contractors ========================= */
	CtcHC4 hc4(system,0.01);

	CtcHC4 hc4_2(system,0.1,true);

	CtcAcid acid(system, hc4_2);

	CtcNewton newton(system.f_ctrs, 5e+08, 1e-07, 1e-04);

	LinearizerCombo linear_relax(system,LinearizerCombo::XNEWTON);

	CtcPolytopeHull polytope(linear_relax);

	CtcCompo polytope_hc4(polytope, hc4);

	CtcFixPoint fixpoint(polytope_hc4);

	CtcCompo compo(hc4,acid,newton,fixpoint);
	/* =========================================================================== */

	/* Create a smear-function bisection heuristic. */
	SmearSumRelative bisector(system, 1e-07);

	/* Create a "stack of boxes" (CellStack) (depth-first search). */
	CellStack buff;

	/* Vector precisions required on variables */
	Vector prec(2, 1e-07);

	/* Create a solver with the previous objects */
	Solver s(system, compo, bisector, buff, prec, prec);

	/* Run the solver */
	s.solve(system.box);

	/* Display the solutions */
	output << s.get_manifold() << endl;

	/* Report performances */
	output << "cpu time used=" << s.get_time() << "s."<< endl;
	output << "number of cells=" << s.get_nb_cells() << endl;

	//! [default-solver-C]
	output << "! [default-solver-O]" << endl;
	}


	return 0;
}
