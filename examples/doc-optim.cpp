//============================================================================
//                                  I B E X                                   
// File        : doc-optim.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jan 17, 2013
// Last Update : Oct 08, 2017
//============================================================================

#include "ibex.h"

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IbexOpt plugin to run this example."
#endif

#define IBEX_OPTIM_BENCHS_DIR "../plugins/optim/benchs"

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of the tutorial.
 */

int main() {

  ofstream output;
  output.open ("doc-optim.txt");

  output << "================= this file is generated ==============" << endl;

  {
  output << "! [optim-call-default-O]" << endl;
  //! [optim-call-default-C]

  /* Build a constrained optimization problem from the file */
  System sys(IBEX_OPTIM_BENCHS_DIR "/easy/ex3_1_3.bch");

  /* Build a default optimizer with a precision set to 1e-07 for f(x) */
  DefaultOptimizer o(sys,1e-07);

  o.optimize(sys.box);// Run the optimizer

  /* Display the result. */
  output << "interval for the minimum: " << Interval(o.get_uplo(),o.get_loup()) << endl;
  output << "minimizer: " << o.get_loup_point() << endl;
  //! [optim-call-default-C]
  output << "! [optim-call-default-O]" << endl;
  }

  {
  output << "! [optim-implem-default-O]" << endl;
  //! [optim-implem-default-C]

  System system(IBEX_OPTIM_BENCHS_DIR "/easy/ex3_1_3.bch");

  double prec=1e-7; // precision

  // normalized system (all inequalities are "<=")
  NormalizedSystem norm_sys(system);

  // extended system (the objective is transformed to a constraint y=f(x))
  ExtendedSystem ext_sys(system);

  /* ============================ building contractors ========================= */
  CtcHC4 hc4(ext_sys,0.01);

  CtcHC4 hc4_2(ext_sys,0.1,true);

  CtcAcid acid(ext_sys, hc4_2);

  LinearizerCombo linear_relax(ext_sys,LinearizerCombo::XNEWTON);

  CtcPolytopeHull polytope(linear_relax);

  CtcCompo polytope_hc4(polytope, hc4);

  CtcFixPoint fixpoint(polytope_hc4);

  CtcCompo compo(hc4,acid,fixpoint);
  /* =========================================================================== */

  /* Create a smear-function bisection heuristic. */
  SmearSumRelative bisector(ext_sys, prec);

  /** Create cell buffer (fix exploration ordering) */
  CellDoubleHeap buffer(ext_sys);

  /** Create a "loup" finder (find feasible points) */
  LoupFinderDefault loup_finder(norm_sys);

  /* Create a solver with the previous objects */
  Optimizer o(system.nb_var, compo, bisector, loup_finder, buffer, ext_sys.goal_var());

  /* Run the optimizer */
  o.optimize(system.box,prec);

  /* Display a safe enclosure of the minimum */
  output << "f* in " << Interval(o.get_uplo(),o.get_loup()) << endl;

  /* Report performances */
  output << "cpu time used=" << o.get_time() << "s."<< endl;
  output << "number of cells=" << o.get_nb_cells() << endl;

  //! [optim-implem-default-C]
  output << "! [optim-implem-default-O]" << endl;
  }

  {
  output << "! [optim-all-minima-O]" << endl;
  //! [optim-all-minima-C]
  // ========== 1st step ==========
  // Build the original system: 
  Variable x,y;
  SystemFactory opt_fac;
  opt_fac.add_var(x);
  opt_fac.add_var(y);
  // minimize f(x)=-x-y
  opt_fac.add_goal(-x-y);
  // s.t. x^2+y^2<=1
  opt_fac.add_ctr(sqr(x)+sqr(y)<=1);
  System opt_sys(opt_fac);
  // build the default optimizer
  DefaultOptimizer optimizer(opt_sys,1e-01);
  // run it with (x,y) in R^2
  optimizer.optimize(IntervalVector(2));

  // ========== 2nd step ==========
  // Build the auxiliary system
  SystemFactory sol_fac;
  sol_fac.add_var(x);
  sol_fac.add_var(y);
  sol_fac.add_ctr(sqr(x)+sqr(y)<=1);
  sol_fac.add_ctr(-x-y>=optimizer.get_uplo());
  sol_fac.add_ctr(-x-y<=optimizer.get_loup());
  System sol_sys(sol_fac);
  DefaultSolver solver(sol_sys,0.01);
  solver.solve(IntervalVector(2));
  // Get an enclosure of all minima, (under
  // the form of manifold)
  const Manifold& minima=solver.get_data();
  //! [optim-all-minima-C]
  output << "! [optim-all-minima-O]" << endl;
  minima.write("doc-optim-all-minima.mnf");
  }

  return 0;
}
