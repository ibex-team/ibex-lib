/* ============================================================================
 * I B E X - Test Split occurrences
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 18, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_SPLIT_OCC_H__
#define __TEST_EXPR_SPLIT_OCC_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExprSplitOcc : public TestIbex {

public:
	TestExprSplitOcc() {
		TEST_ADD(TestExprSplitOcc::test01);
		TEST_ADD(TestExprSplitOcc::test02);
	}

	void test01();
	void test02();
};

} // namespace ibex
#endif // __TEST_EXPR_SPLIT_OCC_H__
