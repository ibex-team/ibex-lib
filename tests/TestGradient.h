/* ============================================================================
 * I B E X - Diff Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_GRADIENT_H__
#define __TEST_GRADIENT_H__

#include "cpptest.h"
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestGradient : public TestIbex {

public:
	TestGradient() {
		TEST_ADD(TestGradient::deco01);
		TEST_ADD(TestGradient::deco02);

		TEST_ADD(TestGradient::add01);
		TEST_ADD(TestGradient::add02);
		TEST_ADD(TestGradient::add03);
		TEST_ADD(TestGradient::add04);
	}

	void deco01();
	void deco02();

	void add01();
	void add02();
	void add03();
	void add04();

private:
	void check_deco(const ExprNode& e);
};

} // end namespace

#endif // __TEST_GRADIENT_H__
