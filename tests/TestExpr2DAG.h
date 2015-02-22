/* ============================================================================
 * I B E X - Expression DAGification tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 22, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_2_DAG_H__
#define __TEST_EXPR_2_DAG_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExpr2DAG : public TestIbex {

public:
	TestExpr2DAG() {
		TEST_ADD(TestExpr2DAG::test01);
		TEST_ADD(TestExpr2DAG::test02);
		TEST_ADD(TestExpr2DAG::test03);
	}

	void test01();
	void test02();
	void test03();
};


} // namespace ibex
#endif // __TEST_EXPR_2_DAG_H__
