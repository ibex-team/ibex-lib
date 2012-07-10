/* ============================================================================
 * I B E X - Inner Arithmetic Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the _3ms of the GNU LGPL.
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
		TEST_ADD(TestInnerArith::add01_2);
		TEST_ADD(TestInnerArith::add01_3);
		TEST_ADD(TestInnerArith::add02);
		TEST_ADD(TestInnerArith::add02_2);
		TEST_ADD(TestInnerArith::add02_3);
		TEST_ADD(TestInnerArith::add03);
		TEST_ADD(TestInnerArith::add03_2);
		TEST_ADD(TestInnerArith::add04);
		TEST_ADD(TestInnerArith::add04_2);
		TEST_ADD(TestInnerArith::add04_3);

		TEST_ADD(TestInnerArith::add05_1);
		TEST_ADD(TestInnerArith::add05_2);
		TEST_ADD(TestInnerArith::add05_3);
		TEST_ADD(TestInnerArith::add05_4);
		TEST_ADD(TestInnerArith::add05_5);
		TEST_ADD(TestInnerArith::add05_6);
		TEST_ADD(TestInnerArith::add05_7);
		TEST_ADD(TestInnerArith::add05_8);
		TEST_ADD(TestInnerArith::add05_9);
		TEST_ADD(TestInnerArith::add05_10);
		TEST_ADD(TestInnerArith::add05_11);


		TEST_ADD(TestInnerArith::add06_1);
		TEST_ADD(TestInnerArith::add06_2);

		TEST_ADD(TestInnerArith::add07);
	}

	// x+y<=z with contraction (no inflation)
	void add01();
	// x+y>=z with contraction (no inflation)
	void add01_2();
	// x+y in [z] with contraction on both sides (no inflation)
	void add01_3();

	// x+y<=z with all (x,y) solution (no inflation)
	void add02();
	// x+y>=z with all (x,y) solution (no inflation)
	void add02_2();
	// x+y in [z] with all (x,y) solution (no inflation)
	void add02_3();

	// x+y<=z with no solution (no inflation)
	void add03();
	// x+y>=z with no solution (no inflation)
	void add03_2();

	// x+y<=z with only 1 pt solution (no inflation)
	void add04();
	// x+y>=z with only 1 pt solution (no inflation)
	void add04_2();
	// x+y in [z] with only 1 pt solution (no inflation)
	void add04_3();

	// x+y<=z with contraction (+inflation)
	void add05_1();
	// x+y>=z with contraction (+inflation)
	void add05_2();
	// x+y in [z] with contraction +inflation xin>0,yin<0
	void add05_3();
	// x+y in [z] with contraction +inflation xin<0,yin>0
	void add05_4();
	// x+y in [z] with contraction +inflation xin>0,yin>0
	void add05_5();
	// x+y in [z] with contraction +inflation xin<0,yin<0
	void add05_6();
	// x+y in [z] with contraction +inflation xin=yin=0
	void add05_7();
	// x+y in [z] with (xin,yin) at a corner xin>0, yin=0
	void add05_8();
	// x+y in [z] with (xin,yin) at a corner xin<0, yin=0
	void add05_9();
	// x+y in [z] with (xin,yin) at a corner xin=0, yin>0
	void add05_10();
	// x+y in [z] with (xin,yin) at a corner xin=0, yin<0
	void add05_11();

	// x+y<=z with (xin,yin) at a corner
	void add06_1();
	// x+y>=z with (xin,yin) at a corner
	void add06_2();

	// x+y=z
	void add07();
};

} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
