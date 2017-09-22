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

#ifndef __TEST_AFFINE_BASE_H__
#define __TEST_AFFINE_BASE_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Affine.h"
#include "utils.h"

using namespace ibex;

template<class T>
class TestAffineBase : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestAffineBase<T>);
	
		CPPUNIT_TEST(tan_issue248);


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

// changement global du mode de calcul des operateur non lineaire
		CPPUNIT_TEST(change_mode_MinRange);

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

	CPPUNIT_TEST_SUITE_END();
private:

	void change_mode_MinRange();

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


	void tan_issue248();



	void check_affine_eq(const AffineMain<T>& y_actual, const Interval& y_expected, double err=ERROR);
	void check_affine_inclu(const AffineMain<T>& y_actual, const Interval& y_expected, double err=ERROR);


	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add_scal(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_add(const Interval& x, double z, const Interval& y_expected);
	void check_add(const AffineMain<T>& x, const AffineMain<T>& z, const Interval& y_expected);
	void check_add(const AffineMain<T>& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul(const AffineMain<T>& x, const AffineMain<T>& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div(const  AffineMain<T>& x, const  AffineMain<T>& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
	void check_sinh(const Interval& x);
	void check_cosh(const Interval& x);
	void check_trigo(const Interval& x, const Interval& y);

	void check_pow(const Interval& x, int p, const Interval& y_expected);


};


CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineBase<AF_Default>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineBase<AF_Other>);


#ifdef _IBEX_WITH_AFFINE_EXTENDED_

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_sAF>);

#endif


#endif // __TEST_ARITH__
