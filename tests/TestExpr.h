/* ============================================================================
 * I B E X - Expression Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_H__
#define __TEST_EXPR_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExpr : public TestIbex {

public:
	TestExpr() {
		TEST_ADD(TestExpr::symbol);
	}

	void symbol();
};

} // end namespace

#endif /* __TEST_FUNCTION_H__ */
