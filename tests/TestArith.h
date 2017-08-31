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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestArith : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestArith);
	
		CPPUNIT_TEST(tan_issue248);

		CPPUNIT_TEST(float01);
		CPPUNIT_TEST(float02);
		CPPUNIT_TEST(float03);
		CPPUNIT_TEST(float04);
		CPPUNIT_TEST(float05);
		CPPUNIT_TEST(float06);
		CPPUNIT_TEST(float07);
		CPPUNIT_TEST(float08);
		CPPUNIT_TEST(float09);
		/*CPPUNIT_TEST(float10); disabled temporarily due to bug between Gaol and elibc (used by Travis CI) */


		CPPUNIT_TEST(minus01);
		CPPUNIT_TEST(minus02);
		CPPUNIT_TEST(minus03);
		CPPUNIT_TEST(minus04);

		CPPUNIT_TEST(add01);
		CPPUNIT_TEST(add02);
		CPPUNIT_TEST(add03);
		CPPUNIT_TEST(add04);
		CPPUNIT_TEST(add05);
		CPPUNIT_TEST(add06);
		CPPUNIT_TEST(add07);
		CPPUNIT_TEST(add08);
		CPPUNIT_TEST(add09);
		CPPUNIT_TEST(add10);
		CPPUNIT_TEST(add11);
		CPPUNIT_TEST(add12);
		CPPUNIT_TEST(add13);
		CPPUNIT_TEST(add14);
		CPPUNIT_TEST(add15);
		CPPUNIT_TEST(add16);
		CPPUNIT_TEST(add17);
		CPPUNIT_TEST(add18);
		CPPUNIT_TEST(add19);

		CPPUNIT_TEST(mul01);
		CPPUNIT_TEST(mul02);
		CPPUNIT_TEST(mul03);
		CPPUNIT_TEST(mul04);
		CPPUNIT_TEST(mul05);
		CPPUNIT_TEST(mul06);
		CPPUNIT_TEST(mul07);
		CPPUNIT_TEST(mul08);
		CPPUNIT_TEST(mul09);
		CPPUNIT_TEST(mul10);
		CPPUNIT_TEST(mul11);
		CPPUNIT_TEST(mul12);
		CPPUNIT_TEST(mul13);
		CPPUNIT_TEST(mul14);
		CPPUNIT_TEST(mul15);
		CPPUNIT_TEST(mul16);
		CPPUNIT_TEST(mul17);
		CPPUNIT_TEST(mul18);
		CPPUNIT_TEST(mul19);

		CPPUNIT_TEST(mulMM01);

		CPPUNIT_TEST(div01);
		CPPUNIT_TEST(div02);
		CPPUNIT_TEST(div03);
		CPPUNIT_TEST(div04);
		CPPUNIT_TEST(div05);
		CPPUNIT_TEST(div06);
		CPPUNIT_TEST(div07);
		CPPUNIT_TEST(div08);
		CPPUNIT_TEST(div09);
		CPPUNIT_TEST(div10);
		CPPUNIT_TEST(div11);
		CPPUNIT_TEST(div12);
		CPPUNIT_TEST(div13);
		CPPUNIT_TEST(div14);
		CPPUNIT_TEST(div15);
		CPPUNIT_TEST(div16);
		CPPUNIT_TEST(div17);

		CPPUNIT_TEST(div2_01);
		CPPUNIT_TEST(div2_02);
		CPPUNIT_TEST(div2_03);
		CPPUNIT_TEST(div2_04);
		CPPUNIT_TEST(div2_05);
		CPPUNIT_TEST(div2_06);
		CPPUNIT_TEST(div2_07);
		CPPUNIT_TEST(div2_08);
		CPPUNIT_TEST(div2_09);
		CPPUNIT_TEST(div2_10);
		CPPUNIT_TEST(div2_11);
		CPPUNIT_TEST(div2_12);
		CPPUNIT_TEST(div2_13);

		CPPUNIT_TEST(log01);
		CPPUNIT_TEST(log02);
		CPPUNIT_TEST(log03);
		CPPUNIT_TEST(log04);
		CPPUNIT_TEST(log05);
		CPPUNIT_TEST(log06);
		CPPUNIT_TEST(log07);
		CPPUNIT_TEST(log08);
		CPPUNIT_TEST(log09);
		CPPUNIT_TEST(log10);
		CPPUNIT_TEST(log11);

		CPPUNIT_TEST(exp01);
		CPPUNIT_TEST(exp02);
		CPPUNIT_TEST(exp03);
		CPPUNIT_TEST(exp04);
		CPPUNIT_TEST(exp05);
		CPPUNIT_TEST(exp06);
		CPPUNIT_TEST(exp07);
		CPPUNIT_TEST(exp08);
		CPPUNIT_TEST(exp09);

		CPPUNIT_TEST(sin01);
		CPPUNIT_TEST(sin02);
		CPPUNIT_TEST(sin03);
		CPPUNIT_TEST(sin04);
		CPPUNIT_TEST(sin05);
		CPPUNIT_TEST(sin06);
		CPPUNIT_TEST(sin07);
		CPPUNIT_TEST(sin08);
		CPPUNIT_TEST(sin09);
		CPPUNIT_TEST(sin10);
		CPPUNIT_TEST(sin11);
		CPPUNIT_TEST(sin12);
		CPPUNIT_TEST(sin13);

		CPPUNIT_TEST(tan01);
		CPPUNIT_TEST(tan02);
		CPPUNIT_TEST(tan03);
		CPPUNIT_TEST(tan04);
		CPPUNIT_TEST(tan05);

		CPPUNIT_TEST(pow01);
		CPPUNIT_TEST(pow02);
		CPPUNIT_TEST(pow03);
		CPPUNIT_TEST(pow04);
		CPPUNIT_TEST(pow05);
		CPPUNIT_TEST(pow06);
		CPPUNIT_TEST(pow07);
		CPPUNIT_TEST(pow08);
		CPPUNIT_TEST(pow09);
		CPPUNIT_TEST(pow10);
		CPPUNIT_TEST(pow11);
		CPPUNIT_TEST(pow12);
		CPPUNIT_TEST(pow13);

		CPPUNIT_TEST(root01);
		CPPUNIT_TEST(root02);
		CPPUNIT_TEST(root03);
		CPPUNIT_TEST(root04);

		CPPUNIT_TEST(sqrt01);
		CPPUNIT_TEST(sqrt02);
		CPPUNIT_TEST(sqrt03);
		CPPUNIT_TEST(sqrt04);
		CPPUNIT_TEST(sqrt05);

		CPPUNIT_TEST(sinh01);
		CPPUNIT_TEST(sinh02);
		CPPUNIT_TEST(sinh03);
		CPPUNIT_TEST(sinh04);
		CPPUNIT_TEST(sinh05);
		CPPUNIT_TEST(sinh06);
		CPPUNIT_TEST(sinh07);

		CPPUNIT_TEST(cosh01);
		CPPUNIT_TEST(cosh02);
		CPPUNIT_TEST(cosh03);
		CPPUNIT_TEST(cosh04);
		CPPUNIT_TEST(cosh05);
		CPPUNIT_TEST(cosh06);
		CPPUNIT_TEST(cosh07);

		CPPUNIT_TEST(atan2_01);
		CPPUNIT_TEST(atan2_02);
		CPPUNIT_TEST(atan2_03);
		CPPUNIT_TEST(atan2_04);
		CPPUNIT_TEST(atan2_05);
		CPPUNIT_TEST(atan2_06);
		CPPUNIT_TEST(atan2_07);
		CPPUNIT_TEST(atan2_08);
		CPPUNIT_TEST(atan2_09);
		CPPUNIT_TEST(atan2_10);
		CPPUNIT_TEST(atan2_11);
		CPPUNIT_TEST(atan2_12);
		CPPUNIT_TEST(atan2_13);
		CPPUNIT_TEST(atan2_14);
		CPPUNIT_TEST(atan2_15);

		CPPUNIT_TEST(bwd_mul01);
		CPPUNIT_TEST(bwd_mul02);
		CPPUNIT_TEST(bwd_mul03);
		CPPUNIT_TEST(bwd_mul04);
		CPPUNIT_TEST(bwd_mul05);

		CPPUNIT_TEST(bwd_mulVV01);
		CPPUNIT_TEST(bwd_mulMV01);


		CPPUNIT_TEST(bwd_div01);
		CPPUNIT_TEST(bwd_div02);
		CPPUNIT_TEST(bwd_div03);
		CPPUNIT_TEST(bwd_div04);
		CPPUNIT_TEST(bwd_div05);
		CPPUNIT_TEST(bwd_div06);
		CPPUNIT_TEST(bwd_div07);
		CPPUNIT_TEST(bwd_div08);
		CPPUNIT_TEST(bwd_div09);

		CPPUNIT_TEST(bwd_maxmin01);
		CPPUNIT_TEST(bwd_maxmin02);
		CPPUNIT_TEST(bwd_maxmin03);
		CPPUNIT_TEST(bwd_maxmin04);
		CPPUNIT_TEST(bwd_maxmin05);

		CPPUNIT_TEST(bwd_sqr01);
		CPPUNIT_TEST(bwd_sqr02);
		CPPUNIT_TEST(bwd_sqr03);
		CPPUNIT_TEST(bwd_sqr04);
		CPPUNIT_TEST(bwd_sqr05);
		CPPUNIT_TEST(bwd_sqr06);
		CPPUNIT_TEST(bwd_sqr07);

		CPPUNIT_TEST(bwd_log01);
		CPPUNIT_TEST(bwd_log02);
		CPPUNIT_TEST(bwd_log03);
		CPPUNIT_TEST(bwd_log04);
		CPPUNIT_TEST(bwd_log05);
		CPPUNIT_TEST(bwd_log06);
		CPPUNIT_TEST(bwd_log07);
		CPPUNIT_TEST(bwd_log08);

		CPPUNIT_TEST(bwd_sin01);
		CPPUNIT_TEST(bwd_sin02);
		CPPUNIT_TEST(bwd_sin03);
		CPPUNIT_TEST(bwd_sin04);
		CPPUNIT_TEST(bwd_sin05);
		CPPUNIT_TEST(bwd_sin06);
		CPPUNIT_TEST(bwd_sin07);
		CPPUNIT_TEST(bwd_sin08);
		CPPUNIT_TEST(bwd_sin09);
		CPPUNIT_TEST(bwd_sin10);
		CPPUNIT_TEST(bwd_sin11);
		CPPUNIT_TEST(bwd_sin12);
		CPPUNIT_TEST(bwd_sin13);
		CPPUNIT_TEST(bwd_sin14);
		CPPUNIT_TEST(bwd_sin15);
		CPPUNIT_TEST(bwd_sin16);
		CPPUNIT_TEST(bwd_sin17);
		CPPUNIT_TEST(bwd_sin18);
		CPPUNIT_TEST(bwd_sin19);
		CPPUNIT_TEST(bwd_sin20);
		CPPUNIT_TEST(bwd_sin21);
		CPPUNIT_TEST(bwd_sin22);
		CPPUNIT_TEST(bwd_sin23);
		CPPUNIT_TEST(bwd_sin24);

		CPPUNIT_TEST(bwd_pow01);
		CPPUNIT_TEST(bwd_pow02);
		CPPUNIT_TEST(bwd_pow03);
		CPPUNIT_TEST(bwd_pow04);
		CPPUNIT_TEST(bwd_pow05);
		CPPUNIT_TEST(bwd_pow06);
		CPPUNIT_TEST(bwd_pow07);
		CPPUNIT_TEST(bwd_pow08);
		CPPUNIT_TEST(bwd_pow09);
		CPPUNIT_TEST(bwd_pow10);
		CPPUNIT_TEST(bwd_pow11);
		CPPUNIT_TEST(bwd_pow12);
		CPPUNIT_TEST(bwd_pow13);
		CPPUNIT_TEST(bwd_pow14);
		CPPUNIT_TEST(bwd_pow15);
		CPPUNIT_TEST(bwd_pow16);
		CPPUNIT_TEST(bwd_pow17);

		// ===== temporarily commented ===============
		CPPUNIT_TEST(bwd_atan2_01);
		CPPUNIT_TEST(bwd_atan2_02);
		CPPUNIT_TEST(bwd_atan2_03);
		CPPUNIT_TEST(bwd_atan2_04);
		CPPUNIT_TEST(bwd_atan2_05);
		CPPUNIT_TEST(bwd_atan2_06);
		CPPUNIT_TEST(bwd_atan2_07);
		CPPUNIT_TEST(bwd_atan2_08);
		CPPUNIT_TEST(bwd_atan2_09); // commented waiting for an optimal contractor.
		CPPUNIT_TEST(bwd_atan2_10);
		CPPUNIT_TEST(bwd_atan2_issue134);

		CPPUNIT_TEST(bwd_sqrt01);
		CPPUNIT_TEST(bwd_sqrt02);
		CPPUNIT_TEST(bwd_sqrt03);
		CPPUNIT_TEST(bwd_sqrt04);
		CPPUNIT_TEST(bwd_sqrt05);
		CPPUNIT_TEST(bwd_sqrt06);
		CPPUNIT_TEST(bwd_sqrt07);

		CPPUNIT_TEST(bwd_atan01);
		CPPUNIT_TEST(bwd_atan02);
		CPPUNIT_TEST(bwd_atan03);
		CPPUNIT_TEST(bwd_atan04);
		CPPUNIT_TEST(bwd_atan05);
		CPPUNIT_TEST(bwd_atan06);
		CPPUNIT_TEST(bwd_atan07);
		CPPUNIT_TEST(bwd_atan08);

		CPPUNIT_TEST(bwd_add01);
		CPPUNIT_TEST(bwd_add02);
		CPPUNIT_TEST(bwd_add03);
		CPPUNIT_TEST(bwd_add04);
		CPPUNIT_TEST(bwd_add05);

		CPPUNIT_TEST(bwd_sub01);
		CPPUNIT_TEST(bwd_sub02);
		CPPUNIT_TEST(bwd_sub03);
		CPPUNIT_TEST(bwd_sub04);
		CPPUNIT_TEST(bwd_sub05);

		CPPUNIT_TEST(bwd_imod_01);
		CPPUNIT_TEST(bwd_imod_02);
		CPPUNIT_TEST(bwd_imod_03);
		CPPUNIT_TEST(bwd_imod_04);
		CPPUNIT_TEST(bwd_imod_05);
		CPPUNIT_TEST(bwd_imod_06);
		CPPUNIT_TEST(bwd_imod_07);
		CPPUNIT_TEST(bwd_imod_08);
	CPPUNIT_TEST_SUITE_END();
private:
	/* test:
	 * =======
	 * previous_float(double x)
	 * next_float(double x)
	 */
	void float01();
	void float02();
	void float03();
	void float04();
	void float05();
	void float06();
	void float07();
	void float08();
	void float09();
	void float10();

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
	void add12();
	void add13();
	void add14();
	void add15();
	void add16();
	void add17();
	void add18();
	void add19();

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

	void mulMM01();

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
	void log07();
	void log08();
	void log09();
	void log10();
	void log11();

	void exp01();
	void exp02();
	void exp03();
	void exp04();
	void exp05();
	void exp06();
	void exp07();
	void exp08();
	void exp09();

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

	void tan01();
	void tan02();
	void tan03();
	void tan04();
	void tan05();
	void tan06();
	void tan07();

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
	void pow13();

	void root01();
	void root02();
	void root03();
	void root04();

	/* test: sqrt(const Interval& x); */
	void sqrt01();
	void sqrt02();
	void sqrt03();
	void sqrt04();
	void sqrt05();
	void sqrt06();

	void sinh01();
	void sinh02();
	void sinh03();
	void sinh04();
	void sinh05();
	void sinh06();
	void sinh07();

	void cosh01();
	void cosh02();
	void cosh03();
	void cosh04();
	void cosh05();
	void cosh06();
	void cosh07();

	void atan2_01();
	void atan2_02();
	void atan2_03();
	void atan2_04();
	void atan2_05();
	void atan2_06();
	void atan2_07();
	void atan2_08();
	void atan2_09();
	void atan2_10();
	void atan2_11();
	void atan2_12();
	void atan2_13();
	void atan2_14();
	void atan2_15();

	/* test: bwd_mul */
	void bwd_mul01();
	void bwd_mul02();
	void bwd_mul03();
	void bwd_mul04();
	void bwd_mul05();

	void bwd_mulVV01();
	void bwd_mulMV01();

	/* test: bwd_div */
	void bwd_div01();
	void bwd_div02();
	void bwd_div03();
	void bwd_div04();
	void bwd_div05();
	void bwd_div06();
	void bwd_div07();
	void bwd_div08();
	void bwd_div09();

	void bwd_maxmin01();
	void bwd_maxmin02();
	void bwd_maxmin03();
	void bwd_maxmin04();
	void bwd_maxmin05();

	/* test: bwd_sqrt(const Interval& y, Interval& x) */
	void bwd_sqr01();
	void bwd_sqr02();
	void bwd_sqr03();
	void bwd_sqr04();
	void bwd_sqr05();
	void bwd_sqr06();
	void bwd_sqr07();

	void bwd_log01();
	void bwd_log02();
	void bwd_log03();
	void bwd_log04();
	void bwd_log05();
	void bwd_log06();
	void bwd_log07();
	void bwd_log08();

	/* test: bwd_sin & bwd_cos */
	void bwd_sin01();
	void bwd_sin02();
	void bwd_sin03();
	void bwd_sin04();
	void bwd_sin05();
	void bwd_sin06();
	void bwd_sin07();
	void bwd_sin08();
	void bwd_sin09();
	void bwd_sin10();
	void bwd_sin11();
	void bwd_sin12();
	void bwd_sin13();
	void bwd_sin14();
	void bwd_sin15();
	void bwd_sin16();
	void bwd_sin17();
	void bwd_sin18();
	void bwd_sin19();
	void bwd_sin20();
	void bwd_sin21();
	void bwd_sin22();
	void bwd_sin23();
	void bwd_sin24();

	void bwd_pow01();
	void bwd_pow02();
	void bwd_pow03();
	void bwd_pow04();
	void bwd_pow05();
	void bwd_pow06();
	void bwd_pow07();
	void bwd_pow08();
	void bwd_pow09();
	void bwd_pow10();
	void bwd_pow11();
	void bwd_pow12();
	void bwd_pow13();
	void bwd_pow14();
	void bwd_pow15();
	void bwd_pow16();
	void bwd_pow17();

	void bwd_atan2_01();
	void bwd_atan2_02();
	void bwd_atan2_03();
	void bwd_atan2_04();
	void bwd_atan2_05();
	void bwd_atan2_06();
	void bwd_atan2_07();
	void bwd_atan2_08();
	void bwd_atan2_09();
	void bwd_atan2_10();
	void bwd_atan2_issue134();

	void bwd_sqrt01();
	void bwd_sqrt02();
	void bwd_sqrt03();
	void bwd_sqrt04();
	void bwd_sqrt05();
	void bwd_sqrt06();
	void bwd_sqrt07();

	void bwd_atan01();
	void bwd_atan02();
	void bwd_atan03();
	void bwd_atan04();
	void bwd_atan05();
	void bwd_atan06();
	void bwd_atan07();
	void bwd_atan08();

	void bwd_add01();
	void bwd_add02();
	void bwd_add03();
	void bwd_add04();
	void bwd_add05();

	void bwd_sub01();
	void bwd_sub02();
	void bwd_sub03();
	void bwd_sub04();
	void bwd_sub05();

	void bwd_imod_01();
	void bwd_imod_02();
	void bwd_imod_03();
	void bwd_imod_04();
	void bwd_imod_05();
	void bwd_imod_06();
	void bwd_imod_07();
	void bwd_imod_08();

	void tan_issue248();


	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
	void check_sinh(const Interval& x);
	void check_cosh(const Interval& x);
	void check_trigo(const Interval& x, const Interval& y);

	void check_pow(const Interval& x, int p, const Interval& y_expected);
	bool checkbwd_mul(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkbwd_div(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkbwd_max(const Interval& z,  const Interval& x, const Interval& y, const Interval& x_expected, const Interval& y_expected);
	void checkbwd_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter);
	void checkbwd_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p);
	bool checkbwd_atan2(const Interval& a, const Interval& y_bef, const Interval& x_bef,
								const Interval& y_aft, const Interval& x_aft);
	bool checkbwd_atan(const Interval& y, const Interval& xbefore, const Interval& xafter);
	bool checkbwd_add(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	bool checkbwd_sub(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	bool checkbwd_imod(const double& period, const Interval& x_bef, const Interval& y_bef,
								const Interval& x_aft, const Interval& y_aft);

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestArith);


#endif // __TEST_ARITH__
