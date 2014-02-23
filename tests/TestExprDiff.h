/* ============================================================================
 * I B E X - Symbolic diff tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_DIFF_H__
#define __TEST_EXPR_DIFF_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExprDiff : public TestIbex {

public:
	TestExprDiff() {
		TEST_ADD(TestExprDiff::linear01);
		TEST_ADD(TestExprDiff::poly01);
		TEST_ADD(TestExprDiff::vec01);
		TEST_ADD(TestExprDiff::vec02);
		TEST_ADD(TestExprDiff::one_var_one_func);
		TEST_ADD(TestExprDiff::mat01);
	}

	void linear01();
	void poly01();
	void vec01();
	void vec02();
	void mat01();
	void one_var_one_func();
};

} // namespace ibex
#endif // __TEST_EXPR_DIFF_H__
