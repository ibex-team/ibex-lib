/* ============================================================================
 * I B E X - Expression comparison tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 22, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_CMP_H__
#define __TEST_EXPR_CMP_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExprCmp : public TestIbex {

public:
	TestExprCmp() {
		TEST_ADD(TestExprCmp::test01);
		TEST_ADD(TestExprCmp::test02);
		TEST_ADD(TestExprCmp::test03);
	}

	void test01();
	void test02();
	void test03();
};


} // namespace ibex
#endif // __TEST_EXPR_CMP_H__
