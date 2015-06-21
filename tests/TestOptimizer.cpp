/* ============================================================================
 * I B E X - Optimizer Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestOptimizer.h"
#include "ibex_Optimizer.h"
#include "ibex_DefaultOptimizer.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

// true minimum is 0.
Status_Opti issue50(double init_loup, double prec) {
	SystemFactory f;
	const ExprSymbol& x=ExprSymbol::new_();
	f.add_var(x);
	f.add_ctr(x>=0);
	f.add_goal(x);

	System sys(f);
	DefaultOptimizer o(sys,prec,prec);

	IntervalVector init_box(1,Interval::ALL_REALS);
	return o.optimize(init_box,init_loup);
}

void TestOptimizer::issue50_1() {
	TEST_ASSERT(issue50(1e-10, 0.1)==NO_FEASIBLE_FOUND);
}

void TestOptimizer::issue50_2() {
	TEST_ASSERT(issue50(1e-10, 0)==SUCCESS);
}

void TestOptimizer::issue50_3() {
	TEST_ASSERT(issue50(-1e-10, 0.1)==NO_FEASIBLE_FOUND);
}

void TestOptimizer::issue50_4() {
	TEST_ASSERT(issue50(-1e-10, 0)==INFEASIBLE);
}


} // end namespace
