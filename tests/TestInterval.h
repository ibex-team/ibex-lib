/* ============================================================================
 * I B E X - Test of interval operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * ---------------------------------------------------------------------------- */

#ifndef TESTInterval_H_
#define TESTInterval_H_

#include "cpptest.h"
#include "ibex_Interval.h"

using namespace ibex;

class TestInterval : public Test::Suite {
public:
	TestInterval() {
		TEST_ADD(TestInterval::add01);
		TEST_ADD(TestInterval::add02);
		TEST_ADD(TestInterval::add03);
		TEST_ADD(TestInterval::add04);
		TEST_ADD(TestInterval::add05);
		TEST_ADD(TestInterval::mul01);
		TEST_ADD(TestInterval::mul02);
		TEST_ADD(TestInterval::mul03);
		TEST_ADD(TestInterval::mul04);
		TEST_ADD(TestInterval::mul05);
		TEST_ADD(TestInterval::mul06);
		TEST_ADD(TestInterval::mul07);
		TEST_ADD(TestInterval::mul08);
		TEST_ADD(TestInterval::mul09);
		TEST_ADD(TestInterval::mul10);
		TEST_ADD(TestInterval::mul11);
		TEST_ADD(TestInterval::mul12);
		TEST_ADD(TestInterval::mul13);
		TEST_ADD(TestInterval::mul14);
		TEST_ADD(TestInterval::mul15);
		TEST_ADD(TestInterval::mul16);
		TEST_ADD(TestInterval::div01);
		TEST_ADD(TestInterval::div02);
		TEST_ADD(TestInterval::div03);
		TEST_ADD(TestInterval::div04);
		TEST_ADD(TestInterval::div05);
		TEST_ADD(TestInterval::div06);
		TEST_ADD(TestInterval::div07);
		TEST_ADD(TestInterval::div08);
		TEST_ADD(TestInterval::div09);
		TEST_ADD(TestInterval::div10);
		TEST_ADD(TestInterval::div11);
		TEST_ADD(TestInterval::div12);
		TEST_ADD(TestInterval::div13);
		TEST_ADD(TestInterval::div14);
		TEST_ADD(TestInterval::sqrt01);
		TEST_ADD(TestInterval::sqrt02);
		TEST_ADD(TestInterval::sqrt03);
		TEST_ADD(TestInterval::sqrt04);
		TEST_ADD(TestInterval::sqrt05);
		TEST_ADD(TestInterval::sqrProj01);
		TEST_ADD(TestInterval::sqrProj02);
		TEST_ADD(TestInterval::sqrProj03);
		TEST_ADD(TestInterval::sqrProj04);
		TEST_ADD(TestInterval::sqrProj05);
		TEST_ADD(TestInterval::sqrProj06);
		TEST_ADD(TestInterval::sqrProj07);
		TEST_ADD(TestInterval::sinProj01);
	}
private:
	void add01();
	void add02();
	void add03();
	void add04();
	void add05();

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

	void sqrt01();
	void sqrt02();
	void sqrt03();
	void sqrt04();
	void sqrt05();

	void sqrProj01();
	void sqrProj02();
	void sqrProj03();
	void sqrProj04();
	void sqrProj05();
	void sqrProj06();
	void sqrProj07();

	void sinProj01();

	void check(const Interval& y_actual, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
};

#endif /* TESTInterval_H_ */
