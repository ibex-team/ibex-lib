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

void TestOptimizer::issue50() {
	double prec=0.1;
	SystemFactory f;
	const ExprSymbol& x=ExprSymbol::new_();
	f.add_var(x);
	f.add_ctr(x>=0);
	f.add_goal(x);

	System sys(f);

	{
		DefaultOptimizer o(sys,prec,prec);

		double init_loup=1e-10; // something very close to 0

		IntervalVector init_box(1,Interval::ALL_REALS);

		// upperbounding with goal_prec=10% will remove everything
		TEST_ASSERT(o.optimize(init_box,init_loup)==Optimizer::NO_FEASIBLE_FOUND);
	}

	{
		DefaultOptimizer o(sys,prec,0);

		double init_loup=1e-10; // something very close to 0

		IntervalVector init_box(1,Interval::ALL_REALS);

		// upperbounding with goal_prec=0 will make the optimizer succeed
		TEST_ASSERT(o.optimize(init_box,init_loup)==Optimizer::SUCCESS);
	}

}


} // end namespace
