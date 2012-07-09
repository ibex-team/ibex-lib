/* ============================================================================
 * I B E X - Inner Arithmetic Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jul 06, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_INNER_ARITH_H__
#define __TEST_INNER_ARITH_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestInnerArith : public TestIbex {

public:
	TestInnerArith() {

		TEST_ADD(TestInnerArith::add01);
		TEST_ADD(TestInnerArith::add02);
		TEST_ADD(TestInnerArith::add03);
		TEST_ADD(TestInnerArith::add04);
		TEST_ADD(TestInnerArith::add05);
		TEST_ADD(TestInnerArith::add06);

		TEST_ADD(TestInnerArith::add01bis);
		TEST_ADD(TestInnerArith::add02bis);
		TEST_ADD(TestInnerArith::add03bis);
		TEST_ADD(TestInnerArith::add04bis);
		TEST_ADD(TestInnerArith::add05bis);
		TEST_ADD(TestInnerArith::add06bis);
	}

	// x+y<=z with contraction (no inflation)
	void add01();
	// x+y>=z with contraction (no inflation)
	void add01bis();
	// x+y<=z with all (x,y) solution (no inflation)
	void add02();
	// x+y>=z with all (x,y) solution (no inflation)
	void add02bis();
	// x+y<=z with no solution (no inflation)
	void add03();
	// x+y>=z with no solution (no inflation)
	void add03bis();
	// x+y<=z with only 1 pt solution (no inflation)
	void add04();
	// xx+y>=z with only 1 pt solution (no inflation)
	void add04bis();
	// x+y<=z with contraction (+inflation)
	void add05();
	// x+y>=z with contraction (+inflation)
	void add05bis();
	// x+y<=z with (xin,yin) at a corner
	void add06();
	// x+y>=z with (xin,yin) at a corner
	void add06bis();
};

} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
