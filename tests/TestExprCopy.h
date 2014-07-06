/* ============================================================================
 * I B E X - Symbolic copy tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_COPY_H__
#define __TEST_EXPR_COPY_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExprCopy : public TestIbex {

public:
	TestExprCopy() {

		TEST_ADD(TestExprCopy::index_copy01);
	}

	// case where the vector is a constant vector
	void index_copy01();

};

} // namespace ibex
#endif // __TEST_EXPR_COPY_H__
