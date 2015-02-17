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

#ifndef __TEST_OPTIMIZER_H__
#define __TEST_OPTIMIZER_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestOptimizer : public TestIbex {

public:
	TestOptimizer() {

		TEST_ADD(TestOptimizer::issue50_1);
		TEST_ADD(TestOptimizer::issue50_2);
		TEST_ADD(TestOptimizer::issue50_3);
		TEST_ADD(TestOptimizer::issue50_4);
	}

	// upperbounding with goal_prec=10% will remove everything (initial loup > true minimum) --> NO_FEASIBLE_FOUND
	void issue50_1();
	// upperbounding with goal_prec=0 will make the optimizer succeed --> SUCCESS
	void issue50_2();
	// upperbounding with goal_prec=10% will make the optimizer fail (initial loup < true minimum) --> NO_FEASIBLE_FOUND
	void issue50_3();
	// upperbounding with goal_prec=0 will make the optimizer fail (initial loup < true minimum) --> INFEASIBLE
	void issue50_4();
};

} // namespace ibex
#endif // __TEST_OPTIMIZER_H__
