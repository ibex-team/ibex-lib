/* ============================================================================
 * I B E X -  Expression/Function serialization tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Oct 05, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_2_MINIBEX_H__
#define __TEST_EXPR_2_MINIBEX_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExpr2Minibex : public TestIbex {

public:
	TestExpr2Minibex() {
		TEST_ADD(TestExpr2Minibex::expr01);
		TEST_ADD(TestExpr2Minibex::expr02);
		TEST_ADD(TestExpr2Minibex::expr03);
		TEST_ADD(TestExpr2Minibex::expr04);
	}

	void expr01();
	void expr02();
	void expr03();
	void expr04();

};


} // namespace ibex
#endif // __TEST_EXPR_2_MINIBEX_H
