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
		TEST_ADD(TestInterval::cons01);
		TEST_ADD(TestInterval::cons02);
		TEST_ADD(TestInterval::cons03);
		TEST_ADD(TestInterval::cons04);
		TEST_ADD(TestInterval::cons05);

		TEST_ADD(TestInterval::add01);
		TEST_ADD(TestInterval::add02);
		TEST_ADD(TestInterval::add03);
		TEST_ADD(TestInterval::add04);
		TEST_ADD(TestInterval::add05);
		TEST_ADD(TestInterval::add06);
		TEST_ADD(TestInterval::add07);
		TEST_ADD(TestInterval::add08);
		TEST_ADD(TestInterval::add09);
		TEST_ADD(TestInterval::add10);
		TEST_ADD(TestInterval::add11);

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
		TEST_ADD(TestInterval::mul17);
		TEST_ADD(TestInterval::mul18);
		TEST_ADD(TestInterval::mul19);

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
		TEST_ADD(TestInterval::div15);
		TEST_ADD(TestInterval::div16);
		TEST_ADD(TestInterval::div17);

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

		TEST_ADD(TestInterval::distance01);
		TEST_ADD(TestInterval::distance02);
		TEST_ADD(TestInterval::distance03);
		TEST_ADD(TestInterval::distance04);
		TEST_ADD(TestInterval::distance05);
		TEST_ADD(TestInterval::distance06);
		TEST_ADD(TestInterval::distance07);
		TEST_ADD(TestInterval::distance08);
		TEST_ADD(TestInterval::distance09);
		TEST_ADD(TestInterval::distance10);
		TEST_ADD(TestInterval::distance11);
		TEST_ADD(TestInterval::distance12);
		TEST_ADD(TestInterval::distance13);
		TEST_ADD(TestInterval::distance14);
		TEST_ADD(TestInterval::distance15);
		TEST_ADD(TestInterval::distance15b);
		TEST_ADD(TestInterval::distance16);
		TEST_ADD(TestInterval::distance17);
		TEST_ADD(TestInterval::distance18);
		TEST_ADD(TestInterval::distance19);
		TEST_ADD(TestInterval::distance20);
		TEST_ADD(TestInterval::distance21);
		TEST_ADD(TestInterval::distance22);
		TEST_ADD(TestInterval::distance23);

		TEST_ADD(TestInterval::rel_distance01);
		TEST_ADD(TestInterval::rel_distance02);
		TEST_ADD(TestInterval::rel_distance03);
		TEST_ADD(TestInterval::rel_distance04);
		TEST_ADD(TestInterval::rel_distance05);
		TEST_ADD(TestInterval::rel_distance06);
		TEST_ADD(TestInterval::rel_distance07);
		TEST_ADD(TestInterval::rel_distance08);
		TEST_ADD(TestInterval::rel_distance09);
		TEST_ADD(TestInterval::rel_distance10);
		TEST_ADD(TestInterval::rel_distance11);
		TEST_ADD(TestInterval::rel_distance12);
		TEST_ADD(TestInterval::rel_distance13);
		TEST_ADD(TestInterval::rel_distance14);
		TEST_ADD(TestInterval::rel_distance15);
		TEST_ADD(TestInterval::rel_distance16);
		TEST_ADD(TestInterval::rel_distance17);
		TEST_ADD(TestInterval::rel_distance18);
		TEST_ADD(TestInterval::rel_distance19);
		TEST_ADD(TestInterval::rel_distance20);
		TEST_ADD(TestInterval::rel_distance21);

		TEST_ADD(TestInterval::hull01);
		TEST_ADD(TestInterval::hull02);
		TEST_ADD(TestInterval::hull03);

		TEST_ADD(TestInterval::div2_01);
		TEST_ADD(TestInterval::div2_02);
		TEST_ADD(TestInterval::div2_03);
		TEST_ADD(TestInterval::div2_04);
		TEST_ADD(TestInterval::div2_05);
		TEST_ADD(TestInterval::div2_06);
		TEST_ADD(TestInterval::div2_07);
		TEST_ADD(TestInterval::div2_08);
		TEST_ADD(TestInterval::div2_09);
		TEST_ADD(TestInterval::div2_10);
		TEST_ADD(TestInterval::div2_11);
		TEST_ADD(TestInterval::div2_12);
		TEST_ADD(TestInterval::div2_13);

	}
private:
	void cons01();
	void cons02();
	void cons03();
	void cons04();
	void cons05();

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

	void distance01();
	void distance02();
	void distance03();
	void distance04();
	void distance05();
	void distance06();
	void distance07();
	void distance08();
	void distance09();
	void distance10();
	void distance11();
	void distance12();
	void distance13();
	void distance14();
	void distance15();
	void distance15b();
	void distance16();
	void distance17();
	void distance18();
	void distance19();
	void distance20();
	void distance21();
	void distance22();
	void distance23();

	void rel_distance01();
	void rel_distance02();
	void rel_distance03();
	void rel_distance04();
	void rel_distance05();
	void rel_distance06();
	void rel_distance07();
	void rel_distance08();
	void rel_distance09();
	void rel_distance10();
	void rel_distance11();
	void rel_distance12();
	void rel_distance13();
	void rel_distance14();
	void rel_distance15();
	void rel_distance16();
	void rel_distance17();
	void rel_distance18();
	void rel_distance19();
	void rel_distance20();
	void rel_distance21();

	void hull01();
	void hull02();
	void hull03();

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

	void check(double y_actual, double y_expected);
	void check(const Interval& y_actual, const Interval& y_expected);
	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_hull(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
};

#endif /* TESTInterval_H_ */
