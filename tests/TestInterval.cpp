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

using namespace std;

void TestInterval::check(const Interval& y_actual, const Interval& y_expected) {
	//cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	TEST_ASSERT_DELTA("lb", y_actual.lb(),y_expected.lb(),1e-10);
	TEST_ASSERT_DELTA("ub", y_actual.ub(),y_expected.ub(),1e-10);
}

void TestInterval::check_mul(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x*z;
	//cout << "check:    " << x << " * " << z << " == " << y_expected << endl;
	check(y_actual, y_expected);

	y_actual = z*x; // test the symmetrical case
	check(y_actual, y_expected);
}

void TestInterval::check_div(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x/z;
	//cout << "check:    " << x << " / " << z << " == " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	check(y_actual, y_expected);
}

void TestInterval::add01() { check(Interval::EMPTY_SET       + Interval(0,1),      Interval::EMPTY_SET); }
void TestInterval::add02() { check(Interval(0,1)            + Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestInterval::add03() { check(Interval(NEG_INFINITY,1) + Interval(0,1),      Interval(NEG_INFINITY, 2)); }
void TestInterval::add04() { check(Interval(1,POS_INFINITY) + Interval(0,1),      Interval(1,POS_INFINITY)); }
void TestInterval::add05() { check(Interval::ALL_REALS       + Interval(0,1),      Interval::ALL_REALS); }

void TestInterval::mul01() { check_mul(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
void TestInterval::mul02() { check_mul(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
void TestInterval::mul03() { check_mul(Interval(-1,1),              Interval::NEG_REALS, 	     Interval::ALL_REALS); }

void TestInterval::mul04() { check_mul(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval::POS_REALS); }
void TestInterval::mul05() { check_mul(Interval(NEG_INFINITY, 1),   Interval(-1,0),              Interval(-1,POS_INFINITY)); }
void TestInterval::mul06() { check_mul(Interval(0, 1),              Interval(1,POS_INFINITY), 	 Interval::POS_REALS); }
void TestInterval::mul07() { check_mul(Interval(0, 1),              Interval(-1,POS_INFINITY),   Interval(-1,POS_INFINITY)); }

void TestInterval::mul08() { check_mul(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval::NEG_REALS); }
void TestInterval::mul09() { check_mul(Interval(NEG_INFINITY, 1),   Interval(0,1),               Interval(NEG_INFINITY,1)); }
void TestInterval::mul10() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,-1),   Interval::NEG_REALS); }
void TestInterval::mul11() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,1),    Interval(NEG_INFINITY,1)); }

void TestInterval::mul12() { check_mul(Interval(1,POS_INFINITY),    Interval(0,1),               Interval::POS_REALS); }
void TestInterval::mul13() { check_mul(Interval(-1,POS_INFINITY),   Interval(0,1),               Interval(-1,POS_INFINITY)); }

void TestInterval::mul14() { check_mul(Interval(1,2),               Interval(1,2), 	        	 Interval(1,4)); }
void TestInterval::mul15() { check_mul(Interval(1,2),               Interval(-2,3), 	         Interval(-4,6)); }
void TestInterval::mul16() { check_mul(Interval(-1,1),              Interval(-2,3), 	         Interval(-3,3)); }

void TestInterval::div01() { check_div(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
void TestInterval::div02() { check_div(Interval::ZERO,              Interval::ZERO,              Interval::EMPTY_SET); }
void TestInterval::div03() { check_div(Interval(1,2),               Interval::ZERO,              Interval::EMPTY_SET); }
void TestInterval::div04() { check_div(Interval::ALL_REALS,         Interval::ZERO,              Interval::EMPTY_SET); }
void TestInterval::div05() { check_div(Interval::ZERO,              Interval(0,1),               Interval::ZERO); }
void TestInterval::div06() { check_div(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
void TestInterval::div07() { check_div(Interval(6,12),              Interval(2,3),               Interval(2,6)); }
void TestInterval::div08() { check_div(Interval(6,12),              Interval(-3,-2),             Interval(-6,-2)); }
void TestInterval::div09() { check_div(Interval(6,12),              Interval(-2,3),              Interval::ALL_REALS); }
void TestInterval::div10() { check_div(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval(1,POS_INFINITY)); }
void TestInterval::div11() { check_div(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval(NEG_INFINITY,-1)); }
void TestInterval::div12() { check_div(Interval(1,POS_INFINITY),    Interval(-1,0),              Interval(NEG_INFINITY,-1)); }
void TestInterval::div13() { check_div(Interval(1,POS_INFINITY),    Interval(0,1),               Interval(1,POS_INFINITY)); }
void TestInterval::div14() { check_div(Interval(-1,1),              Interval(-1,1), 	         Interval::ALL_REALS); }

void TestInterval::sqrt01() { check(sqrt(Interval::ALL_REALS), Interval::POS_REALS); }
void TestInterval::sqrt02() { check(sqrt(Interval::NEG_REALS), Interval::ZERO); }
void TestInterval::sqrt03() { check(sqrt(Interval(-9,4)),      Interval(0,2)); }
void TestInterval::sqrt04() { check(sqrt(Interval(4,9)),       Interval(2,3)); }
void TestInterval::sqrt05() { check(sqrt(Interval(-9,-4)),     Interval::EMPTY_SET); }

#define checkproj(func,y,xbefore,xafter) Interval x=xbefore; NAME2(proj_,func)(y,x); check(x,xafter);
#define NAME2(a,b)         NAME2_HIDDEN(a,b)
#define NAME2_HIDDEN(a,b)  a ## b

void TestInterval::sqrProj01() { checkproj(sqr, Interval(1,9),            Interval(0,4),        Interval(1,3)); }
void TestInterval::sqrProj02() { checkproj(sqr, Interval(1,9),            Interval(0,2),       Interval(1,2)); }
void TestInterval::sqrProj03() { checkproj(sqr, Interval(1,9),            Interval(-4,2),      Interval(-3,2)); }
void TestInterval::sqrProj04() { checkproj(sqr, Interval(1,9),            Interval(-4,-3),     Interval(-3,-3)); }
void TestInterval::sqrProj05() { checkproj(sqr, Interval(NEG_INFINITY,9), Interval(-4,1),      Interval(-3,1)); }
void TestInterval::sqrProj06() { checkproj(sqr, Interval(4,9),            Interval(-1,5),      Interval(2,3)); }
void TestInterval::sqrProj07() { checkproj(sqr, Interval(-4,-2),          Interval::ALL_REALS, Interval::EMPTY_SET); }

void TestInterval::sinProj01() { /*check(asin_Proj(Interval(0,1),   Interval(-0.5*pi,3.5*pi)),  Interval(0,3*pi));*/ }
