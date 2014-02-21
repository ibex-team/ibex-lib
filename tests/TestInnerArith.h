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

		TEST_ADD(TestInnerArith::mul_div_mono01);
		TEST_ADD(TestInnerArith::mul_div_mono01_2);
		TEST_ADD(TestInnerArith::mul_div_mono01_3);
		TEST_ADD(TestInnerArith::mul_div_mono02);
		TEST_ADD(TestInnerArith::mul_div_mono02_2);
		TEST_ADD(TestInnerArith::mul_div_mono02_3);
		TEST_ADD(TestInnerArith::mul_div_mono03);
		TEST_ADD(TestInnerArith::mul_div_mono03_2);
		TEST_ADD(TestInnerArith::mul_div_mono04);
		TEST_ADD(TestInnerArith::mul_div_mono04_2);
		TEST_ADD(TestInnerArith::mul_div_mono04_3);

		TEST_ADD(TestInnerArith::mul_div_mono05_1);
		TEST_ADD(TestInnerArith::mul_div_mono05_2);
		TEST_ADD(TestInnerArith::mul_div_mono05_3);
		TEST_ADD(TestInnerArith::mul_div_mono05_4);
		TEST_ADD(TestInnerArith::mul_div_mono05_5);
		TEST_ADD(TestInnerArith::mul_div_mono05_6);
		TEST_ADD(TestInnerArith::mul_div_mono05_7);
		TEST_ADD(TestInnerArith::mul_div_mono05_8);
		TEST_ADD(TestInnerArith::mul_div_mono05_9);
		TEST_ADD(TestInnerArith::mul_div_mono05_10);
		TEST_ADD(TestInnerArith::mul_div_mono05_11);


		TEST_ADD(TestInnerArith::mul_div_mono06_1);
		TEST_ADD(TestInnerArith::mul_div_mono06_2);

		TEST_ADD(TestInnerArith::mul_div_mono07);

		TEST_ADD(TestInnerArith::mul08);
		TEST_ADD(TestInnerArith::mul09);
		TEST_ADD(TestInnerArith::mul10);
		TEST_ADD(TestInnerArith::mul11);
		TEST_ADD(TestInnerArith::mul12_1);
		TEST_ADD(TestInnerArith::mul12_2);
		TEST_ADD(TestInnerArith::mul12_3);
		TEST_ADD(TestInnerArith::mul12_4);
		TEST_ADD(TestInnerArith::mul13_1);
		TEST_ADD(TestInnerArith::mul13_2);
		TEST_ADD(TestInnerArith::mul13_3);
		TEST_ADD(TestInnerArith::mul13_4);
		TEST_ADD(TestInnerArith::mul14_1);
		TEST_ADD(TestInnerArith::mul14_2);
		TEST_ADD(TestInnerArith::mul14_3);

		TEST_ADD(TestInnerArith::div08_1);
		TEST_ADD(TestInnerArith::div08_2);
		TEST_ADD(TestInnerArith::div09_1);
		TEST_ADD(TestInnerArith::div09_2);

		TEST_ADD(TestInnerArith::abs01);
		TEST_ADD(TestInnerArith::abs02);
		TEST_ADD(TestInnerArith::abs03);
		TEST_ADD(TestInnerArith::abs04);
		TEST_ADD(TestInnerArith::abs05);

		TEST_ADD(TestInnerArith::sqrt01);
		TEST_ADD(TestInnerArith::sqrt02);
		TEST_ADD(TestInnerArith::sqrt03);
		TEST_ADD(TestInnerArith::sqrt04);
		TEST_ADD(TestInnerArith::sqrt05);
		TEST_ADD(TestInnerArith::bugr894);
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

	// x*y<=z with contraction (no inflation)
	void mul_div_mono01();
	// x*y>=z with contraction (no inflation)
	void mul_div_mono01_2();
	// x*y in [z] with contraction on both sides (no inflation)
	void mul_div_mono01_3();

	// x*y<=z with all (x,y) solution (no inflation)
	void mul_div_mono02();
	// x*y>=z with all (x,y) solution (no inflation)
	void mul_div_mono02_2();
	// x*y in [z] with all (x,y) solution (no inflation)
	void mul_div_mono02_3();

	// x*y<=z with no solution (no inflation)
	void mul_div_mono03();
	// x*y>=z with no solution (no inflation)
	void mul_div_mono03_2();

	// x*y<=z with only 1 pt solution (no inflation)
	void mul_div_mono04();
	// x*y>=z with only 1 pt solution (no inflation)
	void mul_div_mono04_2();
	// x*y in [z] with only 1 pt solution (no inflation)
	void mul_div_mono04_3();

	// x*y<=z with contraction (*inflation)
	void mul_div_mono05_1();
	// x*y>=z with contraction (*inflation)
	void mul_div_mono05_2();
	// x*y in [z] with contraction *inflation xin>1,yin<1
	void mul_div_mono05_3();
	// x*y in [z] with contraction *inflation xin<1,yin>1
	void mul_div_mono05_4();
	// x*y in [z] with contraction *inflation xin>1,yin>1
	void mul_div_mono05_5();
	// x*y in [z] with contraction *inflation xin<1,yin<1
	void mul_div_mono05_6();
	// x*y in [z] with contraction *inflation xin=yin=1
	void mul_div_mono05_7();
	// x*y in [z] with (xin,yin) at a corner xin>1, yin=1
	void mul_div_mono05_8();
	// x*y in [z] with (xin,yin) at a corner xin<1, yin=1
	void mul_div_mono05_9();
	// x*y in [z] with (xin,yin) at a corner xin=1, yin>1
	void mul_div_mono05_10();
	// x*y in [z] with (xin,yin) at a corner xin=1, yin<1
	void mul_div_mono05_11();

	// x*y<=z with (xin,yin) at a corner
	void mul_div_mono06_1();
	// x*y>=z with (xin,yin) at a corner
	void mul_div_mono06_2();

	// x*y=z
	void mul_div_mono07();

	/*================================= non monotonous =========================================== */
	// x*y<=z with z>0 non inflating
	void mul08();
	// x*y>=z with z>0, with only 1 possible quadrant (x>0, y>0), non inflating
	void mul09();
	// x*y>=z with z>0, with only 1 possible quadrant (x<0, y<0), non inflating
	void mul10();
	// x*y>=z with z>0, with 2 possible quadrants, non inflating
	void mul11();

	// x*y in [z], non inflating with all corners impacted
	void mul12_1();
	// x*y in [z], non inflating with lower-left/upper right corners impacted
	void mul12_2();
	// x*y in [z], non inflating with lower-right/upper-left corners impacted
	void mul12_3();
	// x*y in [z], non inflating with all the box inner
	void mul12_4();

	// x*y in [z], inflating a box xin>0 yin>0
	void mul13_1();
	// x*y in [z], inflating a box xin>0 yin<0
	void mul13_2();
	// x*y in [z], inflating a box xin<0 yin>0
	void mul13_3();
	// x*y in [z], inflating a box xin<0 yin<0
	void mul13_4();

	// x*y with y=0 and 0 in [z]
	void mul14_1();
	// x*y with y=0 and [z]<>0
	void mul14_2();
	// x*y with y=0 and [z]=0
	void mul14_3();


	// x/y<=z with z>0 non inflating,with only possible positive half-plane
	void div08_1();
	// x/y<=z with z>0 non inflating,with only possible negative half-plane
	void div08_2();
	// x/y<=z with z<0 non inflating,with only possible positive half-plane
	void div09_1();
	// x/y<=z with z<0 non inflating,with only possible negative half-plane
	void div09_2();

	void abs01();
	void abs02();
	void abs03();
	void abs04();
	void abs05();

	void sqrt01();
	void sqrt02();
	void sqrt03();
	void sqrt04();
	void sqrt05();

	// bug in release r893 (fixed in r894).
	void bugr894();

private:
	void check_add_sub(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub);
	void check_mul_div_mono(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub);
	void check_mul_div(const Interval& z, const Interval& xin, const Interval& yin, bool ll, bool lr, bool ul, bool ur);
};

} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
