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

#include "ibex_Interval.h"

namespace ibex {

class TestInnerArith : public TestIbex {

public:
	TestInnerArith() {

		TEST_ADD(TestInnerArith::add_sub01);
		TEST_ADD(TestInnerArith::add_sub01_2);
		TEST_ADD(TestInnerArith::add_sub01_3);
		TEST_ADD(TestInnerArith::add_sub02);
		TEST_ADD(TestInnerArith::add_sub02_2);
		TEST_ADD(TestInnerArith::add_sub02_3);
		TEST_ADD(TestInnerArith::add_sub03);
		TEST_ADD(TestInnerArith::add_sub03_2);
		TEST_ADD(TestInnerArith::add_sub04);
		TEST_ADD(TestInnerArith::add_sub04_2);
		TEST_ADD(TestInnerArith::add_sub04_3);

		TEST_ADD(TestInnerArith::add_sub05_1);
		TEST_ADD(TestInnerArith::add_sub05_2);
		TEST_ADD(TestInnerArith::add_sub05_3);
		TEST_ADD(TestInnerArith::add_sub05_4);
		TEST_ADD(TestInnerArith::add_sub05_5);
		TEST_ADD(TestInnerArith::add_sub05_6);
		TEST_ADD(TestInnerArith::add_sub05_7);
		TEST_ADD(TestInnerArith::add_sub05_8);
		TEST_ADD(TestInnerArith::add_sub05_9);
		TEST_ADD(TestInnerArith::add_sub05_10);
		TEST_ADD(TestInnerArith::add_sub05_11);


		TEST_ADD(TestInnerArith::add_sub06_1);
		TEST_ADD(TestInnerArith::add_sub06_2);

		TEST_ADD(TestInnerArith::add_sub07);
	}

	// x+y<=z with contraction (no inflation)
	void add_sub01();
	// x+y>=z with contraction (no inflation)
	void add_sub01_2();
	// x+y in [z] with contraction on both sides (no inflation)
	void add_sub01_3();

	// x+y<=z with all (x,y) solution (no inflation)
	void add_sub02();
	// x+y>=z with all (x,y) solution (no inflation)
	void add_sub02_2();
	// x+y in [z] with all (x,y) solution (no inflation)
	void add_sub02_3();

	// x+y<=z with no solution (no inflation)
	void add_sub03();
	// x+y>=z with no solution (no inflation)
	void add_sub03_2();

	// x+y<=z with only 1 pt solution (no inflation)
	void add_sub04();
	// x+y>=z with only 1 pt solution (no inflation)
	void add_sub04_2();
	// x+y in [z] with only 1 pt solution (no inflation)
	void add_sub04_3();

	// x+y<=z with contraction (+inflation)
	void add_sub05_1();
	// x+y>=z with contraction (+inflation)
	void add_sub05_2();
	// x+y in [z] with contraction +inflation xin>0,yin<0
	void add_sub05_3();
	// x+y in [z] with contraction +inflation xin<0,yin>0
	void add_sub05_4();
	// x+y in [z] with contraction +inflation xin>0,yin>0
	void add_sub05_5();
	// x+y in [z] with contraction +inflation xin<0,yin<0
	void add_sub05_6();
	// x+y in [z] with contraction +inflation xin=yin=0
	void add_sub05_7();
	// x+y in [z] with (xin,yin) at a corner xin>0, yin=0
	void add_sub05_8();
	// x+y in [z] with (xin,yin) at a corner xin<0, yin=0
	void add_sub05_9();
	// x+y in [z] with (xin,yin) at a corner xin=0, yin>0
	void add_sub05_10();
	// x+y in [z] with (xin,yin) at a corner xin=0, yin<0
	void add_sub05_11();

	// x+y<=z with (xin,yin) at a corner
	void add_sub06_1();
	// x+y>=z with (xin,yin) at a corner
	void add_sub06_2();

	// x+y=z
	void add_sub07();

private:
	void check_add_sub(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub);
};

} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
