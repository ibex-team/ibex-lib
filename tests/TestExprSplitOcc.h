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
		TEST_ADD(TestExprSplitOcc::test03);
		TEST_ADD(TestExprSplitOcc::test04);
		TEST_ADD(TestExprSplitOcc::test05);
		TEST_ADD(TestExprSplitOcc::test07);
	}

	// two occurrences of scalar var
	void test01();
	// several scalar vars
	void test02();
	// vector var: one occurrence for the whole vector, one with index
	void test03();
	// vector var: same index node with several fathers
	void test04();
	// vector var: several occurrences of an index (but different nodes)
	void test05();
	// vector var: only indices
	void test06();
	// vector var: more complex example
	void test07();
};

} // namespace ibex
#endif // __TEST_EXPR_SPLIT_OCC_H__
