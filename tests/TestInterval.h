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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestInterval : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestInterval);

	CPPUNIT_TEST(cons01);
	CPPUNIT_TEST(cons02);
	CPPUNIT_TEST(cons03);
	CPPUNIT_TEST(cons04);
	CPPUNIT_TEST(cons05);

	CPPUNIT_TEST(eq01);
	CPPUNIT_TEST(eq02);
	CPPUNIT_TEST(eq03);
	CPPUNIT_TEST(eq04);
	CPPUNIT_TEST(eq05);
	CPPUNIT_TEST(eq06);
	CPPUNIT_TEST(eq07);
	CPPUNIT_TEST(eq08);

	CPPUNIT_TEST(set_empty01);
	CPPUNIT_TEST(set_empty02);

	CPPUNIT_TEST(inter01);
	CPPUNIT_TEST(inter02);
	CPPUNIT_TEST(inter03);
	CPPUNIT_TEST(inter04);
	CPPUNIT_TEST(inter05);
	CPPUNIT_TEST(inter06);

	CPPUNIT_TEST(hull01);
	CPPUNIT_TEST(hull02);
	CPPUNIT_TEST(hull03);

	CPPUNIT_TEST(getters01);
	CPPUNIT_TEST(getters02);
	CPPUNIT_TEST(getters03);
	CPPUNIT_TEST(getters04);
	CPPUNIT_TEST(getters05);
	CPPUNIT_TEST(getters06);

	CPPUNIT_TEST(distance01);
	CPPUNIT_TEST(distance02);
	CPPUNIT_TEST(distance03);
	CPPUNIT_TEST(distance04);
	CPPUNIT_TEST(distance05);
	CPPUNIT_TEST(distance06);
	CPPUNIT_TEST(distance07);
	CPPUNIT_TEST(distance08);
	CPPUNIT_TEST(distance09);
	CPPUNIT_TEST(distance10);
	CPPUNIT_TEST(distance11);
	CPPUNIT_TEST(distance12);
	CPPUNIT_TEST(distance13);
	CPPUNIT_TEST(distance14);
	CPPUNIT_TEST(distance15);
	CPPUNIT_TEST(distance15b);
	CPPUNIT_TEST(distance16);
	CPPUNIT_TEST(distance17);
	CPPUNIT_TEST(distance18);
	CPPUNIT_TEST(distance19);
	CPPUNIT_TEST(distance20);
	CPPUNIT_TEST(distance21);
	CPPUNIT_TEST(distance22);
	CPPUNIT_TEST(distance23);

	CPPUNIT_TEST(rel_distance01);
	CPPUNIT_TEST(rel_distance02);
	CPPUNIT_TEST(rel_distance03);
	CPPUNIT_TEST(rel_distance04);
	CPPUNIT_TEST(rel_distance05);
	CPPUNIT_TEST(rel_distance06);
	CPPUNIT_TEST(rel_distance07);
	CPPUNIT_TEST(rel_distance08);
	CPPUNIT_TEST(rel_distance09);
	CPPUNIT_TEST(rel_distance10);
	CPPUNIT_TEST(rel_distance11);
	CPPUNIT_TEST(rel_distance12);
	CPPUNIT_TEST(rel_distance13);
	CPPUNIT_TEST(rel_distance14);
	CPPUNIT_TEST(rel_distance15);
	CPPUNIT_TEST(rel_distance16);
	CPPUNIT_TEST(rel_distance17);
	CPPUNIT_TEST(rel_distance18);
	CPPUNIT_TEST(rel_distance19);
	CPPUNIT_TEST(rel_distance20);
	CPPUNIT_TEST(rel_distance21);

	CPPUNIT_TEST(is_subset01);
	CPPUNIT_TEST(is_subset02);
	CPPUNIT_TEST(is_strict_interior_subset01);
	CPPUNIT_TEST(is_strict_interior_subset02);
	CPPUNIT_TEST(is_strict_interior_subset03);
	CPPUNIT_TEST(is_superset01);
	CPPUNIT_TEST(is_superset02);
	CPPUNIT_TEST(is_strict_superset01);
	CPPUNIT_TEST(is_strict_superset02);
	CPPUNIT_TEST(is_strict_superset03);
	CPPUNIT_TEST(contains01);
	CPPUNIT_TEST(contains02);
	CPPUNIT_TEST(interior_contains01);
	CPPUNIT_TEST(interior_contains02);
	CPPUNIT_TEST(is_disjoint01);
	CPPUNIT_TEST(is_disjoint02);
	CPPUNIT_TEST(is_empty01);
	CPPUNIT_TEST(is_empty02);
	CPPUNIT_TEST(is_degenerated01);
	CPPUNIT_TEST(is_degenerated02);
	CPPUNIT_TEST(is_unbounded01);
	CPPUNIT_TEST(is_unbounded02);
	CPPUNIT_TEST(is_unbounded03);
	CPPUNIT_TEST(is_unbounded04);

	CPPUNIT_TEST(is_bisectable01);
	CPPUNIT_TEST(is_bisectable02);
	CPPUNIT_TEST(is_bisectable03);
	CPPUNIT_TEST(is_bisectable04);
	CPPUNIT_TEST(is_bisectable05);
	CPPUNIT_TEST(is_bisectable06);
	CPPUNIT_TEST(is_bisectable07);
	CPPUNIT_TEST(is_bisectable08);
	CPPUNIT_TEST(is_bisectable09);
	CPPUNIT_TEST(is_bisectable10);

	CPPUNIT_TEST(integer01);
	CPPUNIT_TEST(integer02);
	CPPUNIT_TEST(integer03);
	CPPUNIT_TEST(integer04);
	CPPUNIT_TEST(integer05);
	CPPUNIT_TEST(integer06);
	CPPUNIT_TEST(integer07);
	CPPUNIT_TEST(integer08);

	CPPUNIT_TEST(mid01);
	CPPUNIT_TEST(mid02);
	CPPUNIT_TEST(mid03);
	CPPUNIT_TEST(mid04);
	CPPUNIT_TEST(mid05);
	CPPUNIT_TEST(mid06);
	CPPUNIT_TEST(mid07);
	CPPUNIT_TEST(mid08);

	CPPUNIT_TEST(delta01);
	CPPUNIT_TEST(delta02);
	CPPUNIT_TEST(delta03);

	CPPUNIT_TEST(diff01);
	CPPUNIT_TEST(diff02);
	CPPUNIT_TEST(diff03);
	CPPUNIT_TEST(diff04);
	CPPUNIT_TEST(diff05);
	CPPUNIT_TEST(diff06);
	CPPUNIT_TEST(diff07);
	CPPUNIT_TEST(diff08);
	CPPUNIT_TEST(diff09);
	CPPUNIT_TEST(diff10);
	CPPUNIT_TEST(diff11);
	CPPUNIT_TEST(diff12);
	CPPUNIT_TEST(diff13);
	CPPUNIT_TEST(diff14);
	CPPUNIT_TEST(diff15);
	CPPUNIT_TEST(diff16);
	CPPUNIT_TEST(diff17);

	CPPUNIT_TEST_SUITE_END();
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

	/* test: is_strict_interior_subset */
    void is_strict_interior_subset01();
    void is_strict_interior_subset02();
    void is_strict_interior_subset03();

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

	/* test: interior_contains */
    void interior_contains01();
    void interior_contains02();

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
    void is_bisectable06();
    void is_bisectable07();
    void is_bisectable08();
    void is_bisectable09();
    void is_bisectable10();

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

	// [0,0] \ [0,0]
	void diff01();
	// [0,0] \ [1,2]
	void diff02();
	// [0,0] \ [0,1]
	void diff03();
	// [0,0] \ empty
	void diff04();
	// empty \ [0,0]
	void diff05();
	// [0,1] \ [0,1]
	void diff06();
	// [0,1] \ empty
	void diff07();
	// [0,1] \ [1,2]
	void diff08();
	// [0,1] \ [-1,1]
	void diff09();
	// [0,1] \ [0,0]
	void diff10();
	// [0,1] \ [1,1]
	void diff11();
	// [0,1] \ [2,3]
	void diff12();
	// [0,3] \ [1,2]
	void diff13();
	// [0,2] \ [1,4]
	void diff14();
	// [0,2] \ [-1,1]
	void diff15();
	// [0,2] \ [1,1]
	void diff16();
	// [0,2] \ [1,1]
	void diff17();

	void check_eq(const Interval& x, const Interval& y, bool);
	void check_hull(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_inter(const Interval& x, const Interval& z, const Interval& y_expected);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestInterval);


#endif // __TEST_INTERVAL__
