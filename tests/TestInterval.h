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

#ifndef __TEST_INTERVAL_H__
#define __TEST_INTERVAL_H__

#include "cpptest.h"
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestInterval : public TestIbex {
public:
	TestInterval() {
		TEST_ADD(TestInterval::cons01);
		TEST_ADD(TestInterval::cons02);
		TEST_ADD(TestInterval::cons03);
		TEST_ADD(TestInterval::cons04);
		TEST_ADD(TestInterval::cons05);

		TEST_ADD(TestInterval::eq01);
		TEST_ADD(TestInterval::eq02);
		TEST_ADD(TestInterval::eq03);
		TEST_ADD(TestInterval::eq04);
		TEST_ADD(TestInterval::eq05);
		TEST_ADD(TestInterval::eq06);
		TEST_ADD(TestInterval::eq07);
		TEST_ADD(TestInterval::eq08);

		TEST_ADD(TestInterval::set_empty01);
		TEST_ADD(TestInterval::set_empty02);

		TEST_ADD(TestInterval::inter01);
		TEST_ADD(TestInterval::inter02);
		TEST_ADD(TestInterval::inter03);
		TEST_ADD(TestInterval::inter04);
		TEST_ADD(TestInterval::inter05);
		TEST_ADD(TestInterval::inter06);

		TEST_ADD(TestInterval::hull01);
		TEST_ADD(TestInterval::hull02);
		TEST_ADD(TestInterval::hull03);

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

		TEST_ADD(TestInterval::sin01);
		TEST_ADD(TestInterval::sin02);
		TEST_ADD(TestInterval::sin03);
		TEST_ADD(TestInterval::sin04);
		TEST_ADD(TestInterval::sin05);
		TEST_ADD(TestInterval::sin06);
		TEST_ADD(TestInterval::sin07);
		TEST_ADD(TestInterval::sin08);
		TEST_ADD(TestInterval::sin09);
		TEST_ADD(TestInterval::sin10);
		TEST_ADD(TestInterval::sin11);
		TEST_ADD(TestInterval::sin12);
		TEST_ADD(TestInterval::sin13);

		TEST_ADD(TestInterval::getters01);
		TEST_ADD(TestInterval::getters02);
		TEST_ADD(TestInterval::getters03);
		TEST_ADD(TestInterval::getters04);
		TEST_ADD(TestInterval::getters05);
		TEST_ADD(TestInterval::getters06);

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

		TEST_ADD(TestInterval::sqrt01);
		TEST_ADD(TestInterval::sqrt02);
		TEST_ADD(TestInterval::sqrt03);
		TEST_ADD(TestInterval::sqrt04);
		TEST_ADD(TestInterval::sqrt05);

		TEST_ADD(TestInterval::proj_mul01);
		TEST_ADD(TestInterval::proj_mul02);
		TEST_ADD(TestInterval::proj_mul03);
		TEST_ADD(TestInterval::proj_mul04);
		TEST_ADD(TestInterval::proj_mul05);

		TEST_ADD(TestInterval::sqrProj01);
		TEST_ADD(TestInterval::sqrProj02);
		TEST_ADD(TestInterval::sqrProj03);
		TEST_ADD(TestInterval::sqrProj04);
		TEST_ADD(TestInterval::sqrProj05);
		TEST_ADD(TestInterval::sqrProj06);
		TEST_ADD(TestInterval::sqrProj07);

		TEST_ADD(TestInterval::sinProj01);
		TEST_ADD(TestInterval::sinProj02);
		TEST_ADD(TestInterval::sinProj03);
		TEST_ADD(TestInterval::sinProj04);
		TEST_ADD(TestInterval::sinProj05);
		TEST_ADD(TestInterval::sinProj06);
		TEST_ADD(TestInterval::sinProj07);
		TEST_ADD(TestInterval::sinProj08);
		TEST_ADD(TestInterval::sinProj09);
		TEST_ADD(TestInterval::sinProj10);
		TEST_ADD(TestInterval::sinProj11);
		TEST_ADD(TestInterval::sinProj12);
		TEST_ADD(TestInterval::sinProj13);
		TEST_ADD(TestInterval::sinProj14);
		TEST_ADD(TestInterval::sinProj15);
		TEST_ADD(TestInterval::sinProj16);
		TEST_ADD(TestInterval::sinProj17);
		TEST_ADD(TestInterval::sinProj18);
		TEST_ADD(TestInterval::sinProj19);
		TEST_ADD(TestInterval::sinProj20);
		TEST_ADD(TestInterval::sinProj21);
		TEST_ADD(TestInterval::sinProj22);
		TEST_ADD(TestInterval::sinProj23);

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

	    TEST_ADD(TestInterval::is_subset01);
	    TEST_ADD(TestInterval::is_subset02);
	    TEST_ADD(TestInterval::is_strict_subset01);
	    TEST_ADD(TestInterval::is_strict_subset02);
	    TEST_ADD(TestInterval::is_strict_subset03);
	    TEST_ADD(TestInterval::is_superset01);
	    TEST_ADD(TestInterval::is_superset02);
	    TEST_ADD(TestInterval::is_strict_superset01);
	    TEST_ADD(TestInterval::is_strict_superset02);
	    TEST_ADD(TestInterval::is_strict_superset03);
	    TEST_ADD(TestInterval::contains01);
	    TEST_ADD(TestInterval::contains02);
	    TEST_ADD(TestInterval::strictly_contains01);
	    TEST_ADD(TestInterval::strictly_contains02);
	    TEST_ADD(TestInterval::is_disjoint01);
	    TEST_ADD(TestInterval::is_disjoint02);
	    TEST_ADD(TestInterval::is_empty01);
	    TEST_ADD(TestInterval::is_empty02);
	    TEST_ADD(TestInterval::is_degenerated01);
	    TEST_ADD(TestInterval::is_degenerated02);
	    TEST_ADD(TestInterval::is_unbounded01);
	    TEST_ADD(TestInterval::is_unbounded02);
	    TEST_ADD(TestInterval::is_unbounded03);
	    TEST_ADD(TestInterval::is_unbounded04);
	}
private:

	/* test
	 * =========
	 *  Interval()
	 *  Interval(double a, double b)
	 *  Interval(double a)
	 *  Interval(const Interval&)
	 *  operator=(const Interval& x)
	 *  operator=(double x)
	 */
	void cons01();
	void cons02();
	void cons03();
	void cons04();
	void cons05();

	/* test
	 * =========
	 *  operator==(const Interval& x) const;
     *  operator!=(const Interval& x) const;
	 */
	void eq01();
	void eq02();
	void eq03();
	void eq04();
	void eq05();
	void eq06();
	void eq07();
	void eq08();

	/* test: set_empty() */
	void set_empty01();
	void set_empty02();

    /* test
     * =====
     *  operator&=(const Interval& x)
     * 	operator&(const Interval& x1, const Interval& x2)
     */
    void inter01();
    void inter02();
    void inter03();
    void inter04();
    void inter05();
    void inter06();

    /* test
     * =====
     *  operator|=(const Interval& x)
     * 	operator|(const Interval& x1, const Interval& x2)
     */
	void hull01();
	void hull02();
	void hull03();

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
	/* test
	 * =============
	 *  lb() const
	 *  ub() const
	 *  mid() const
	 *  rad() const;
	 *  diam() const;
     *  mig() const;
     *  mag() const;
     */
	void getters01();
	void getters02();
	void getters03();
	void getters04();
	void getters05();
	void getters06();

	/* test: is_subset */
    void is_subset01();
    void is_subset02();

	/* test: is_strict_subset */
    void is_strict_subset01();
    void is_strict_subset02();
    void is_strict_subset03();

	/* test: is_superset */
    void is_superset01();
    void is_superset02();

	/* test: is_strict_superset */
    void is_strict_superset01();
    void is_strict_superset02();
    void is_strict_superset03();

	/* test: contains */
    void contains01();
    void contains02();

	/* test: strictly_contains */
    void strictly_contains01();
    void strictly_contains02();

	/* test: is_disjoint */
    void is_disjoint01();
    void is_disjoint02();

	/* test: is_empty */
    void is_empty01();
    void is_empty02();

	/* test: is_degenerated */
    void is_degenerated01();
    void is_degenerated02();

	/* test: is_unbounded */
    void is_unbounded01();
    void is_unbounded02();
    void is_unbounded03();
    void is_unbounded04();

	/* test: rel_distance(const Interval& x) const */
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

	/* test: distance(const Interval &x1, const Interval &x2); */
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

	/* test: proj_sqrt(const Interval& y, Interval& x) */
	void sqrProj01();
	void sqrProj02();
	void sqrProj03();
	void sqrProj04();
	void sqrProj05();
	void sqrProj06();
	void sqrProj07();

	/* test: proj_sin & proj_cos */
	void sinProj01();
	void sinProj02();
	void sinProj03();
	void sinProj04();
	void sinProj05();
	void sinProj06();
	void sinProj07();
	void sinProj08();
	void sinProj09();
	void sinProj10();
	void sinProj11();
	void sinProj12();
	void sinProj13();
	void sinProj14();
	void sinProj15();
	void sinProj16();
	void sinProj17();
	void sinProj18();
	void sinProj19();
	void sinProj20();
	void sinProj21();
	void sinProj22();
	void sinProj23();

	void check_eq(const Interval& x, const Interval& y, bool);
	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_hull(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_inter(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
	void check_trigo(const Interval& x, const Interval& y);
	bool checkproj_mul(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkproj_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter);
};

#endif // __TEST_INTERVAL__
