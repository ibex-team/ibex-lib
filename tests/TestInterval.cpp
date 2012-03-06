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

#define ERROR 1e-10

void TestInterval::check(double y_actual, double y_expected) {
	TEST_ASSERT_DELTA("==",y_actual,y_expected,ERROR);
}

void TestInterval::check(const Interval& y_actual, const Interval& y_expected) {
	//cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	TEST_ASSERT_DELTA("lb", y_actual.lb(),y_expected.lb(),ERROR);
	TEST_ASSERT_DELTA("ub", y_actual.ub(),y_expected.ub(),ERROR);
}

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

#define checkproj(func,y,xbefore,xafter) Interval x=xbefore; NAME2(proj_,func)(y,x); check(x,xafter);
#define NAME2(a,b)         NAME2_HIDDEN(a,b)
#define NAME2_HIDDEN(a,b)  a ## b

void TestInterval::sqrProj01() { checkproj(sqr, Interval(1,9),            Interval(0,4),       Interval(1,3)); }
void TestInterval::sqrProj02() { checkproj(sqr, Interval(1,9),            Interval(0,2),       Interval(1,2)); }
void TestInterval::sqrProj03() { checkproj(sqr, Interval(1,9),            Interval(-4,2),      Interval(-3,2)); }
void TestInterval::sqrProj04() { checkproj(sqr, Interval(1,9),            Interval(-4,-3),     Interval(-3,-3)); }
void TestInterval::sqrProj05() { checkproj(sqr, Interval(NEG_INFINITY,9), Interval(-4,1),      Interval(-3,1)); }
void TestInterval::sqrProj06() { checkproj(sqr, Interval(4,9),            Interval(-1,5),      Interval(2,3)); }
void TestInterval::sqrProj07() { checkproj(sqr, Interval(-4,-2),          Interval::ALL_REALS, Interval::EMPTY_SET); }

void TestInterval::sinProj01() { /*check(asin_Proj(Interval(0,1),   Interval(-0.5*pi,3.5*pi)),  Interval(0,3*pi));*/ }

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
	TEST_ASSERT_DELTA("lb",x.lb(),0,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),2,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),1,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),1,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),2,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),0,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),2,ERROR);
}

void TestInterval::getters02() {
	Interval x(-3,-1);
	TEST_ASSERT_DELTA("lb",x.lb(),-3,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),-1,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),-2,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),1,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),2,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),1,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),3,ERROR);
}

void TestInterval::getters03() {
	Interval x(-3,1);
	TEST_ASSERT_DELTA("lb",x.lb(),-3,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),1,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),-1,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),2,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),4,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),0,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),3,ERROR);
}

void TestInterval::getters04() {
	Interval x(Interval::NEG_REALS);
	TEST_ASSERT_DELTA("lb",x.lb(),NEG_INFINITY,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),0,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),-DBL_MAX,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),0,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),POS_INFINITY,ERROR);
}

void TestInterval::getters05() {
	Interval x(Interval::ALL_REALS);
	TEST_ASSERT_DELTA("lb",x.lb(),NEG_INFINITY,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),0,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),0,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),POS_INFINITY,ERROR);
}

void TestInterval::getters06() {
	Interval x(DBL_MAX,POS_INFINITY);
	TEST_ASSERT_DELTA("lb",x.lb(),DBL_MAX,ERROR);
	TEST_ASSERT_DELTA("ub",x.ub(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("mid",x.mid(),DBL_MAX,ERROR);
	TEST_ASSERT_DELTA("rad",x.rad(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("diam",x.diam(),POS_INFINITY,ERROR);
	TEST_ASSERT_DELTA("mig",x.mig(),DBL_MAX,ERROR);
	TEST_ASSERT_DELTA("mag",x.mag(),POS_INFINITY,ERROR);
}


void TestInterval::is_subset01()          { TEST_ASSERT(Interval(0,1).is_subset(Interval(0,2))); }
void TestInterval::is_subset02()          { TEST_ASSERT(!Interval(0,1).is_subset(Interval(1,2))); }
void TestInterval::is_strict_subset01()   { TEST_ASSERT(!Interval(0,1).is_strict_subset(Interval(0,2))); }
void TestInterval::is_strict_subset02()   { TEST_ASSERT(Interval::NEG_REALS.is_strict_subset(Interval::ALL_REALS)); }
void TestInterval::is_superset01()        { TEST_ASSERT(Interval(0,2).is_superset(Interval(0,1))); }
void TestInterval::is_superset02()        { TEST_ASSERT(!Interval(1,2).is_superset(Interval(0,1))); }
void TestInterval::is_strict_superset01() { TEST_ASSERT(!Interval(0,2).is_strict_superset(Interval(0,1))); }
void TestInterval::is_strict_superset02() { TEST_ASSERT(Interval::ALL_REALS.is_strict_superset(Interval::NEG_REALS)); }
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

