//============================================================================
//                                  I B E X
// File        : doc-covfiles.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 29, 2018
//============================================================================

#include "ibex.h"

#ifndef _IBEX_WITH_SOLVER_
#error "You need to install the IbexSolve plugin (--with-solver)."
#endif

using namespace std;
using namespace ibex;

/**
 * This file contains examples of the documentation.
 */

int main() {

ofstream output;
output.open ("doc-covfiles.txt");

output << "================= this file is generated ==============" << endl;

{
//! [cov-ex1-build-C]
// create a simple inequality x^2+y^2<=1.
const ExprSymbol& x=ExprSymbol::new_("x");
const ExprSymbol& y=ExprSymbol::new_("y");
SystemFactory fac;
fac.add_var(x);
fac.add_var(y);
fac.add_ctr(sqr(x)+sqr(y)<=1);

System sys(fac);

// run the solver with a coarse precision (1.0)
// to have a limited number of boxes
DefaultSolver solver(sys,1);

// we set a very short storage capacity
// to have a partial solving only.
solver.cell_limit = 12;

solver.solve(sys.box);
//! [cov-ex1-build-C]

{
output << "! [cov-ex1-list-O]" << endl;
//! [cov-ex1-list-C]
const CovList& cov = solver.get_data();
output << "The list contains " << cov.size() << " boxes" << endl;
for (size_t i=0; i<cov.size(); i++) {
  output << "box n°" << i << " = " << cov[i] << endl;
}
//! [cov-ex1-list-C]
output << "! [cov-ex1-list-O]" << endl;
}

{
output << "! [cov-ex1-IUlist-O]" << endl;
//! [cov-ex1-IUlist-C]
const CovIUList& cov = solver.get_data();
output << "Inner boxes:" << endl;
for (size_t i=0; i<cov.nb_inner(); i++) {
  output << "inner n°" << i << " = " << cov.inner(i) << endl;
}
output << endl << "Unknown boxes:" << endl;
for (size_t i=0; i<cov.nb_unknown(); i++) {
  output << "unknown n°" << i << " = " << cov.unknown(i) << endl;
}
//! [cov-ex1-IUlist-C]
output << "! [cov-ex1-IUlist-O]" << endl;
}

{
output << "! [cov-ex1-SolverData-O]" << endl;
//! [cov-ex1-SolverData-C]
const CovSolverData& cov = solver.get_data();
output << "Inner boxes:" << endl;
for (size_t i=0; i<cov.nb_inner(); i++) {
  output << "inner n°" << i << " = " << cov.inner(i) << endl;
}
output << endl << "Unknown boxes:" << endl;
for (size_t i=0; i<cov.nb_unknown(); i++) {
  output << "unknown n°" << i << " = " << cov.unknown(i) << endl;
}
output << endl << "Pending boxes:" << endl;
for (size_t i=0; i<cov.nb_pending(); i++) {
  output << "Pending n°" << i << " = " << cov.pending(i) << endl;
}
//! [cov-ex1-SolverData-C]
output << "! [cov-ex1-SolverData-O]" << endl;
}
}

{
const ExprSymbol& x=ExprSymbol::new_("x");
const ExprSymbol& y=ExprSymbol::new_("y");
SystemFactory fac;
fac.add_var(x);
fac.add_var(y);
fac.add_ctr(sqr(x)+sqr(y)<=1);

System sys(fac);

//! [cov-ex2-solver-C]

// solve the problem with boundary boxes
// of diameter < 0.1
DefaultSolver solver(sys,0.1,0.1);
solver.solve(sys.box);

//! [cov-ex2-solver-C]

//! [cov-ex2-optim-C]

SystemFactory opt_fac;
opt_fac.add_var(x);
opt_fac.add_var(y);
opt_fac.add_goal(x+y);

System opt(opt_fac);
//! [cov-ex2-optim-C]

output << "! [cov-ex2-run-O]" << endl;
//! [cov-ex2-run-C]
DefaultOptimizer optim(opt);

// run the optimizer with solver data as input covering:
optim.optimize(solver.get_data());

output << " best bound=" << optim.get_loup() << endl;

//! [cov-ex2-run-C]
output << "! [cov-ex2-run-O]" << endl;

}


{
//! [cov-ex3-optim-sys-C]
// build the problem "minimize |x^2+y^2-1|"
// -------------------------------------
Function g("x","y","abs(x^2+y^2-[0.9,1.1])");
SystemFactory opt_fac;
opt_fac.add_var(g.args());
opt_fac.add_goal(g);
System opt(opt_fac);

// we need bounded domain for unconstrained optimization:
opt.box[0]=Interval(-100,100);
opt.box[1]=Interval(-100,100);
// -------------------------------------
//! [cov-ex3-optim-sys-C]

//! [cov-ex3-optim-run-C]
// solve it
DefaultOptimizer optim(opt); // 1 is a very coarse precision
optim.timeout = 1;
optim.extended_COV = false;
optim.optimize(opt.box);
//! [cov-ex3-optim-run-C]

//! [cov-ex3-solver-sys-C]
// Build the system "solve x+y=0"
// -------------------------------------
Function f("x","y","x-y");
NumConstraint c(f);
SystemFactory sys_fac;
sys_fac.add_var(g.args());
sys_fac.add_ctr(c);
System sys(sys_fac);
// -------------------------------------
//! [cov-ex3-solver-sys-C]

output << "! [cov-ex3-solver-run-O]" << endl;
//! [cov-ex3-solver-run-C]
// solve the problem
DefaultSolver solver(sys,0.1,0.1);
solver.solve(optim.get_data());
output << solver.get_data() << endl;

//! [cov-ex3-solver-run-C]
output << "! [cov-ex3-solver-run-O]" << endl;
}

}
