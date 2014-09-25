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

		TEST_ADD(TestOptimizer::issue50);
	}

	void issue50();
};

} // namespace ibex
#endif // __TEST_OPTIMIZER_H__
