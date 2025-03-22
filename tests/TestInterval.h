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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestInterval : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();

		cons01();
		cons02();
		cons03();
		cons04();
		cons05();
		consCppArray();

		eq01();
		eq02();
		eq03();
		eq04();
		eq05();
		eq06();
		eq07();
		eq08();

		set_empty01();
		set_empty02();

		inter01();
		inter02();
		inter03();
		inter04();
		inter05();
		inter06();

		hull01();
		hull02();
		hull03();

		getters01();
		getters02();
		getters03();
		getters04();
		getters05();
		getters06();

		distance01();
		distance02();
		distance03();
		distance04();
		distance05();
		distance06();
		distance07();
		distance08();
		distance09();
		distance10();
		distance11();
		distance12();
		distance13();
		distance14();
		distance15();
		distance15b();
		distance16();
		distance17();
		distance18();
		distance19();
		distance20();
		distance21();
		distance22();
		distance23();

		rel_distance01();
		rel_distance02();
		rel_distance03();
		rel_distance04();
		rel_distance05();
		rel_distance06();
		rel_distance07();
		rel_distance08();
		rel_distance09();
		rel_distance10();
		rel_distance11();
		rel_distance12();
		rel_distance13();
		rel_distance14();
		rel_distance15();
		rel_distance16();
		rel_distance17();
		rel_distance18();
		rel_distance19();
		rel_distance20();
		rel_distance21();

		is_subset01();
		is_subset02();
		is_strict_interior_subset01();
		is_strict_interior_subset02();
		is_strict_interior_subset03();
		is_superset01();
		is_superset02();
		is_strict_superset01();
		is_strict_superset02();
		is_strict_superset03();
		contains01();
		contains02();
		interior_contains01();
		interior_contains02();
		is_disjoint01();
		is_disjoint02();
		is_empty01();
		is_empty02();
		is_degenerated01();
		is_degenerated02();
		is_unbounded01();
		is_unbounded02();
		is_unbounded03();
		is_unbounded04();

		is_bisectable01();
		is_bisectable02();
		is_bisectable03();
		is_bisectable04();
		is_bisectable05();
		is_bisectable06();
		is_bisectable07();
		is_bisectable08();
		is_bisectable09();
		is_bisectable10();

		mid01();
		mid02();
		mid03();
		mid04();
		mid05();
		mid06();
		mid07();
		mid08();

		delta01();
		delta02();
		delta03();

		diff01();
		diff02();
		diff03();
		diff04();
		diff05();
		diff06();
		diff07();
		diff08();
		diff09();
		diff10();
		diff11();
		diff12();
		diff13();
		diff14();
		diff15();
		diff16();
		diff17();
		issue_463();
		tearDown();
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
	/* Uses C++ fixed-size array */
	void consCppArray();

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

	void issue_463();
};




#endif // __TEST_INTERVAL__
