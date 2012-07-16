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

void TestInterval::check_add(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x+z;
	check(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z+x;
	check(y_actual, y_expected);

	// test the +=operator
	y_actual = x;
	y_actual += z;
	check(y_actual, y_expected);

	// test the +=operator in the other direction
	y_actual = z;
	y_actual += x;
	check(y_actual, y_expected);

	// test subtraction
	y_actual=(-x)-z;
	check(y_actual, -y_expected);

	// test the symmetrical case
	y_actual = (-z)-x;
	check(y_actual, -y_expected);

	// test the -=operator
	y_actual = -x;
	y_actual -= z;
	check(y_actual, -y_expected);

	// test the -=operator in the other direction
	y_actual = -z;
	y_actual -= x;
	check(y_actual, -y_expected);

}

void TestInterval::check_add_scal(const Interval& x, double z, const Interval& y_expected) {
	check(x+z, y_expected);
	check(z+x, y_expected);
	check(Interval(x)+=z, y_expected); // test the +=operator
	check((-x)-z, -y_expected);
	check((-z)-x, -y_expected);
	check(Interval(-x)-=z, -y_expected); // test the -=operator
}


void TestInterval::check_mul(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x*z;
	//cout << "check:    " << x << " * " << z << ", " << y_expected << endl;
	check(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z*x;
	check(y_actual, y_expected);

	// test the *=operator
	y_actual = x;
	y_actual *= z;
	check(y_actual, y_expected);
}

void TestInterval::check_mul_scal(const Interval& x, double z, const Interval& y_expected) {
	check(x*z, y_expected);
	check(z*x, y_expected);
	check(Interval(x)*=z, y_expected); // test the +=operator
}

void TestInterval::check_div(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x/z;
	//cout << "check:    " << x << " / " << z << ", " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	check(y_actual, y_expected);

	// test the /=operator
	y_actual = x;
	y_actual /= z;
	check(y_actual, y_expected);
}

void TestInterval::check_div_scal(const Interval& x, double z, const Interval& y_expected) {
	Interval y_actual=x/z;
	check(y_actual, y_expected);

	// test the /=operator
	y_actual = x;
	y_actual /= z;
	check(y_actual, y_expected);
}

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

void TestInterval::minus01() { check(-Interval(0,1), Interval(-1,0)); }
void TestInterval::minus02() { check(-Interval::ALL_REALS, Interval::ALL_REALS); }
void TestInterval::minus03() { check(-Interval::NEG_REALS, Interval::POS_REALS); }
void TestInterval::minus04() { check(-Interval(NEG_INFINITY,1), Interval(-1,POS_INFINITY)); }

void TestInterval::add01() { check_add(Interval::EMPTY_SET,      Interval(0,1),      Interval::EMPTY_SET); }
void TestInterval::add02() { check_add(Interval(0,1),            Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestInterval::add03() { check_add(Interval(NEG_INFINITY,1), Interval(0,1),      Interval(NEG_INFINITY, 2)); }
void TestInterval::add04() { check_add(Interval(1,POS_INFINITY), Interval(0,1),      Interval(1,POS_INFINITY)); }
void TestInterval::add05() { check_add(Interval::ALL_REALS,      Interval(0,1),      Interval::ALL_REALS); }

void TestInterval::add06() { check_add_scal(Interval::EMPTY_SET,      POS_INFINITY,       Interval::EMPTY_SET); }
void TestInterval::add07() { check_add_scal(Interval::EMPTY_SET,      0,                  Interval::EMPTY_SET); }
void TestInterval::add08() { check_add_scal(Interval(0,1),            1,                  Interval(1,2)); }
void TestInterval::add09() { check_add_scal(Interval(0,1),            NEG_INFINITY,       Interval::EMPTY_SET); }
void TestInterval::add10() { check_add_scal(Interval(0,1),            POS_INFINITY,       Interval::EMPTY_SET); }
void TestInterval::add11() { check_add_scal(Interval(NEG_INFINITY,1), 1,                  Interval(NEG_INFINITY,2)); }

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

void TestInterval::mul17() { check_mul_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestInterval::mul18() { check_mul_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestInterval::mul19() { check_mul_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

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
void TestInterval::div15() { check_div_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestInterval::div16() { check_div_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestInterval::div17() { check_div_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

void TestInterval::sqrt01() { check(sqrt(Interval::ALL_REALS), Interval::POS_REALS); }
void TestInterval::sqrt02() { check(sqrt(Interval::NEG_REALS), Interval::ZERO); }
void TestInterval::sqrt03() { check(sqrt(Interval(-9,4)),      Interval(0,2)); }
void TestInterval::sqrt04() { check(sqrt(Interval(4,9)),       Interval(2,3)); }
void TestInterval::sqrt05() { check(sqrt(Interval(-9,-4)),     Interval::EMPTY_SET); }

#define piL Interval::PI.lb()
#define piU Interval::PI.ub()

void TestInterval::check_trigo(const Interval& x, const Interval& sin_x_expected) {
	check(sin(x), sin_x_expected);
	check(sin(Interval::PI-x), sin_x_expected);
	check(sin(x+Interval::TWO_PI), sin_x_expected);
	check(sin(-x), -sin_x_expected);
	check(cos(x-Interval::HALF_PI), sin_x_expected);
	check(cos(Interval::HALF_PI-x), sin_x_expected);
	check(cos(x+Interval::HALF_PI), -sin_x_expected);
	check(cos(x+Interval::TWO_PI-Interval::HALF_PI), sin_x_expected);
}

void TestInterval::sin01() { check_trigo(Interval::ALL_REALS, Interval(-1,1)); }
void TestInterval::sin02() { check_trigo(Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestInterval::sin03() { check_trigo(Interval(0,piU/2.0), Interval(0,1)); }
void TestInterval::sin04() { check_trigo(Interval(0,piU), Interval(0,1)); }
void TestInterval::sin05() { check_trigo(Interval(0,3*piU/2.0), Interval(-1,1)); }
void TestInterval::sin06() { check_trigo(Interval(piL,3*piU/2.0), Interval(-1,0)); }
void TestInterval::sin07() { check_trigo(Interval(0.5,1.5), Interval(sin(0.5),sin(1.5))); }
void TestInterval::sin08() { check_trigo(Interval(1.5,3), Interval(sin(3.0),1)); }
void TestInterval::sin09() { check_trigo(Interval(3,4), Interval(sin(4.0),sin(3.0))); }
void TestInterval::sin10() { check_trigo(Interval(3,5), Interval(-1,sin(3.0))); }
void TestInterval::sin11() { check_trigo(Interval(3,2*piU+1.5), Interval(-1,sin(1.5))); }
void TestInterval::sin12() { check_trigo(Interval(5,2*piU+1.5), Interval(sin(5),sin(1.5))); }
void TestInterval::sin13() { check_trigo(Interval(5,2*piU+3), Interval(sin(5),1)); }


#define checkproj(func,y,xbefore,xafter) { Interval x=xbefore; NAME2(proj_,func)(y,x); check(x,xafter); }
#define NAME2(a,b)         NAME2_HIDDEN(a,b)
#define NAME2_HIDDEN(a,b)  a ## b

void TestInterval::checkproj_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter) {
    checkproj(sin,  y, xbefore, xafter);
	checkproj(sin, -y,-xbefore,-xafter);
	checkproj(cos,  y, xbefore-Interval::HALF_PI, xafter-Interval::HALF_PI);
	checkproj(cos, -y, xbefore+Interval::HALF_PI, xafter+Interval::HALF_PI);
}

#define checkprojpow(p,y,xbefore,xafter) {

void TestInterval::checkproj_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p) {
	Interval x;
	x=xbefore; proj_pow(y,p,x); check(x,xafter);
	x=-xbefore; proj_pow(p%2==0? y : -y,p,x); check(x,-xafter);
}

bool TestInterval::checkproj_mul(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
								const Interval& x1_aft, const Interval& x2_aft) {
	bool res=true;
	Interval x1;
	Interval x2;

	x1=x1_bef;
	x2=x2_bef;
	proj_mul(y,x1,x2);
	//cout << "x1=" << x1 << " x2=" << x2 << endl;
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=x1_bef;
	x2=x2_bef;
	proj_mul(y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=-x1_bef;
	x2=x2_bef;
	proj_mul(-y,x2,x1);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=x1_bef;
	x2=-x2_bef;
	proj_mul(-y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	return res;
}

void TestInterval::proj_mul01() {
	TEST_ASSERT(checkproj_mul(Interval(1,2),Interval(0.1,2.0),Interval(-10,2),Interval(0.5,2.0),Interval(0.5,2)));
}

void TestInterval::proj_mul02() {
	TEST_ASSERT(checkproj_mul(Interval::EMPTY_SET,Interval(0.1,2.0),Interval(-10,2),Interval::EMPTY_SET,Interval::EMPTY_SET));
}

void TestInterval::proj_mul03() {
	TEST_ASSERT(checkproj_mul(Interval(0,0),Interval(0.1,2.0),Interval(-10,2),Interval(0.1,2.0),Interval(0,0)));
}

void TestInterval::proj_mul04() {
	TEST_ASSERT(checkproj_mul(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1)));
}

void TestInterval::proj_mul05() {
	TEST_ASSERT(checkproj_mul(Interval(1,1),Interval(0,10),Interval(0,10),Interval(0.1,10.0),Interval(0.1,10.0)));
}

void TestInterval::sqrProj01() { checkproj(sqr, Interval(1,9),            Interval(0,4),       Interval(1,3)); }
void TestInterval::sqrProj02() { checkproj(sqr, Interval(1,9),            Interval(0,2),       Interval(1,2)); }
void TestInterval::sqrProj03() { checkproj(sqr, Interval(1,9),            Interval(-4,2),      Interval(-3,2)); }
void TestInterval::sqrProj04() { checkproj(sqr, Interval(1,9),            Interval(-4,-3),     Interval(-3,-3)); }
void TestInterval::sqrProj05() { checkproj(sqr, Interval(NEG_INFINITY,9), Interval(-4,1),      Interval(-3,1)); }
void TestInterval::sqrProj06() { checkproj(sqr, Interval(4,9),            Interval(-1,5),      Interval(2,3)); }
void TestInterval::sqrProj07() { checkproj(sqr, Interval(-4,-2),          Interval::ALL_REALS, Interval::EMPTY_SET); }

void TestInterval::proj_sin01() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0,piU/2.0),        Interval(0.5,1.5)); }
void TestInterval::proj_sin02() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0,5*piU/2.0),      Interval(0.5,2*piU+1.5)); }
void TestInterval::proj_sin03() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,1.5)); }
void TestInterval::proj_sin04() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,2*piU+1.5)); }
void TestInterval::proj_sin05() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piL-0.4,piU/2.0), Interval(0.5,1.5)); }
void TestInterval::proj_sin06() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,1.5)); }
void TestInterval::proj_sin07() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU/2.0), Interval(-piU-1.5,1.5)); }
void TestInterval::proj_sin08() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piL-1.6),      Interval(0.5,1.5)); }
void TestInterval::proj_sin09() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU-1.5),      Interval(0.5,piU-1.5)); }
void TestInterval::proj_sin10() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestInterval::proj_sin11() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU-1.5), Interval(-piU-0.5,piU-1.5)); }
void TestInterval::proj_sin12() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU),     Interval(-piU-1.5,piU-0.5)); }
void TestInterval::proj_sin13() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU/2,piU/2.0),   Interval(0.5,1.5)); }
void TestInterval::proj_sin14() { checkproj_trigo(Interval(2,3),          Interval::ALL_REALS,        Interval::EMPTY_SET); }

void TestInterval::proj_sin15() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0,piU/2.0),        Interval(0.5,piU/2.0)); }
void TestInterval::proj_sin16() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0,5*piU/2.0),      Interval(0.5,5*piU/2.0)); }
void TestInterval::proj_sin17() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,piU/2.0)); }
void TestInterval::proj_sin18() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,5*piU/2.0)); }
void TestInterval::proj_sin19() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-piL-0.4,piU/2.0), Interval(0.5,piU/2.0)); }
void TestInterval::proj_sin20() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,piU/2.0)); }
void TestInterval::proj_sin21() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU/2.0), Interval(-3*piU/2,piU/2.0)); }
void TestInterval::proj_sin22() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestInterval::proj_sin23() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU),     Interval(-3*piU/2,piU-0.5)); }

// not the same case as before because
// when the image is > 1 or < -1 the box x is
// emptied by the call to asin (not by the
//  intersection scheme)
void TestInterval::proj_sin24() { checkproj_trigo(Interval(1.0),          Interval(4.0,6.0),          Interval::EMPTY_SET); }

void TestInterval::proj_pow01() { checkproj_pow(Interval(4,9), Interval(1,4), Interval(2,3), 2); }
void TestInterval::proj_pow02() { checkproj_pow(Interval(4,9), Interval(3,4), Interval(3,3), 2); }
void TestInterval::proj_pow03() { checkproj_pow(Interval(4,9), Interval(-1,4), Interval(2,3), 2); }
void TestInterval::proj_pow04() { checkproj_pow(Interval(4,9), Interval(-2,4), Interval(-2,3), 2); }
void TestInterval::proj_pow05() { checkproj_pow(Interval(4,9), Interval(-5,4), Interval(-3,3), 2); }
void TestInterval::proj_pow06() { checkproj_pow(Interval(4,9), Interval(1,1), Interval::EMPTY_SET, 2); }
void TestInterval::proj_pow07() { checkproj_pow(Interval(4,9), Interval(4,4), Interval::EMPTY_SET, 2); }
void TestInterval::proj_pow08() { checkproj_pow(Interval(8,27), Interval(1,4), Interval(2,3), 3); }
void TestInterval::proj_pow09() { checkproj_pow(Interval(8,27), Interval(3,4), Interval(3,3), 3); }
void TestInterval::proj_pow10() { checkproj_pow(Interval(8,27), Interval(-5,4), Interval(2,3), 3); }
void TestInterval::proj_pow11() { checkproj_pow(Interval(-8,27), Interval(-5,4), Interval(-2,3), 3); }
void TestInterval::proj_pow12() { checkproj_pow(Interval(-27,27), Interval(-5,4), Interval(-3,3), 3); }
void TestInterval::proj_pow13() { checkproj_pow(Interval(8,27), Interval(1,1), Interval::EMPTY_SET, 3); }
void TestInterval::proj_pow14() { checkproj_pow(Interval(8,27), Interval(4,4), Interval::EMPTY_SET, 3); }

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

void TestInterval::check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2) {
	Interval _out1,_out2;
	div2(x,y,_out1,_out2);
	check(_out1,out1);
	check(_out2,out2);

	Interval _inout=out1;
	_inout.div2_inter(x,y);
	check(_inout,out1);

	_inout=out1;
	_inout.div2_inter(x,y,_out2);
	check(_inout,out1);
	TEST_ASSERT(_out2.is_empty());

	_inout=out2;
	_inout.div2_inter(x,y);
	check(_inout,out2);

	_inout=out2;
	_inout.div2_inter(x,y,_out2);
	check(_inout,out2);
	TEST_ASSERT(_out2.is_empty());

	_inout=Interval::ALL_REALS;
	_inout.div2_inter(x,y);
	check(_inout,out1 | out2);

	_inout=Interval::ALL_REALS;
	_inout.div2_inter(x,y,_out2);
	check(_inout,out1);
	check(_out2,out2);

	if (!out1.is_empty() && out1.rad()>0) {
		Interval out1bis(out1.mid(),out1.ub());

		if (!out2.is_empty() && out2.lb()>out1.ub())
			_inout = Interval(out1.mid(),0.5*(out1.ub()+out2.lb()));
		else
			_inout = out1bis;

		_inout.div2_inter(x,y);
		check(_inout,out1bis);

		_inout = out1bis;
		_inout.div2_inter(x,y,_out2);
		check(_inout,out1bis);
		TEST_ASSERT(_out2.is_empty());

		if (!out2.is_empty() && out2.rad()>0) {
			Interval out2bis(out2.lb(),out2.mid());

			_inout = out1bis | out2bis;
			_inout.div2_inter(x,y);
			check(_inout,out1bis | out2bis);

			_inout = out1bis | out2bis;
			_inout.div2_inter(x,y,_out2);
			check(_inout,out1bis);
			check(_out2,out2bis);
		}
	}

}

void TestInterval::div2_01() { check_div2(Interval(6,12),  Interval(0,0),             Interval::EMPTY_SET,       Interval::EMPTY_SET); }
void TestInterval::div2_02() { check_div2(Interval(6,12),  Interval(2,3),             Interval(2,6),             Interval::EMPTY_SET); }
void TestInterval::div2_03() { check_div2(Interval(-2,3),  Interval(-1,1),            Interval::ALL_REALS,       Interval::EMPTY_SET); }
void TestInterval::div2_04() { check_div2(Interval(2,3),   Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestInterval::div2_05() { check_div2(Interval(-3,-2), Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestInterval::div2_06() { check_div2(Interval(2,3),   Interval(NEG_INFINITY,1),  Interval::NEG_REALS,       Interval(2,POS_INFINITY)); }
void TestInterval::div2_07() { check_div2(Interval(2,3),   Interval(-1,POS_INFINITY), Interval(NEG_INFINITY,-2), Interval::POS_REALS); }
void TestInterval::div2_08() { check_div2(Interval(-3,-2), Interval(NEG_INFINITY,1),  Interval(NEG_INFINITY,-2), Interval::POS_REALS); }
void TestInterval::div2_09() { check_div2(Interval(-3,-2), Interval(-1,POS_INFINITY), Interval::NEG_REALS,       Interval(2,POS_INFINITY)); }
void TestInterval::div2_10() { check_div2(Interval(2,3),   Interval::NEG_REALS,       Interval::NEG_REALS,       Interval::EMPTY_SET); }
void TestInterval::div2_11() { check_div2(Interval(2,3),   Interval::POS_REALS,       Interval::POS_REALS,       Interval::EMPTY_SET); }
void TestInterval::div2_12() { check_div2(Interval(-3,-2), Interval::NEG_REALS,       Interval::POS_REALS,       Interval::EMPTY_SET); }
void TestInterval::div2_13() { check_div2(Interval(-3,-2), Interval::POS_REALS,       Interval::NEG_REALS,       Interval::EMPTY_SET); }

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

