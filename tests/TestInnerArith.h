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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

#include "ibex_Interval.h"

namespace ibex {

class TestInnerArith : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestInnerArith);
	

		CPPUNIT_TEST(add_sub01);
		CPPUNIT_TEST(add_sub01_2);
		CPPUNIT_TEST(add_sub01_3);
		CPPUNIT_TEST(add_sub02);
		CPPUNIT_TEST(add_sub02_2);
		CPPUNIT_TEST(add_sub02_3);
		CPPUNIT_TEST(add_sub03);
		CPPUNIT_TEST(add_sub03_2);
		CPPUNIT_TEST(add_sub04);
		CPPUNIT_TEST(add_sub04_2);
		CPPUNIT_TEST(add_sub04_3);

		CPPUNIT_TEST(add_sub05_1);
		CPPUNIT_TEST(add_sub05_2);
		CPPUNIT_TEST(add_sub05_3);
		CPPUNIT_TEST(add_sub05_4);
		CPPUNIT_TEST(add_sub05_5);
		CPPUNIT_TEST(add_sub05_6);
		CPPUNIT_TEST(add_sub05_7);
		CPPUNIT_TEST(add_sub05_8);
		CPPUNIT_TEST(add_sub05_9);
		CPPUNIT_TEST(add_sub05_10);
		CPPUNIT_TEST(add_sub05_11);

		CPPUNIT_TEST(add_sub06_1);
		CPPUNIT_TEST(add_sub06_2);

		CPPUNIT_TEST(add_sub07);

		CPPUNIT_TEST(mul_div_mono01);
		CPPUNIT_TEST(mul_div_mono01_2);
		CPPUNIT_TEST(mul_div_mono01_3);
		CPPUNIT_TEST(mul_div_mono02);
		CPPUNIT_TEST(mul_div_mono02_2);
		CPPUNIT_TEST(mul_div_mono02_3);
		CPPUNIT_TEST(mul_div_mono03);
		CPPUNIT_TEST(mul_div_mono03_2);
		CPPUNIT_TEST(mul_div_mono04);
		CPPUNIT_TEST(mul_div_mono04_2);
		CPPUNIT_TEST(mul_div_mono04_3);

		CPPUNIT_TEST(mul_div_mono05_1);
		CPPUNIT_TEST(mul_div_mono05_2);
		CPPUNIT_TEST(mul_div_mono05_3);
		CPPUNIT_TEST(mul_div_mono05_4);
		CPPUNIT_TEST(mul_div_mono05_5);
		CPPUNIT_TEST(mul_div_mono05_6);
		CPPUNIT_TEST(mul_div_mono05_7);
		CPPUNIT_TEST(mul_div_mono05_8);
		CPPUNIT_TEST(mul_div_mono05_9);
		CPPUNIT_TEST(mul_div_mono05_10);
		CPPUNIT_TEST(mul_div_mono05_11);


		CPPUNIT_TEST(mul_div_mono06_1);
		CPPUNIT_TEST(mul_div_mono06_2);

		CPPUNIT_TEST(mul_div_mono07);

		CPPUNIT_TEST(mul08);
		CPPUNIT_TEST(mul09);
		CPPUNIT_TEST(mul10);
		CPPUNIT_TEST(mul11);
		CPPUNIT_TEST(mul12_1);
		CPPUNIT_TEST(mul12_2);
		CPPUNIT_TEST(mul12_3);
		CPPUNIT_TEST(mul12_4);
		CPPUNIT_TEST(mul13_1);
		CPPUNIT_TEST(mul13_2);
		CPPUNIT_TEST(mul13_3);
		CPPUNIT_TEST(mul13_4);
		CPPUNIT_TEST(mul14_1);
		CPPUNIT_TEST(mul14_2);
		CPPUNIT_TEST(mul14_3);

		CPPUNIT_TEST(div08_1);
		CPPUNIT_TEST(div08_2);
		CPPUNIT_TEST(div09_1);
		CPPUNIT_TEST(div09_2);

		CPPUNIT_TEST(maxmin01);
		CPPUNIT_TEST(maxmin02);
		CPPUNIT_TEST(maxmin03);
		CPPUNIT_TEST(maxmin04);
		CPPUNIT_TEST(maxmin05);
		CPPUNIT_TEST(maxmin06);
		CPPUNIT_TEST(maxmin07);

		CPPUNIT_TEST(abs01);
		CPPUNIT_TEST(abs02);
		CPPUNIT_TEST(abs03);
		CPPUNIT_TEST(abs04);
		CPPUNIT_TEST(abs05);

		CPPUNIT_TEST(sqrt01);
		CPPUNIT_TEST(sqrt02);
		CPPUNIT_TEST(sqrt03);
		CPPUNIT_TEST(sqrt04);
		CPPUNIT_TEST(sqrt05);

		CPPUNIT_TEST(pow01);


		CPPUNIT_TEST(bugr894);
		CPPUNIT_TEST(bugr899);
		CPPUNIT_TEST(bugr902);

		CPPUNIT_TEST(imul01);
		CPPUNIT_TEST(imul02);
		CPPUNIT_TEST(imul03);
		CPPUNIT_TEST(imul04);
		CPPUNIT_TEST(imul05);
		CPPUNIT_TEST(imul06);
		CPPUNIT_TEST(imul07);
		CPPUNIT_TEST(imul08);
		CPPUNIT_TEST(imul09);
		CPPUNIT_TEST(imul10);
	CPPUNIT_TEST_SUITE_END();

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

	void maxmin01();
	void maxmin02();
	void maxmin03();
	void maxmin04();
	void maxmin05();
	void maxmin06();
	void maxmin07();

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

    // y=x^3 with y in[-oo,0] and x in [-oo,oo]
	void pow01();

	// bug in release r893 (fixed in r894).
	void bugr894();
	// bug in release r899 (fixed in r900).
	void bugr899();
	// bug in release r902 (fixed in r903).
	void bugr902();

	void imul01();
	void imul02();
	void imul03();
	void imul04();
	void imul05();
	void imul06();
	void imul07();
	void imul08();
	void imul09();
	void imul10();

private:
	void check_add_sub(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub);
	void check_mul_div_mono(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub);
	void check_mul_div(const Interval& z, const Interval& xin, const Interval& yin, bool ll, bool lr, bool ul, bool ur);

	// check also wrt min(-x,-y)=-z, max(y,x)=z and min(-y,-x)=-z
	void check_max_min(const Interval& z,  const Interval& x, const Interval& y, const Interval& xin, const Interval& yin, const Interval& x_expected, const Interval& y_expected);

	void check_imul(const Interval& x,  const Interval& y, const Interval& z);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestInnerArith);


} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
