/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EVAL_H__
#define __TEST_EVAL_H__

#include "cpptest.h"
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestEval : public TestIbex {

public:
	TestEval() {
		TEST_ADD(TestEval::deco01);
		TEST_ADD(TestEval::deco02);

		TEST_ADD(TestEval::add01);
		TEST_ADD(TestEval::add02);
		TEST_ADD(TestEval::add03);
		TEST_ADD(TestEval::add04);

		TEST_ADD(TestEval::mul01);

		TEST_ADD(TestEval::dist01);
	}

	void deco01();
	void deco02();

	void add01();
	void add02();
	void add03();
	void add04();

	void mul01();

	void dist01();

private:
	void check_deco(const ExprNode& e);
};

} // end namespace

#endif /* __TEST_EVAL_H__ */
