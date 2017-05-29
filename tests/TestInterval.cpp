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
	CPPUNIT_ASSERT(is_eq? x==y : !(x==y));
	CPPUNIT_ASSERT(is_eq? !(x!=y) : x!=y);
	CPPUNIT_ASSERT(is_eq? y==x : !(y==x));
	CPPUNIT_ASSERT(is_eq? !(y!=x) : y!=x);
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
	//std::cout << x<< "  " << z<< "  "<<  (x | z) << std::endl;
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
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",0,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",2,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",1,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",1,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",2,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",2,x.mag(), ERROR);
}

void TestInterval::getters02() {
	Interval x(-3,-1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",-3,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",-1,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-2,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",1,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",2,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",1,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",3,x.mag(), ERROR);
}

void TestInterval::getters03() {
	Interval x(-3,1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",-3,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",1,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-1,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",2,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",4,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",3,x.mag(), ERROR);
}

void TestInterval::getters04() {
	Interval x(Interval::NEG_REALS);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",NEG_INFINITY,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",0,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",-DBL_MAX,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}

void TestInterval::getters05() {
	Interval x(Interval::ALL_REALS);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",NEG_INFINITY,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",POS_INFINITY,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",0,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}

void TestInterval::getters06() {
	Interval x(DBL_MAX,POS_INFINITY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("lb",DBL_MAX,x.lb(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ub",POS_INFINITY,x.ub(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mid",DBL_MAX,x.mid(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("rad",POS_INFINITY,x.rad(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("diam",POS_INFINITY,x.diam(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mig",DBL_MAX,x.mig(), ERROR);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mag",POS_INFINITY,x.mag(), ERROR);
}


void TestInterval::is_subset01()          { CPPUNIT_ASSERT(Interval(0,1).is_subset(Interval(0,2))); }
void TestInterval::is_subset02()          { CPPUNIT_ASSERT(!Interval(0,1).is_subset(Interval(1,2))); }
void TestInterval::is_strict_interior_subset01()   { CPPUNIT_ASSERT(!Interval(0,1).is_strict_interior_subset(Interval(0,2))); }
void TestInterval::is_strict_interior_subset02()   { CPPUNIT_ASSERT(Interval::NEG_REALS.is_strict_interior_subset(Interval::ALL_REALS)); }
void TestInterval::is_strict_interior_subset03()   { CPPUNIT_ASSERT(Interval::EMPTY_SET.is_strict_interior_subset(Interval(0,1))); }
void TestInterval::is_superset01()        { CPPUNIT_ASSERT(Interval(0,2).is_superset(Interval(0,1))); }
void TestInterval::is_superset02()        { CPPUNIT_ASSERT(!Interval(1,2).is_superset(Interval(0,1))); }
void TestInterval::is_strict_superset01() { CPPUNIT_ASSERT(!Interval(0,2).is_strict_superset(Interval(0,2))); }
void TestInterval::is_strict_superset02() { CPPUNIT_ASSERT(Interval::ALL_REALS.is_strict_superset(Interval::NEG_REALS)); }
void TestInterval::is_strict_superset03() { CPPUNIT_ASSERT(!Interval::EMPTY_SET.is_strict_superset(Interval::EMPTY_SET)); }
void TestInterval::contains01()           { CPPUNIT_ASSERT(Interval(0,2).contains(0)); }
void TestInterval::contains02()           { CPPUNIT_ASSERT(Interval::ALL_REALS.contains(POS_INFINITY)); }
void TestInterval::interior_contains01()  { CPPUNIT_ASSERT(!Interval(0,2).interior_contains(0)); }
void TestInterval::interior_contains02()  { CPPUNIT_ASSERT(Interval(0,2).interior_contains(1)); }
void TestInterval::is_disjoint01()        { CPPUNIT_ASSERT(!Interval::NEG_REALS.is_disjoint(Interval::POS_REALS)); }
void TestInterval::is_disjoint02()        { CPPUNIT_ASSERT(Interval(0,1).is_disjoint(Interval(2,3))); }
void TestInterval::is_empty01()           { CPPUNIT_ASSERT(Interval(POS_INFINITY).is_empty()); }
void TestInterval::is_empty02()           { CPPUNIT_ASSERT(!Interval(0,1).is_empty()); }
void TestInterval::is_degenerated01()     { CPPUNIT_ASSERT(Interval::ZERO.is_degenerated()); }
void TestInterval::is_degenerated02()     { CPPUNIT_ASSERT(Interval::EMPTY_SET.is_degenerated()); }
void TestInterval::is_unbounded01()       { CPPUNIT_ASSERT(!Interval::EMPTY_SET.is_unbounded()); }
void TestInterval::is_unbounded02()       { CPPUNIT_ASSERT(!Interval(0,1).is_unbounded()); }
void TestInterval::is_unbounded03()       { CPPUNIT_ASSERT(Interval::NEG_REALS.is_unbounded()); }
void TestInterval::is_unbounded04()       { CPPUNIT_ASSERT(Interval::POS_REALS.is_unbounded()); }
void TestInterval::is_bisectable01()      { CPPUNIT_ASSERT(!Interval::EMPTY_SET.is_bisectable()); }
void TestInterval::is_bisectable02()      { CPPUNIT_ASSERT(!Interval(DBL_MAX,POS_INFINITY).is_bisectable()); }
void TestInterval::is_bisectable03()      { CPPUNIT_ASSERT(!Interval(NEG_INFINITY,-DBL_MAX).is_bisectable()); }
void TestInterval::is_bisectable04()      { CPPUNIT_ASSERT(!Interval(0,ibex::next_float(0)).is_bisectable()); }
void TestInterval::is_bisectable05()      {	CPPUNIT_ASSERT(Interval(0,ibex::next_float(ibex::next_float(0))).is_bisectable());}
void TestInterval::is_bisectable06()      {CPPUNIT_ASSERT(Interval(10,ibex::next_float(ibex::next_float(10))).is_bisectable());}
void TestInterval::is_bisectable07()      {CPPUNIT_ASSERT(Interval(ibex::previous_float(ibex::previous_float(0)),0).is_bisectable());}
void TestInterval::is_bisectable08()      {CPPUNIT_ASSERT(Interval(ibex::previous_float(ibex::previous_float(10)),10).is_bisectable());}
void TestInterval::is_bisectable09()      {CPPUNIT_ASSERT(!Interval(10,ibex::next_float(10)).is_bisectable()); }
void TestInterval::is_bisectable10()      {CPPUNIT_ASSERT(!Interval(ibex::previous_float(0),0).is_bisectable()); }

void TestInterval::integer01() {
	Interval x=Interval::EMPTY_SET; bwd_integer(x); check(x,Interval::EMPTY_SET);
}
void TestInterval::integer02() {
	Interval x(NEG_INFINITY,-0.000001); bwd_integer(x); check(x,Interval(NEG_INFINITY,-1));
}
void TestInterval::integer03() {
	Interval x(0.00000001,POS_INFINITY); bwd_integer(x); check(x,Interval(1.0,POS_INFINITY));
}
void TestInterval::integer04() {
	Interval x=Interval::ALL_REALS; bwd_integer(x); check(x,Interval::ALL_REALS);
}

void TestInterval::integer05() {
	Interval x(0.01,2.99); bwd_integer(x); check(x,Interval(1,2));
}

void TestInterval::integer06() {
	Interval x(-0.01,2.99); bwd_integer(x); check(x,Interval(0,2));
}

void TestInterval::integer07() {
	Interval x(1.000000001,1.9999999999); bwd_integer(x); check(x,Interval::EMPTY_SET);
}

void TestInterval::integer08() {
	Interval x(1e8, DBL_MAX); bwd_integer(x); check(x, Interval(1e8,DBL_MAX));
}

void TestInterval::mid01() {
	check(Interval(0,2).mid(),1.0);
}

void TestInterval::mid02() {
	CPPUNIT_ASSERT(Interval(NEG_INFINITY,POS_INFINITY).mid()==0);
}

void TestInterval::mid03() {
	CPPUNIT_ASSERT(Interval::POS_REALS.mid()==DBL_MAX);
}

void TestInterval::mid04() {
	CPPUNIT_ASSERT(Interval::NEG_REALS.mid()==-DBL_MAX);
}

void TestInterval::mid05() {
	double d=Interval(DBL_MAX,POS_INFINITY).mid();
	CPPUNIT_ASSERT(d==DBL_MAX || d==-DBL_MAX);
}

void TestInterval::mid06() {
	CPPUNIT_ASSERT(Interval::ZERO.mid()==0.0);
}

void TestInterval::mid07() {
	Interval x(2.7629416257693772963,2.7629416257693835135);
	Interval y=x;
	//cout.precision(15);
	//cout << "x=" << y << endl;
	static double m=x.mid();
	//cout << "milieu = " << m << endl;
	Interval x2=x+m;
	//cout << "x+m=" << x2 << endl;
	Interval x3=x2-m;
	//cout << "(x+m)-m" <<x3 << endl;
	CPPUNIT_ASSERT(x3.is_superset(y));
}

void TestInterval::mid08() {
	double _box[][2]= {
			{3.4836061062102725572,3.4836061062102761099},
			{3.5754039600153961764,3.5754039600154006173},
			{3.2401050837195057852,3.2401050837195093379},
			{3.5187713541601914891,3.5187713541601937095},
			{3.4109008703180077049,3.410900870318013034},
			{3.336209585763303842,3.3362095857633069507},
			{3.3970878856415889224,3.3970878856415973601},
			{3.0865914745327103219,3.0865914745327134305},
			{3.1673998478272404888,3.1673998478272467061},
			{3.2795769799080982665,3.2795769799081013751},
			{2.9966040612287390132,2.996604061228742566},
			{3.4621387483049854694,3.4621387483049890221},
			{0.39999999999999980016,0.40000000000000074385},
			{1.9595917942265419498,1.959591794226542838},
			{2.4033959387712586775,2.4033959387712604538},
			{-1.5025469540784459621,-1.5025469540784437417},
			{3.4474903944979735648,3.4474903944979780057},
			{2.8417250066344248438,2.8417250066344270643},
			{3.6343497218046629804,3.6343497218046700858},
			{3.0078080936676636625,3.0078080936676689916},
			{3.8716115579677374825,3.8716115579677432557},
			{2.9290247128026156709,2.929024712802626329},
			{3.6847522306610471787,3.6847522306610511755},
			{2.7629416257693772963,2.7629416257693835135},
			{3.4978929033543559868,3.4978929033543604277},
			{2.5968585387361389216,2.5968585387361411421},
			{5,5},
			{-0,0},
			{-0,0},
			{-0,0} };

	IntervalVector box(30,_box);
	CPPUNIT_ASSERT((box+box.mid()-box.mid()).is_superset(box));
}

void TestInterval::delta01() {
	Interval x(-1.7976931348623157081e+308,6.1000000000000005329);
	//cout << "delta=" << x.delta(x) << " x.diam()=" << (x.diam()==POS_INFINITY) << endl;
	CPPUNIT_ASSERT(x.delta(x)==0.0);
}

void TestInterval::delta02() {
	double a=-3.6841361702068902e+17; // == DBL_MAX
	Interval x(a,POS_INFINITY);
	//cout << "delta=" << x.delta(x) << " x.diam()=" << (x.diam()==POS_INFINITY) << endl;
	Interval y(Interval::ALL_REALS);
	Interval z(NEG_INFINITY,a);
	//cout << y.delta(x) << " " << z.diam() << endl;
	CPPUNIT_ASSERT(y.delta(x)==z.diam());
}

void TestInterval::delta03() {
	Interval x(-DBL_MAX,DBL_MAX); // so that diam()>DBL_MAX
	//cout << x.diam() << " " << x.diam() << endl;
	//cout << "delta=" << x.delta(x) << endl;
	CPPUNIT_ASSERT(x.delta(x)==0);
}

void TestInterval::diff01() {
	Interval c1,c2;
	Interval::ZERO.diff(Interval::ZERO,c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
 }


void TestInterval::diff02() {
	Interval c1,c2;
	Interval(0,0).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval::ZERO && c2.is_empty());
}

void TestInterval::diff03() {
	Interval c1,c2;
	Interval(0,0).diff(Interval(0,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void TestInterval::diff04() {
	Interval c1,c2;
	Interval(0,0).diff(Interval::EMPTY_SET,c1,c2);
	CPPUNIT_ASSERT(c1==Interval::ZERO && c2.is_empty());
}

void TestInterval::diff05() {
	Interval c1,c2;
	Interval::EMPTY_SET.diff(Interval::ZERO,c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void TestInterval::diff06() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(0,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void TestInterval::diff07() {
	Interval c1,c2;
	Interval(0,1).diff(Interval::EMPTY_SET,c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff08() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff09() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(-1,1),c1,c2);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}

void TestInterval::diff10() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(0,0),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff11() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(1,1),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff12() {
	Interval c1,c2;
	Interval(0,1).diff(Interval(2,3),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff13() {
	Interval c1,c2;
	Interval(0,3).diff(Interval(1,2),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2==Interval(2,3));
}

void TestInterval::diff14() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(1,4),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2.is_empty());
}

void TestInterval::diff15() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(-1,1),c1,c2);
	CPPUNIT_ASSERT(c1==Interval(1,2) && c2.is_empty());
}

void TestInterval::diff16() {
	Interval c1,c2;
	Interval(0,2).diff(Interval(1,1),c1,c2,false);
	CPPUNIT_ASSERT(c1==Interval(0,1) && c2==Interval(1,2));
}

void TestInterval::diff17() {
	Interval c1,c2;
	Interval(1,1).diff(Interval(0,2),c1,c2,false);
	CPPUNIT_ASSERT(c1.is_empty() && c2.is_empty());
}
