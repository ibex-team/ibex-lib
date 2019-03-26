//============================================================================
//                                  I B E X                                   
// File        : doc-solver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jan 17, 2013
// Last Update : Oct 08, 2017
//============================================================================

#include "ibex.h"

#ifndef _IBEX_WITH_SOLVER_
#error "You need the IbexSolve plugin to run this example."
#endif

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IbexOpt plugin to run this example."
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
	output << "! [solver-call-default-O]" << endl;
	//! [solver-call-default-C]
	/* Build a system of equations from the file */
	System system(IBEX_BENCHS_DIR "/others/kolev36.bch");

	/* Build a default solver for the system and with a precision set to 1e-07 */
	DefaultSolver solver(system,1e-07);

	solver.solve(system.box); // Run the solver

	/* Display the solutions. */
	output << solver.get_data() << endl;
	//! [solver-call-default-C]
	output << "! [solver-call-default-O]" << endl;
	}

	{
	output << "! [solver-solver-generic-O]" << endl;
	//! [solver-solver-generic-C]

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
	Vector prec(6, 1e-07);

	/* Create a solver with the previous objects */
	Solver s(system, compo, bisector, buff, prec, prec);

	/* Run the solver */
	s.solve(box);

	/* Display the solutions */
	output << s.get_data() << endl;
	//! [solver-solver-generic-C]
	output << "! [solver-solver-generic-O]" << endl;
	}

	{
	output << "! [solver-implem-default-O]" << endl;
	//! [solver-implem-default-C]

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
	Vector prec(6, 1e-07);

	/* Create a solver with the previous objects */
	Solver s(system, compo, bisector, buff, prec, prec);

	/* Run the solver */
	s.solve(system.box);

	/* Display the solutions */
	output << s.get_data() << endl;

	/* Report performances */
	output << "cpu time used=" << s.get_time() << "s."<< endl;
	output << "number of cells=" << s.get_nb_cells() << endl;

	//! [solver-implem-default-C]
	output << "! [solver-implem-default-O]" << endl;
	}

	{
	//! [solver-parallel-C-1]

	// Get the system
	System sys1(IBEX_BENCHS_DIR "/polynom/ponts-geo.bch");

	// Create a copy for the second solver
	System sys2(sys1,System::COPY);

	// Precision of the solution boxes
	double prec=1e-08;

	// Create two solvers
	DefaultSolver solver1(sys1,prec);
	DefaultSolver solver2(sys2,prec);

	// Create a partition of the initial box into two subboxes,
	// by bisecting any variable (here, n°4)
	pair<IntervalVector,IntervalVector> pair=sys1.box.bisect(4);

	// =======================================================
	// Run the solvers in parallel
	// =======================================================
#pragma omp parallel sections
	{
		solver1.solve(pair.first);
#pragma omp section
		solver2.solve(pair.second);
	}
	// =======================================================

	output << "solver #1 found " << solver1.get_data().size() << endl;
	output << "solver #2 found " << solver2.get_data().size() << endl;
	//! [solver-parallel-C-1]

	{
	//! [solver-parallel-C-2]
	Vector eps_min(sys1.nb_var,prec);
	Vector eps_max(sys1.nb_var,POS_INFINITY);
	Solver solver1(sys1,*new CtcCompo(*new CtcHC4(sys1),*new CtcNewton(sys1.f_ctrs)), *new RoundRobin(prec), *new CellStack(), eps_min, eps_max);
	Solver solver2(sys2,*new CtcCompo(*new CtcHC4(sys2),*new CtcNewton(sys2.f_ctrs)), *new RoundRobin(prec), *new CellStack(), eps_min, eps_max);
	//! [solver-parallel-C-2]
	}
	}

	return 0;
}
