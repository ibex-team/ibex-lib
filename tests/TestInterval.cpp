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

#include "TestInterval.h"
#include "utils.h"
#include <float.h>

using namespace std;
void TestInterval::cons01() {
	check(Interval(),Interval(NEG_INFINITY,POS_INFINITY));
	check((Interval())=Interval(),Interval(NEG_INFINITY,POS_INFINITY));
}
void TestInterval::cons02() {
	check(Interval(1.0),Interval(1.0,1.0));
	check((Interval())=1.0, Interval(1.0,1.0));
}
void TestInterval::cons03() {
	check(Interval(NEG_INFINITY),Interval::EMPTY_SET);
	check((Interval())=NEG_INFINITY, Interval::EMPTY_SET);
}
void TestInterval::cons04() {
	check(Interval(POS_INFINITY),Interval::EMPTY_SET);
	check((Interval())=POS_INFINITY, Interval::EMPTY_SET);
}
void TestInterval::cons05() {
	check(Interval(1,0), Interval::EMPTY_SET);
	check((Interval())=Interval(1,0), Interval::EMPTY_SET);
} // reverse bounds

void TestInterval::check_eq(const Interval& x, const Interval& y, bool is_eq) {
	TEST_ASSERT(is_eq? x==y : !(x==y));
	TEST_ASSERT(is_eq? !(x!=y) : x!=y);
	TEST_ASSERT(is_eq? y==x : !(y==x));
	TEST_ASSERT(is_eq? !(y!=x) : y!=x);
}

void TestInterval::eq01() { check_eq(Interval::ALL_REALS,Interval::ALL_REALS, true); }
void TestInterval::eq02() { check_eq(Interval::ALL_REALS,Interval::POS_REALS, false); }
void TestInterval::eq03() { check_eq(Interval(0,1), Interval(0,1), true); }
void TestInterval::eq04() { check_eq(Interval(0,2), Interval(0,1), false); }
void TestInterval::eq05() { check_eq(Interval(0,2), Interval(1,3), false); }
void TestInterval::eq06() { check_eq(Interval::POS_REALS,Interval::POS_REALS, true); }
void TestInterval::eq07() { check_eq(Interval::EMPTY_SET,Interval::EMPTY_SET, true); }
void TestInterval::eq08() { check_eq(Interval(0,0),Interval(0,0), true); }

void TestInterval::set_empty01() { Interval x(0,1); x.set_empty(); check(x,Interval::EMPTY_SET); }
void TestInterval::set_empty02() { Interval x(Interval::EMPTY_SET); x.set_empty(); check(x,Interval::EMPTY_SET); }

void TestInterval::distance01() { check(ibex::distance(Interval(0,10), Interval(-5,5)), 5); }
void TestInterval::distance02() { check(ibex::distance(Interval(0,10), Interval(5,10)), 5); }
void TestInterval::distance03() { check(ibex::distance(Interval(0,10), Interval(5,15)), 5); }
void TestInterval::distance04() { check(ibex::distance(Interval(0,10), Interval(1,2)), 8); /* 9 */}
void TestInterval::distance05() { check(ibex::distance(Interval(0,10), Interval(0,10)), 0); }
void TestInterval::distance06() { check(ibex::distance(Interval(0,10), Interval(-10,20)), 10); /* 0 */}

/* with infinite bounds */
void TestInterval::distance07() { check(ibex::distance(Interval::POS_REALS, Interval(0,10)), POS_INFINITY); }
void TestInterval::distance08() { check(ibex::distance(Interval::ALL_REALS, Interval(0,10)), POS_INFINITY); }
void TestInterval::distance09() { check(ibex::distance(Interval::ALL_REALS, Interval::ALL_REALS), 0); }
void TestInterval::distance10() { check(ibex::distance(Interval::ALL_REALS, Interval::NEG_REALS), POS_INFINITY); }
void TestInterval::distance11() { check(ibex::distance(Interval::ALL_REALS, Interval::POS_REALS), POS_INFINITY); }
void TestInterval::distance12() { check(ibex::distance(Interval::POS_REALS, Interval(NEG_INFINITY,1)), POS_INFINITY); }
void TestInterval::distance13() { check(ibex::distance(Interval::POS_REALS, Interval(0,1)), POS_INFINITY); }
void TestInterval::distance14() { check(ibex::distance(Interval::NEG_REALS, Interval(-1,POS_INFINITY)), POS_INFINITY); }
void TestInterval::distance15() { check(ibex::distance(Interval::NEG_REALS, Interval(NEG_INFINITY,-1)), 1); }
void TestInterval::distance15b(){ check(ibex::distance(Interval::POS_REALS, Interval(1,POS_INFINITY)), 1); }

/* with degenerated intervals */
void TestInterval::distance16() { check(ibex::distance(Interval(0,10), Interval(5,5)), 5); /* 10 */}
void TestInterval::distance17() { check(ibex::distance(Interval(0,10), Interval(0,0)), 10); }
void TestInterval::distance18() { check(ibex::distance(Interval(0,10), Interval(10,10)), 10); }
void TestInterval::distance19() { check(ibex::distance(Interval(0,0), Interval(0,0)), 0); }

/* with empty intervals */

void TestInterval::distance20() { check(ibex::distance(Interval(0,10), Interval::EMPTY_SET), 5); /* 10 */}
void TestInterval::distance21() { check(ibex::distance(Interval(0,0), Interval::EMPTY_SET), 0); }

void TestInterval::distance22() { check(ibex::distance(Interval(0,10), Interval(11,20)), 11); }
void TestInterval::distance23() { check(ibex::distance(Interval(0,10), Interval(0,5)), 5); }

void TestInterval::rel_distance01() { check(Interval(0,10).rel_distance(Interval(-5,5)), 0.5); }
void TestInterval::rel_distance02() { check(Interval(0,10).rel_distance(Interval(5,10)), 0.5); }
void TestInterval::rel_distance03() { check(Interval(0,10).rel_distance(Interval(5,15)), 0.5); }
void TestInterval::rel_distance04() { check(Interval(0,10).rel_distance(Interval(1,2)), 0.8); }
void TestInterval::rel_distance05() { check(Interval(0,10).rel_distance(Interval(0,10)), 0); }
void TestInterval::rel_distance06() { check(Interval(0,10).rel_distance(Interval(-10,20)), 1);/*0*/ }

/* with infinite bounds */
void TestInterval::rel_distance07() { check(Interval::POS_REALS.rel_distance(Interval(0,10)), 1); }
void TestInterval::rel_distance08() { check(Interval::ALL_REALS.rel_distance(Interval(0,10)), 1); }
void TestInterval::rel_distance09() { check(Interval::ALL_REALS.rel_distance(Interval::ALL_REALS), 0); }
void TestInterval::rel_distance10() { check(Interval::ALL_REALS.rel_distance(Interval::NEG_REALS), 1); }
void TestInterval::rel_distance11() { check(Interval::ALL_REALS.rel_distance(Interval::POS_REALS), 1); }
void TestInterval::rel_distance12() { check(Interval::POS_REALS.rel_distance(Interval(NEG_INFINITY,1)), 1); }
void TestInterval::rel_distance13() { check(Interval::POS_REALS.rel_distance(Interval(0,1)), 1); }
void TestInterval::rel_distance14() { check(Interval::NEG_REALS.rel_distance(Interval(-1,POS_INFINITY)), 1); }
void TestInterval::rel_distance15() { check(Interval::NEG_REALS.rel_distance(Interval(NEG_INFINITY,-1)), 0); }

/* with degenerated intervals */
void TestInterval::rel_distance16() { check(Interval(0,10).rel_distance(Interval(5,5)), 0.5); /*1*/}
void TestInterval::rel_distance17() { check(Interval(0,10).rel_distance(Interval(0,0)), 1); }
void TestInterval::rel_distance18() { check(Interval(0,10).rel_distance(Interval(10,10)), 1); }
void TestInterval::rel_distance19() { check(Interval(0,0).rel_distance(Interval(0,0)), 0); }
void TestInterval::rel_distance20() { check(Interval(1,4).rel_distance(Interval(1.5,3)), 1.0/3.0); }

void TestInterval::rel_distance21() { check(Interval(0,10).rel_distance(Interval(0,5)), 0.5); }

void TestInterval::check_hull(const Interval& x, const Interval& z, const Interval& y_expected) {
	check(x|z, y_expected);
	check(z|x, y_expected);
    check((Interval(x)|=z), y_expected);
    check((Interval(z)|=x), y_expected);
}

void TestInterval::hull01() { check_hull(Interval(0,1), Interval(2,3), Interval(0,3)); }
void TestInterval::hull02() { check_hull(Interval(0,1), Interval::EMPTY_SET, Interval(0,1)); }
void TestInterval::hull03() { check_hull(Interval(-1), Interval::POS_REALS, Interval(-1,POS_INFINITY)); }

void TestInterval::check_inter(const Interval& x, const Interval& z, const Interval& y_expected) {
	check(x&z, y_expected);
	check(z&x, y_expected);
    check((Interval(x)&=z), y_expected);
    check((Interval(z)&=x), y_expected);
}

void TestInterval::inter01() { check_inter(Interval(0,1), Interval(2,3), Interval::EMPTY_SET); }
void TestInterval::inter02() { check_inter(Interval(0,1), Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestInterval::inter03() { check_inter(Interval(0,2), Interval(-1,1), Interval(0,1)); }
void TestInterval::inter04() { check_inter(Interval(0,1), Interval::POS_REALS, Interval(0,1)); }
void TestInterval::inter05() { check_inter(Interval(0,1), Interval::NEG_REALS, Interval::ZERO); }
void TestInterval::inter06() { check_inter(Interval(0,1), Interval(1,2), Interval(1,1)); }

void TestInterval::getters01() {
	Interval x(0,2);
	TEST_ASSERT_DELTA_MSG(x.lb(),0,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),2,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),1,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),1,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),2,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),0,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),2,ERROR,"mag");
}

void TestInterval::getters02() {
	Interval x(-3,-1);
	TEST_ASSERT_DELTA_MSG(x.lb(),-3,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),-1,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),-2,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),1,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),2,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),1,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),3,ERROR,"mag");
}

void TestInterval::getters03() {
	Interval x(-3,1);
	TEST_ASSERT_DELTA_MSG(x.lb(),-3,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),1,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),-1,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),2,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),4,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),0,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),3,ERROR,"mag");
}

void TestInterval::getters04() {
	Interval x(Interval::NEG_REALS);
	TEST_ASSERT_DELTA_MSG(x.lb(),NEG_INFINITY,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),0,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),-DBL_MAX,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),POS_INFINITY,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),POS_INFINITY,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),0,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),POS_INFINITY,ERROR,"mag");
}

void TestInterval::getters05() {
	Interval x(Interval::ALL_REALS);
	TEST_ASSERT_DELTA_MSG(x.lb(),NEG_INFINITY,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),POS_INFINITY,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),0,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),POS_INFINITY,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),POS_INFINITY,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),0,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),POS_INFINITY,ERROR,"mag");
}

void TestInterval::getters06() {
	Interval x(DBL_MAX,POS_INFINITY);
	TEST_ASSERT_DELTA_MSG(x.lb(),DBL_MAX,ERROR,"lb");
	TEST_ASSERT_DELTA_MSG(x.ub(),POS_INFINITY,ERROR,"ub");
	TEST_ASSERT_DELTA_MSG(x.mid(),DBL_MAX,ERROR,"mid");
	TEST_ASSERT_DELTA_MSG(x.rad(),POS_INFINITY,ERROR,"rad");
	TEST_ASSERT_DELTA_MSG(x.diam(),POS_INFINITY,ERROR,"diam");
	TEST_ASSERT_DELTA_MSG(x.mig(),DBL_MAX,ERROR,"mig");
	TEST_ASSERT_DELTA_MSG(x.mag(),POS_INFINITY,ERROR,"mag");
}


void TestInterval::is_subset01()          { TEST_ASSERT(Interval(0,1).is_subset(Interval(0,2))); }
void TestInterval::is_subset02()          { TEST_ASSERT(!Interval(0,1).is_subset(Interval(1,2))); }
void TestInterval::is_strict_subset01()   { TEST_ASSERT(!Interval(0,1).is_strict_subset(Interval(0,2))); }
void TestInterval::is_strict_subset02()   { TEST_ASSERT(Interval::NEG_REALS.is_strict_subset(Interval::ALL_REALS)); }
void TestInterval::is_strict_subset03()   { TEST_ASSERT(Interval::EMPTY_SET.is_strict_subset(Interval::EMPTY_SET)); }
void TestInterval::is_superset01()        { TEST_ASSERT(Interval(0,2).is_superset(Interval(0,1))); }
void TestInterval::is_superset02()        { TEST_ASSERT(!Interval(1,2).is_superset(Interval(0,1))); }
void TestInterval::is_strict_superset01() { TEST_ASSERT(!Interval(0,2).is_strict_superset(Interval(0,1))); }
void TestInterval::is_strict_superset02() { TEST_ASSERT(Interval::ALL_REALS.is_strict_superset(Interval::NEG_REALS)); }
void TestInterval::is_strict_superset03() { TEST_ASSERT(Interval::EMPTY_SET.is_strict_superset(Interval::EMPTY_SET)); }
void TestInterval::contains01()           { TEST_ASSERT(Interval(0,2).contains(0)); }
void TestInterval::contains02()           { TEST_ASSERT(Interval::ALL_REALS.contains(POS_INFINITY)); }
void TestInterval::strictly_contains01()  { TEST_ASSERT(!Interval(0,2).strictly_contains(0)); }
void TestInterval::strictly_contains02()  { TEST_ASSERT(Interval(0,2).strictly_contains(1)); }
void TestInterval::is_disjoint01()        { TEST_ASSERT(!Interval::NEG_REALS.is_disjoint(Interval::POS_REALS)); }
void TestInterval::is_disjoint02()        { TEST_ASSERT(Interval(0,1).is_disjoint(Interval(2,3))); }
void TestInterval::is_empty01()           { TEST_ASSERT(Interval(POS_INFINITY).is_empty()); }
void TestInterval::is_empty02()           { TEST_ASSERT(!Interval(0,1).is_empty()); }
void TestInterval::is_degenerated01()     { TEST_ASSERT(Interval::ZERO.is_degenerated()); }
void TestInterval::is_degenerated02()     { TEST_ASSERT(Interval::EMPTY_SET.is_degenerated()); }
void TestInterval::is_unbounded01()       { TEST_ASSERT(!Interval::EMPTY_SET.is_unbounded()); }
void TestInterval::is_unbounded02()       { TEST_ASSERT(!Interval(0,1).is_unbounded()); }
void TestInterval::is_unbounded03()       { TEST_ASSERT(Interval::NEG_REALS.is_unbounded()); }
void TestInterval::is_unbounded04()       { TEST_ASSERT(Interval::POS_REALS.is_unbounded()); }

void TestInterval::integer01() {
	Interval x=Interval::EMPTY_SET; proj_integer(x); check(x,Interval::EMPTY_SET);
}
void TestInterval::integer02() {
	Interval x(NEG_INFINITY,-0.000001); proj_integer(x); check(x,Interval(NEG_INFINITY,-1));
}
void TestInterval::integer03() {
	Interval x(0.00000001,POS_INFINITY); proj_integer(x); check(x,Interval(1.0,POS_INFINITY));
}
void TestInterval::integer04() {
	Interval x=Interval::ALL_REALS; proj_integer(x); check(x,Interval::ALL_REALS);
}

void TestInterval::integer05() {
	Interval x(0.01,2.99); proj_integer(x); check(x,Interval(1,2));
}

void TestInterval::integer06() {
	Interval x(-0.01,2.99); proj_integer(x); check(x,Interval(0,2));
}

void TestInterval::integer07() {
	Interval x(1.000000001,1.9999999999); proj_integer(x); check(x,Interval::EMPTY_SET);
}

void TestInterval::integer08() {
	Interval x(1e8, DBL_MAX); proj_integer(x); check(x, Interval(1e8,DBL_MAX));
}

void TestInterval::delta01() {
	Interval x(-1.7976931348623157081e+308,6.1000000000000005329);
	//cout << "delta=" << x.delta(x) << " x.diam()=" << (x.diam()==POS_INFINITY) << endl;
	TEST_ASSERT(x.delta(x)==0.0);
}
