/* ============================================================================
 * I B E X - Test of interval operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * Last Update : Jul 16, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_ARITH_H__
#define __TEST_ARITH_H__

#include "cpptest.h"
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestArith : public TestIbex {
public:
	TestArith() {

		TEST_ADD(TestArith::minus01);
		TEST_ADD(TestArith::minus02);
		TEST_ADD(TestArith::minus03);
		TEST_ADD(TestArith::minus04);

		TEST_ADD(TestArith::add01);
		TEST_ADD(TestArith::add02);
		TEST_ADD(TestArith::add03);
		TEST_ADD(TestArith::add04);
		TEST_ADD(TestArith::add05);
		TEST_ADD(TestArith::add06);
		TEST_ADD(TestArith::add07);
		TEST_ADD(TestArith::add08);
		TEST_ADD(TestArith::add09);
		TEST_ADD(TestArith::add10);
		TEST_ADD(TestArith::add11);

		TEST_ADD(TestArith::mul01);
		TEST_ADD(TestArith::mul02);
		TEST_ADD(TestArith::mul03);
		TEST_ADD(TestArith::mul04);
		TEST_ADD(TestArith::mul05);
		TEST_ADD(TestArith::mul06);
		TEST_ADD(TestArith::mul07);
		TEST_ADD(TestArith::mul08);
		TEST_ADD(TestArith::mul09);
		TEST_ADD(TestArith::mul10);
		TEST_ADD(TestArith::mul11);
		TEST_ADD(TestArith::mul12);
		TEST_ADD(TestArith::mul13);
		TEST_ADD(TestArith::mul14);
		TEST_ADD(TestArith::mul15);
		TEST_ADD(TestArith::mul16);
		TEST_ADD(TestArith::mul17);
		TEST_ADD(TestArith::mul18);
		TEST_ADD(TestArith::mul19);

		TEST_ADD(TestArith::div01);
		TEST_ADD(TestArith::div02);
		TEST_ADD(TestArith::div03);
		TEST_ADD(TestArith::div04);
		TEST_ADD(TestArith::div05);
		TEST_ADD(TestArith::div06);
		TEST_ADD(TestArith::div07);
		TEST_ADD(TestArith::div08);
		TEST_ADD(TestArith::div09);
		TEST_ADD(TestArith::div10);
		TEST_ADD(TestArith::div11);
		TEST_ADD(TestArith::div12);
		TEST_ADD(TestArith::div13);
		TEST_ADD(TestArith::div14);
		TEST_ADD(TestArith::div15);
		TEST_ADD(TestArith::div16);
		TEST_ADD(TestArith::div17);

		TEST_ADD(TestArith::div2_01);
		TEST_ADD(TestArith::div2_02);
		TEST_ADD(TestArith::div2_03);
		TEST_ADD(TestArith::div2_04);
		TEST_ADD(TestArith::div2_05);
		TEST_ADD(TestArith::div2_06);
		TEST_ADD(TestArith::div2_07);
		TEST_ADD(TestArith::div2_08);
		TEST_ADD(TestArith::div2_09);
		TEST_ADD(TestArith::div2_10);
		TEST_ADD(TestArith::div2_11);
		TEST_ADD(TestArith::div2_12);
		TEST_ADD(TestArith::div2_13);

		TEST_ADD(TestArith::log01);
		TEST_ADD(TestArith::log02);
		TEST_ADD(TestArith::log03);
		TEST_ADD(TestArith::log04);
		TEST_ADD(TestArith::log05);
		TEST_ADD(TestArith::log06);

		TEST_ADD(TestArith::sin01);
		TEST_ADD(TestArith::sin02);
		TEST_ADD(TestArith::sin03);
		TEST_ADD(TestArith::sin04);
		TEST_ADD(TestArith::sin05);
		TEST_ADD(TestArith::sin06);
		TEST_ADD(TestArith::sin07);
		TEST_ADD(TestArith::sin08);
		TEST_ADD(TestArith::sin09);
		TEST_ADD(TestArith::sin10);
		TEST_ADD(TestArith::sin11);
		TEST_ADD(TestArith::sin12);
		TEST_ADD(TestArith::sin13);

		TEST_ADD(TestArith::pow01);
		TEST_ADD(TestArith::pow02);
		TEST_ADD(TestArith::pow03);
		TEST_ADD(TestArith::pow04);
		TEST_ADD(TestArith::pow05);
		TEST_ADD(TestArith::pow06);
		TEST_ADD(TestArith::pow07);
		TEST_ADD(TestArith::pow08);
		TEST_ADD(TestArith::pow09);
		TEST_ADD(TestArith::pow10);
		TEST_ADD(TestArith::pow11);
		TEST_ADD(TestArith::pow12);

		TEST_ADD(TestArith::sqrt01);
		TEST_ADD(TestArith::sqrt02);
		TEST_ADD(TestArith::sqrt03);
		TEST_ADD(TestArith::sqrt04);
		TEST_ADD(TestArith::sqrt05);

		TEST_ADD(TestArith::proj_mul01);
		TEST_ADD(TestArith::proj_mul02);
		TEST_ADD(TestArith::proj_mul03);
		TEST_ADD(TestArith::proj_mul04);
		TEST_ADD(TestArith::proj_mul05);

		TEST_ADD(TestArith::proj_div01);
		TEST_ADD(TestArith::proj_div02);
		TEST_ADD(TestArith::proj_div03);
		TEST_ADD(TestArith::proj_div04);
		TEST_ADD(TestArith::proj_div05);
		TEST_ADD(TestArith::proj_div06);
		TEST_ADD(TestArith::proj_div07);
		TEST_ADD(TestArith::proj_div08);
		TEST_ADD(TestArith::proj_div09);

		TEST_ADD(TestArith::proj_sqr01);
		TEST_ADD(TestArith::proj_sqr02);
		TEST_ADD(TestArith::proj_sqr03);
		TEST_ADD(TestArith::proj_sqr04);
		TEST_ADD(TestArith::proj_sqr05);
		TEST_ADD(TestArith::proj_sqr06);
		TEST_ADD(TestArith::proj_sqr07);

		TEST_ADD(TestArith::proj_log01);
		TEST_ADD(TestArith::proj_log02);
		TEST_ADD(TestArith::proj_log03);
		TEST_ADD(TestArith::proj_log04);
		TEST_ADD(TestArith::proj_log05);
		TEST_ADD(TestArith::proj_log06);
		TEST_ADD(TestArith::proj_log07);
		TEST_ADD(TestArith::proj_log08);

		TEST_ADD(TestArith::proj_sin01);
		TEST_ADD(TestArith::proj_sin02);
		TEST_ADD(TestArith::proj_sin03);
		TEST_ADD(TestArith::proj_sin04);
		TEST_ADD(TestArith::proj_sin05);
		TEST_ADD(TestArith::proj_sin06);
		TEST_ADD(TestArith::proj_sin07);
		TEST_ADD(TestArith::proj_sin08);
		TEST_ADD(TestArith::proj_sin09);
		TEST_ADD(TestArith::proj_sin10);
		TEST_ADD(TestArith::proj_sin11);
		TEST_ADD(TestArith::proj_sin12);
		TEST_ADD(TestArith::proj_sin13);
		TEST_ADD(TestArith::proj_sin14);
		TEST_ADD(TestArith::proj_sin15);
		TEST_ADD(TestArith::proj_sin16);
		TEST_ADD(TestArith::proj_sin17);
		TEST_ADD(TestArith::proj_sin18);
		TEST_ADD(TestArith::proj_sin19);
		TEST_ADD(TestArith::proj_sin20);
		TEST_ADD(TestArith::proj_sin21);
		TEST_ADD(TestArith::proj_sin22);
		TEST_ADD(TestArith::proj_sin23);
		TEST_ADD(TestArith::proj_sin24);

		TEST_ADD(TestArith::proj_pow01);
		TEST_ADD(TestArith::proj_pow02);
		TEST_ADD(TestArith::proj_pow03);
		TEST_ADD(TestArith::proj_pow04);
		TEST_ADD(TestArith::proj_pow05);
		TEST_ADD(TestArith::proj_pow06);
		TEST_ADD(TestArith::proj_pow07);
		TEST_ADD(TestArith::proj_pow08);
		TEST_ADD(TestArith::proj_pow09);
		TEST_ADD(TestArith::proj_pow10);
		TEST_ADD(TestArith::proj_pow11);
		TEST_ADD(TestArith::proj_pow12);
		TEST_ADD(TestArith::proj_pow13);
		TEST_ADD(TestArith::proj_pow14);
		TEST_ADD(TestArith::proj_pow15);
		TEST_ADD(TestArith::proj_pow16);

	}
private:

	/* test: operator&=(const Interval& x) */
	void minus01();
	void minus02();
	void minus03();
	void minus04();

	/* test:
	 * =======
	 *   operator+=(double d)
	 *   operator+=(const Interval& x)
	 *   operator+(const Interval& x, double d)
	 *   operator+(double d,const Interval& x)
	 *   operator+(const Interval& x1, const Interval& x2)
	 *   operator-=(double d)
	 *   operator-=(const Interval& x)
	 *   operator-(const Interval& x, double d)
	 *   operator-(double d,const Interval& x)
	 *   operator-(const Interval& x1, const Interval& x2)
	 */
	void add01();
	void add02();
	void add03();
	void add04();
	void add05();
	void add06();
	void add07();
	void add08();
	void add09();
	void add10();
	void add11();

	/* test:
	 * =======
	 *   operator*=(double d)
	 *   operator*=(const Interval& x)
	 *   operator*(const Interval& x, double d)
	 *   operator*(double d,const Interval& x)
	 *   operator*(const Interval& x1, const Interval& x2)
	 */
	void mul01();
	void mul02();
	void mul03();
	void mul04();
	void mul05();
	void mul06();
	void mul07();
	void mul08();
	void mul09();
	void mul10();
	void mul11();
	void mul12();
	void mul13();
	void mul14();
	void mul15();
	void mul16();
	void mul17();
	void mul18();
	void mul19();

	/* test:
	 * =======
	 *   operator/=(double d)
	 *   operator/=(const Interval& x)
	 *   operator/(const Interval& x, double d)
	 *   operator/(double d,const Interval& x)
	 *   operator/(const Interval& x1, const Interval& x2)
	 */

	void div01();
	void div02();
	void div03();
	void div04();
	void div05();
	void div06();
	void div07();
	void div08();
	void div09();
	void div10();
	void div11();
	void div12();
	void div13();
	void div14();
	void div15();
	void div16();
	void div17();

	/* test:
	 * =========
	 *  div2_inter(const Interval& x, const Interval& y, Interval& out2)
	 *  div2_inter(const Interval& x, const Interval& y)
	 *  div2(const Interval& x, const Interval& y, Interval& out1, Interval& out2)
	 */
	void div2_01();
	void div2_02();
	void div2_03();
	void div2_04();
	void div2_05();
	void div2_06();
	void div2_07();
	void div2_08();
	void div2_09();
	void div2_10();
	void div2_11();
	void div2_12();
	void div2_13();

	void log01();
	void log02();
	void log03();
	void log04();
	void log05();
	void log06();

	/* test:
	 * sin
	 * cos
	 */
	void sin01();
	void sin02();
	void sin03();
	void sin04();
	void sin05();
	void sin06();
	void sin07();
	void sin08();
	void sin09();
	void sin10();
	void sin11();
	void sin12();
	void sin13();

	void pow01();
	void pow02();
	void pow03();
	void pow04();
	void pow05();
	void pow06();
	void pow07();
	void pow08();
	void pow09();
	void pow10();
	void pow11();
	void pow12();

	/* test: sqrt(const Interval& x); */
	void sqrt01();
	void sqrt02();
	void sqrt03();
	void sqrt04();
	void sqrt05();

	/* test: proj_mul */
	void proj_mul01();
	void proj_mul02();
	void proj_mul03();
	void proj_mul04();
	void proj_mul05();

	/* test: proj_div */
	void proj_div01();
	void proj_div02();
	void proj_div03();
	void proj_div04();
	void proj_div05();
	void proj_div06();
	void proj_div07();
	void proj_div08();
	void proj_div09();

	/* test: proj_sqrt(const Interval& y, Interval& x) */
	void proj_sqr01();
	void proj_sqr02();
	void proj_sqr03();
	void proj_sqr04();
	void proj_sqr05();
	void proj_sqr06();
	void proj_sqr07();

	void proj_log01();
	void proj_log02();
	void proj_log03();
	void proj_log04();
	void proj_log05();
	void proj_log06();
	void proj_log07();
	void proj_log08();

	/* test: proj_sin & proj_cos */
	void proj_sin01();
	void proj_sin02();
	void proj_sin03();
	void proj_sin04();
	void proj_sin05();
	void proj_sin06();
	void proj_sin07();
	void proj_sin08();
	void proj_sin09();
	void proj_sin10();
	void proj_sin11();
	void proj_sin12();
	void proj_sin13();
	void proj_sin14();
	void proj_sin15();
	void proj_sin16();
	void proj_sin17();
	void proj_sin18();
	void proj_sin19();
	void proj_sin20();
	void proj_sin21();
	void proj_sin22();
	void proj_sin23();
	void proj_sin24();

	void proj_pow01();
	void proj_pow02();
	void proj_pow03();
	void proj_pow04();
	void proj_pow05();
	void proj_pow06();
	void proj_pow07();
	void proj_pow08();
	void proj_pow09();
	void proj_pow10();
	void proj_pow11();
	void proj_pow12();
	void proj_pow13();
	void proj_pow14();
	void proj_pow15();
	void proj_pow16();

	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
	void check_trigo(const Interval& x, const Interval& y);
	void check_pow(const Interval& x, int p, const Interval& y_expected);
	bool checkproj_mul(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkproj_div(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkproj_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter);
	void checkproj_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p);
};

#endif // __TEST_ARITH__
