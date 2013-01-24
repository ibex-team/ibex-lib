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

		TEST_ADD(TestInterval::is_bisectable01);
		TEST_ADD(TestInterval::is_bisectable02);
		TEST_ADD(TestInterval::is_bisectable03);
		TEST_ADD(TestInterval::is_bisectable04);
		TEST_ADD(TestInterval::is_bisectable05);

		TEST_ADD(TestInterval::integer01);
		TEST_ADD(TestInterval::integer02);
		TEST_ADD(TestInterval::integer03);
		TEST_ADD(TestInterval::integer04);
		TEST_ADD(TestInterval::integer05);
		TEST_ADD(TestInterval::integer06);
		TEST_ADD(TestInterval::integer07);
		TEST_ADD(TestInterval::integer08);

		TEST_ADD(TestInterval::mid01);
		TEST_ADD(TestInterval::mid02);
		TEST_ADD(TestInterval::mid03);
		TEST_ADD(TestInterval::mid04);
		TEST_ADD(TestInterval::mid05);
		TEST_ADD(TestInterval::mid06);
		TEST_ADD(TestInterval::mid07);
		TEST_ADD(TestInterval::mid08);

	    TEST_ADD(TestInterval::delta01);
	    TEST_ADD(TestInterval::delta02);
	    TEST_ADD(TestInterval::delta03);
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

    void is_bisectable01();
    void is_bisectable02();
    void is_bisectable03();
    void is_bisectable04();
    void is_bisectable05();

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

	void integer01();
	void integer02();
	void integer03();
	void integer04();
	void integer05();
	void integer06();
	void integer07();
	void integer08();

	void mid01();
	void mid02();
	void mid03();
	void mid04();
	void mid05();
	void mid06();
	void mid07();
	void mid08();

	void delta01();
	void delta02();
	void delta03();

	void check_eq(const Interval& x, const Interval& y, bool);
	void check_hull(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_inter(const Interval& x, const Interval& z, const Interval& y_expected);
};

#endif // __TEST_INTERVAL__
