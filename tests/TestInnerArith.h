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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

#include "ibex_Interval.h"

namespace ibex {

class TestInnerArith : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	

			add_sub01();
			add_sub01_2();
			add_sub01_3();
			add_sub02();
			add_sub02_2();
			add_sub02_3();
			add_sub03();
			add_sub03_2();
			add_sub04();
			add_sub04_2();
			add_sub04_3();

			add_sub05_1();
			add_sub05_2();
			add_sub05_3();
			add_sub05_4();
			add_sub05_5();
			add_sub05_6();
			add_sub05_7();
			add_sub05_8();
			add_sub05_9();
			add_sub05_10();
			add_sub05_11();

			add_sub06_1();
			add_sub06_2();

			add_sub07();

			mul_div_mono01();
			mul_div_mono01_2();
			mul_div_mono01_3();
			mul_div_mono02();
			mul_div_mono02_2();
			mul_div_mono02_3();
			mul_div_mono03();
			mul_div_mono03_2();
			mul_div_mono04();
			mul_div_mono04_2();
			mul_div_mono04_3();

			mul_div_mono05_1();
			mul_div_mono05_2();
			mul_div_mono05_3();
			mul_div_mono05_4();
			mul_div_mono05_5();
			mul_div_mono05_6();
			mul_div_mono05_7();
			mul_div_mono05_8();
			mul_div_mono05_9();
			mul_div_mono05_10();
			mul_div_mono05_11();


			mul_div_mono06_1();
			mul_div_mono06_2();

			mul_div_mono07();

			mul08();
			mul09();
			mul10();
			mul11();
			mul12_1();
			mul12_2();
			mul12_3();
			mul12_4();
			mul13_1();
			mul13_2();
			mul13_3();
			mul13_4();
			mul14_1();
			mul14_2();
			mul14_3();

			div08_1();
			div08_2();
			div09_1();
			div09_2();

			maxmin01();
			maxmin02();
			maxmin03();
			maxmin04();
			maxmin05();
			maxmin06();
			maxmin07();

			abs01();
			abs02();
			abs03();
			abs04();
			abs05();

			sqrt01();
			sqrt02();
			sqrt03();
			sqrt04();
			sqrt05();

			pow01();


			bugr894();
			bugr899();
			bugr902();

			imul01();
			imul02();
			imul03();
			imul04();
			imul05();
			imul06();
			imul07();
			imul08();
			imul09();
			imul10();
		tearDown();
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




} // namespace ibex
#endif // __TEST_INNER_ARITH_H__
