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

#include "TestArith.h"
#include "ibex_Linear.h"
#include "utils.h"
#include <float.h>

using namespace std;

void TestArith::check_add(const Interval& x, const Interval& z, const Interval& y_expected) {
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

void TestArith::check_add_scal(const Interval& x, double z, const Interval& y_expected) {
	check(x+z, y_expected);
	check(z+x, y_expected);
	check(Interval(x)+=z, y_expected); // test the +=operator
	check((-x)-z, -y_expected);
	check((-z)-x, -y_expected);
	check(Interval(-x)-=z, -y_expected); // test the -=operator
}


void TestArith::check_mul(const Interval& x, const Interval& z, const Interval& y_expected) {
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

void TestArith::check_mul_scal(const Interval& x, double z, const Interval& y_expected) {
	check(x*z, y_expected);
	check(z*x, y_expected);
	check(Interval(x)*=z, y_expected); // test the +=operator
}

void TestArith::check_div(const Interval& x, const Interval& z, const Interval& y_expected) {
	Interval y_actual=x/z;
	//cout << "check:    " << x << " / " << z << ", " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	check(y_actual, y_expected);

	// test the /=operator
	y_actual = x;
	y_actual /= z;
	check(y_actual, y_expected);
}

void TestArith::check_div_scal(const Interval& x, double z, const Interval& y_expected) {
	Interval y_actual=x/z;
	check(y_actual, y_expected);

	// test the /=operator
	y_actual = x;
	y_actual /= z;
	check(y_actual, y_expected);
}


void TestArith::minus01() { check(-Interval(0,1), Interval(-1,0)); }
void TestArith::minus02() { check(-Interval::ALL_REALS, Interval::ALL_REALS); }
void TestArith::minus03() { check(-Interval::NEG_REALS, Interval::POS_REALS); }
void TestArith::minus04() { check(-Interval(NEG_INFINITY,1), Interval(-1,POS_INFINITY)); }

void TestArith::add01() { check_add(Interval::EMPTY_SET,      Interval(0,1),      Interval::EMPTY_SET); }
void TestArith::add02() { check_add(Interval(0,1),            Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestArith::add03() { check_add(Interval(NEG_INFINITY,1), Interval(0,1),      Interval(NEG_INFINITY, 2)); }
void TestArith::add04() { check_add(Interval(1,POS_INFINITY), Interval(0,1),      Interval(1,POS_INFINITY)); }
void TestArith::add05() { check_add(Interval::ALL_REALS,      Interval(0,1),      Interval::ALL_REALS); }

void TestArith::add06() { check_add_scal(Interval::EMPTY_SET,      POS_INFINITY,       Interval::EMPTY_SET); }
void TestArith::add07() { check_add_scal(Interval::EMPTY_SET,      0,                  Interval::EMPTY_SET); }
void TestArith::add08() { check_add_scal(Interval(0,1),            1,                  Interval(1,2)); }
void TestArith::add09() { check_add_scal(Interval(0,1),            NEG_INFINITY,       Interval::EMPTY_SET); }
void TestArith::add10() { check_add_scal(Interval(0,1),            POS_INFINITY,       Interval::EMPTY_SET); }
void TestArith::add11() { check_add_scal(Interval(NEG_INFINITY,1), 1,                  Interval(NEG_INFINITY,2)); }

void TestArith::mul01() { check_mul(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
void TestArith::mul02() { check_mul(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
void TestArith::mul03() { check_mul(Interval(-1,1),              Interval::NEG_REALS, 	     Interval::ALL_REALS); }

void TestArith::mul04() { check_mul(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval::POS_REALS); }
void TestArith::mul05() { check_mul(Interval(NEG_INFINITY, 1),   Interval(-1,0),              Interval(-1,POS_INFINITY)); }
void TestArith::mul06() { check_mul(Interval(0, 1),              Interval(1,POS_INFINITY), 	 Interval::POS_REALS); }
void TestArith::mul07() { check_mul(Interval(0, 1),              Interval(-1,POS_INFINITY),   Interval(-1,POS_INFINITY)); }

void TestArith::mul08() { check_mul(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval::NEG_REALS); }
void TestArith::mul09() { check_mul(Interval(NEG_INFINITY, 1),   Interval(0,1),               Interval(NEG_INFINITY,1)); }
void TestArith::mul10() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,-1),   Interval::NEG_REALS); }
void TestArith::mul11() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,1),    Interval(NEG_INFINITY,1)); }

void TestArith::mul12() { check_mul(Interval(1,POS_INFINITY),    Interval(0,1),               Interval::POS_REALS); }
void TestArith::mul13() { check_mul(Interval(-1,POS_INFINITY),   Interval(0,1),               Interval(-1,POS_INFINITY)); }

void TestArith::mul14() { check_mul(Interval(1,2),               Interval(1,2), 	        	 Interval(1,4)); }
void TestArith::mul15() { check_mul(Interval(1,2),               Interval(-2,3), 	         Interval(-4,6)); }
void TestArith::mul16() { check_mul(Interval(-1,1),              Interval(-2,3), 	         Interval(-3,3)); }

void TestArith::mul17() { check_mul_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestArith::mul18() { check_mul_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestArith::mul19() { check_mul_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

void TestArith::div01() { check_div(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
void TestArith::div02() { check_div(Interval::ZERO,              Interval::ZERO,              Interval::EMPTY_SET); }
void TestArith::div03() { check_div(Interval(1,2),               Interval::ZERO,              Interval::EMPTY_SET); }
void TestArith::div04() { check_div(Interval::ALL_REALS,         Interval::ZERO,              Interval::EMPTY_SET); }
void TestArith::div05() { check_div(Interval::ZERO,              Interval(0,1),               Interval::ZERO); }
void TestArith::div06() { check_div(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
void TestArith::div07() { check_div(Interval(6,12),              Interval(2,3),               Interval(2,6)); }
void TestArith::div08() { check_div(Interval(6,12),              Interval(-3,-2),             Interval(-6,-2)); }
void TestArith::div09() { check_div(Interval(6,12),              Interval(-2,3),              Interval::ALL_REALS); }
void TestArith::div10() { check_div(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval(1,POS_INFINITY)); }
void TestArith::div11() { check_div(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval(NEG_INFINITY,-1)); }
void TestArith::div12() { check_div(Interval(1,POS_INFINITY),    Interval(-1,0),              Interval(NEG_INFINITY,-1)); }
void TestArith::div13() { check_div(Interval(1,POS_INFINITY),    Interval(0,1),               Interval(1,POS_INFINITY)); }
void TestArith::div14() { check_div(Interval(-1,1),              Interval(-1,1), 	         Interval::ALL_REALS); }
void TestArith::div15() { check_div_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestArith::div16() { check_div_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
void TestArith::div17() { check_div_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

void TestArith::sqrt01() { check(sqrt(Interval::ALL_REALS), Interval::POS_REALS); }
void TestArith::sqrt02() { check(sqrt(Interval::NEG_REALS), Interval::ZERO); }
void TestArith::sqrt03() { check(sqrt(Interval(-9,4)),      Interval(0,2)); }
void TestArith::sqrt04() { check(sqrt(Interval(4,9)),       Interval(2,3)); }
void TestArith::sqrt05() { check(sqrt(Interval(-9,-4)),     Interval::EMPTY_SET); }

#define piL Interval::PI.lb()
#define piU Interval::PI.ub()

void TestArith::check_sinh(const Interval& x) {
	double xl=x.lb();
	double xu=x.ub();
	double yl=xl==NEG_INFINITY? NEG_INFINITY : 0.5*(exp(xl)-exp(-xl));
	double yu=xu==POS_INFINITY? POS_INFINITY : 0.5*(exp(xu)-exp(-xu));

	check(sinh(x), Interval(yl,yu));
	check(sinh(-x), Interval(-yu,-yl));
	check(x,asinh(sinh(x)));
	check(-x,asinh(sinh(-x)));
}

void TestArith::sinh01() { check_sinh(Interval::ALL_REALS); }
void TestArith::sinh02() { check_sinh(Interval::POS_REALS); }
void TestArith::sinh03() { check_sinh(Interval(0,1)); }
void TestArith::sinh04() { check_sinh(Interval(1,POS_INFINITY)); }
void TestArith::sinh05() { check_sinh(Interval(1,1)); }
void TestArith::sinh06() { check_sinh(Interval(2,3)); }
void TestArith::sinh07() { check_sinh(Interval(4,5)); }

void TestArith::check_trigo(const Interval& x, const Interval& sin_x_expected) {
	check(sin(x), sin_x_expected);
	check(sin(Interval::PI-x), sin_x_expected);
	check(sin(x+Interval::TWO_PI), sin_x_expected);
	check(sin(-x), -sin_x_expected);
	check(cos(x-Interval::HALF_PI), sin_x_expected);
	check(cos(Interval::HALF_PI-x), sin_x_expected);
	check(cos(x+Interval::HALF_PI), -sin_x_expected);
	check(cos(x+Interval::TWO_PI-Interval::HALF_PI), sin_x_expected);
}

void TestArith::log01() { check(log(Interval::EMPTY_SET), Interval::EMPTY_SET); }
void TestArith::log02() { check(log(Interval::ALL_REALS), Interval::ALL_REALS); }
void TestArith::log03() { check(log(Interval::POS_REALS), Interval::ALL_REALS); }
void TestArith::log04() { check(log(Interval::NEG_REALS), Interval::EMPTY_SET); }
void TestArith::log05() { check(log(Interval(1,2)),       Interval(0,::log(2))); }
void TestArith::log06() { check(log(Interval(-1,1)),      Interval(NEG_INFINITY,0)); }

void TestArith::sin01() { check_trigo(Interval::ALL_REALS, Interval(-1,1)); }
void TestArith::sin02() { check_trigo(Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestArith::sin03() { check_trigo(Interval(0,piU/2.0), Interval(0,1)); }
void TestArith::sin04() { check_trigo(Interval(0,piU), Interval(0,1)); }
void TestArith::sin05() { check_trigo(Interval(0,3*piU/2.0), Interval(-1,1)); }
void TestArith::sin06() { check_trigo(Interval(piL,3*piU/2.0), Interval(-1,0)); }
void TestArith::sin07() { check_trigo(Interval(0.5,1.5), Interval(sin(0.5),sin(1.5))); }
void TestArith::sin08() { check_trigo(Interval(1.5,3), Interval(sin(3.0),1)); }
void TestArith::sin09() { check_trigo(Interval(3,4), Interval(sin(4.0),sin(3.0))); }
void TestArith::sin10() { check_trigo(Interval(3,5), Interval(-1,sin(3.0))); }
void TestArith::sin11() { check_trigo(Interval(3,2*piU+1.5), Interval(-1,sin(1.5))); }
void TestArith::sin12() { check_trigo(Interval(5,2*piU+1.5), Interval(sin(5),sin(1.5))); }
void TestArith::sin13() { check_trigo(Interval(5,2*piU+3), Interval(sin(5),1)); }

void TestArith::check_pow(const Interval& x, int p, const Interval& y_expected) {
	check(pow(x,p),y_expected);
	check(pow(-x,p),(p%2==0)? y_expected : -y_expected);
}

void TestArith::pow01() { check_pow(Interval::ALL_REALS, 4, Interval::POS_REALS); }
void TestArith::pow02() { check_pow(Interval::EMPTY_SET, 4, Interval::EMPTY_SET); }
void TestArith::pow03() { check_pow(Interval(2,3), 4, Interval(16,81)); }
void TestArith::pow04() { check_pow(Interval(-2,3), 4, Interval(0,81)); }
void TestArith::pow05() { check_pow(Interval(-3,2), 4, Interval(0,81)); }
void TestArith::pow06() { check_pow(Interval(2,POS_INFINITY), 4, Interval(16,POS_INFINITY)); }
void TestArith::pow07() { check_pow(Interval::ALL_REALS, 3, Interval::ALL_REALS); }
void TestArith::pow08() { check_pow(Interval::EMPTY_SET, 3, Interval::EMPTY_SET); }
void TestArith::pow09() { check_pow(Interval(2,3), 3, Interval(8,27)); }
void TestArith::pow10() { check_pow(Interval(-2,3), 3, Interval(-8,27)); }
void TestArith::pow11() { check_pow(Interval(-3,2), 3, Interval(-27,8)); }
void TestArith::pow12() { check_pow(Interval(2,POS_INFINITY), 3, Interval(8,POS_INFINITY)); }
void TestArith::pow13() { check_pow(Interval(-10,10), -2, Interval(1.0/100,POS_INFINITY)); }

void TestArith::root01() { check(root(Interval(0,1),-1), Interval(1.0,POS_INFINITY)); }
void TestArith::root02() { TEST_ASSERT(almost_eq(root(Interval(-27,-8), 3),Interval(-3,-2),1e-10)); }

#define checkproj(func,y,xbefore,xafter) { Interval x=xbefore; NAME2(proj_,func)(y,x); check(x,xafter); }
#define NAME2(a,b)         NAME2_HIDDEN(a,b)
#define NAME2_HIDDEN(a,b)  a ## b

void TestArith::checkproj_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter) {
    checkproj(sin,  y, xbefore, xafter);
	checkproj(sin, -y,-xbefore,-xafter);
	checkproj(cos,  y, xbefore-Interval::HALF_PI, xafter-Interval::HALF_PI);
	checkproj(cos, -y, xbefore+Interval::HALF_PI, xafter+Interval::HALF_PI);
}

#define checkprojpow(p,y,xbefore,xafter) {

void TestArith::checkproj_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p) {
	Interval x;
	x=xbefore; proj_pow(y,p,x); check(x,xafter);
	x=-xbefore; proj_pow(p%2==0? y : -y,p,x); check(x,-xafter);
}

bool TestArith::checkproj_mul(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
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

void TestArith::proj_mul01() {
	TEST_ASSERT(checkproj_mul(Interval(1,2),Interval(0.1,2.0),Interval(-10,2),Interval(0.5,2.0),Interval(0.5,2)));
}

void TestArith::proj_mul02() {
	TEST_ASSERT(checkproj_mul(Interval::EMPTY_SET,Interval(0.1,2.0),Interval(-10,2),Interval::EMPTY_SET,Interval::EMPTY_SET));
}

void TestArith::proj_mul03() {
	TEST_ASSERT(checkproj_mul(Interval(0,0),Interval(0.1,2.0),Interval(-10,2),Interval(0.1,2.0),Interval(0,0)));
}

void TestArith::proj_mul04() {
	TEST_ASSERT(checkproj_mul(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1)));
}

void TestArith::proj_mul05() {
	TEST_ASSERT(checkproj_mul(Interval(1,1),Interval(0,10),Interval(0,10),Interval(0.1,10.0),Interval(0.1,10.0)));
}

void TestArith::proj_mulVV01() {
	IntervalVector a(2);
	a[0]=Interval(2,4);
	a[1]=Interval(-0.1,0.1);

	Interval b(2,3);

	IntervalVector x(2,Interval(-10,10));

	proj_mul(b,a,x);
	check(x[0],Interval(1.0/4,2));
	check(x[1],Interval(-10,10));
}

void TestArith::proj_mulMV01() {
	double delta=0.1;
	IntervalMatrix A(2,2);
	Interval deltaM(-delta,delta);
	A[0][0]=1+deltaM;
	A[0][1]=deltaM;
	A[1][0]=deltaM;
	A[1][1]=1+deltaM;

	Vector b(2,1.0);

	IntervalVector x(2,Interval(-10,10));

	double _M[16]={1+delta,   0,       0,     delta,
				     0,    1-delta,    0,    -delta,
				     0,     -delta,    0,   1-delta,
				     0,      delta, 1+delta,    0};
	Matrix M(4,4,_M);
	Matrix invM(4,4);
	real_inverse(M,invM);

	Vector b2(4,1.0);
	Vector bounds=invM*b2;
	double _x2[2][2]={ {bounds[0],bounds[1]}, {bounds[2],bounds[3]} };
	IntervalVector x2(2,_x2);

	double epsilon=1e-07;
	proj_mul(b,A,x,epsilon);

	TEST_ASSERT(almost_eq(x,x2,epsilon));
}

void TestArith::checkproj_div(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
		const Interval& x1_aft, const Interval& x2_aft) {
	Interval x1;
	Interval x2;

	x1=x1_bef;
	x2=x2_bef;
	proj_div(y,x1,x2);
	check(x1,x1_aft);
	check(x2,x2_aft);

	x1=-x1_bef;
	x2=-x2_bef;
	proj_div(y,x1,x2);
	check(x1,-x1_aft);
	check(x2,-x2_aft);

	x1=-x1_bef;
	x2=x2_bef;
	proj_div(-y,x1,x2);
	check(x1,-x1_aft);
	check(x2,x2_aft);

	x1=x1_bef;
	x2=-x2_bef;
	proj_div(-y,x1,x2);
	check(x1,x1_aft);
	check(x2,-x2_aft);
}

void TestArith::proj_div01() { checkproj_div(Interval(1,2),  Interval(0,1),   Interval(2,3),  Interval::EMPTY_SET, Interval::EMPTY_SET); }
void TestArith::proj_div02() { checkproj_div(Interval(1,2),  Interval(0,1),   Interval(1,3),  1,                   1); }
void TestArith::proj_div03() { checkproj_div(Interval(1,2),  Interval(1,3),   Interval(0,1),  Interval(1,2),       Interval(0.5,1)); }
void TestArith::proj_div04() { checkproj_div(Interval(-1,1), Interval(-2,2),  Interval(0,1),  Interval(-1,1),      Interval(0,1)); }
void TestArith::proj_div05() { checkproj_div(Interval(-1,1), Interval(-2,2),  Interval::ZERO, Interval::ZERO,      Interval::ZERO); }
void TestArith::proj_div06() { checkproj_div(Interval::ZERO, Interval(-2,2),  Interval(-2,2), Interval::ZERO,      Interval(-2,2)); }
void TestArith::proj_div07() { checkproj_div(Interval::POS_REALS, Interval(0,1),  Interval(-1,0), Interval::ZERO,   Interval(-1,0)); }
// note: 0/0 can be any number.
void TestArith::proj_div08() { checkproj_div(Interval(ibex::next_float(0),POS_INFINITY), Interval(0,1), Interval(-1,0), Interval::ZERO, Interval::ZERO); }
void TestArith::proj_div09() { checkproj_div(Interval::POS_REALS, Interval(ibex::next_float(0),1),  Interval(-1,0),  Interval::EMPTY_SET, Interval::EMPTY_SET); }

void TestArith::proj_sqr01() { checkproj(sqr, Interval(1,9),            Interval(0,4),       Interval(1,3)); }
void TestArith::proj_sqr02() { checkproj(sqr, Interval(1,9),            Interval(0,2),       Interval(1,2)); }
void TestArith::proj_sqr03() { checkproj(sqr, Interval(1,9),            Interval(-4,2),      Interval(-3,2)); }
void TestArith::proj_sqr04() { checkproj(sqr, Interval(1,9),            Interval(-4,-3),     Interval(-3,-3)); }
void TestArith::proj_sqr05() { checkproj(sqr, Interval(NEG_INFINITY,9), Interval(-4,1),      Interval(-3,1)); }
void TestArith::proj_sqr06() { checkproj(sqr, Interval(4,9),            Interval(-1,5),      Interval(2,3)); }
void TestArith::proj_sqr07() { checkproj(sqr, Interval(-4,-2),          Interval::ALL_REALS, Interval::EMPTY_SET); }

void TestArith::proj_log01() { checkproj(log, Interval::ALL_REALS,      Interval::ALL_REALS, Interval::POS_REALS); }
void TestArith::proj_log02() { checkproj(log, Interval::NEG_REALS,      Interval::ALL_REALS, Interval(0,1)); }
void TestArith::proj_log03() { checkproj(log, Interval::POS_REALS,      Interval(0,1),       1); }
void TestArith::proj_log04() { checkproj(log, Interval(0,1),      		Interval(-1,3),      Interval(1,::exp(1))); }
void TestArith::proj_log05() { checkproj(log, Interval(NEG_INFINITY,1), Interval(-1,3),      Interval(0,::exp(1))); }
void TestArith::proj_log06() { checkproj(log, Interval(NEG_INFINITY,1), Interval(-1,2),      Interval(0,2)); }
void TestArith::proj_log07() { checkproj(log, Interval(NEG_INFINITY,1), Interval(3,4),       Interval::EMPTY_SET); }
void TestArith::proj_log08() { checkproj(log, Interval(-1,1),           Interval(-0.01,0.01),Interval::EMPTY_SET); }


void TestArith::proj_sin01() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0,piU/2.0),        Interval(0.5,1.5)); }
void TestArith::proj_sin02() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0,5*piU/2.0),      Interval(0.5,2*piU+1.5)); }
void TestArith::proj_sin03() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,1.5)); }
void TestArith::proj_sin04() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,2*piU+1.5)); }
void TestArith::proj_sin05() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piL-0.4,piU/2.0), Interval(0.5,1.5)); }
void TestArith::proj_sin06() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,1.5)); }
void TestArith::proj_sin07() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU/2.0), Interval(-piU-1.5,1.5)); }
void TestArith::proj_sin08() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piL-1.6),      Interval(0.5,1.5)); }
void TestArith::proj_sin09() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU-1.5),      Interval(0.5,piU-1.5)); }
void TestArith::proj_sin10() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestArith::proj_sin11() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU-1.5), Interval(-piU-0.5,piU-1.5)); }
void TestArith::proj_sin12() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU),     Interval(-piU-1.5,piU-0.5)); }
void TestArith::proj_sin13() { checkproj_trigo(sin(Interval(0.5,1.5)), Interval(-piU/2,piU/2.0),   Interval(0.5,1.5)); }
void TestArith::proj_sin14() { checkproj_trigo(Interval(2,3),          Interval::ALL_REALS,        Interval::EMPTY_SET); }

void TestArith::proj_sin15() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0,piU/2.0),        Interval(0.5,piU/2.0)); }
void TestArith::proj_sin16() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0,5*piU/2.0),      Interval(0.5,5*piU/2.0)); }
void TestArith::proj_sin17() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,piU/2.0)); }
void TestArith::proj_sin18() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,5*piU/2.0)); }
void TestArith::proj_sin19() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-piL-0.4,piU/2.0), Interval(0.5,piU/2.0)); }
void TestArith::proj_sin20() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,piU/2.0)); }
void TestArith::proj_sin21() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU/2.0), Interval(-3*piU/2,piU/2.0)); }
void TestArith::proj_sin22() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestArith::proj_sin23() { checkproj_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU),     Interval(-3*piU/2,piU-0.5)); }

// not the same case as before because
// when the image is > 1 or < -1 the box x is
// emptied by the call to asin (not by the
//  intersection scheme)
void TestArith::proj_sin24() { checkproj_trigo(Interval(1.0),          Interval(4.0,6.0),          Interval::EMPTY_SET); }

void TestArith::proj_pow01() { checkproj_pow(Interval(16,81), Interval(1,4), Interval(2,3), 4); }
void TestArith::proj_pow02() { checkproj_pow(Interval(16,81), Interval(3,4), Interval(3,3), 4); }
void TestArith::proj_pow03() { checkproj_pow(Interval(16,81), Interval(-1,4), Interval(2,3), 4); }
void TestArith::proj_pow04() { checkproj_pow(Interval(16,81), Interval(-2,4), Interval(-2,3), 4); }
void TestArith::proj_pow05() { checkproj_pow(Interval(16,81), Interval(-5,4), Interval(-3,3), 4); }
void TestArith::proj_pow06() { checkproj_pow(Interval(16,81), Interval(1,1), Interval::EMPTY_SET, 4); }
void TestArith::proj_pow07() { checkproj_pow(Interval(16,81), Interval(4,4), Interval::EMPTY_SET, 4); }
void TestArith::proj_pow08() { checkproj_pow(Interval(8,27), Interval(1,4), Interval(2,3), 3); }
void TestArith::proj_pow09() { checkproj_pow(Interval(8,27), Interval(3,4), Interval(3,3), 3); }
void TestArith::proj_pow10() { checkproj_pow(Interval(8,27), Interval(-5,4), Interval(2,3), 3); }
void TestArith::proj_pow11() { checkproj_pow(Interval(-8,27), Interval(-5,4), Interval(-2,3), 3); }
void TestArith::proj_pow12() { checkproj_pow(Interval(-27,27), Interval(-5,4), Interval(-3,3), 3); }
void TestArith::proj_pow13() { checkproj_pow(Interval(8,27), Interval(1,1), Interval::EMPTY_SET, 3); }
void TestArith::proj_pow14() { checkproj_pow(Interval(8,27), Interval(4,4), Interval::EMPTY_SET, 3); }
void TestArith::proj_pow15() { checkproj_pow(Interval(-27,-8), Interval(-1,-1), Interval::EMPTY_SET, 3); }
void TestArith::proj_pow16() { checkproj_pow(Interval(-27,-8), Interval(-4,-4), Interval::EMPTY_SET, 3); }
void TestArith::proj_pow17() { checkproj_pow(Interval(0,1), Interval(-10,10), Interval(-10,10), -2); }

void TestArith::check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2) {
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

void TestArith::div2_01() { check_div2(Interval(6,12),  Interval(0,0),             Interval::EMPTY_SET,       Interval::EMPTY_SET); }
void TestArith::div2_02() { check_div2(Interval(6,12),  Interval(2,3),             Interval(2,6),             Interval::EMPTY_SET); }
void TestArith::div2_03() { check_div2(Interval(-2,3),  Interval(-1,1),            Interval::ALL_REALS,       Interval::EMPTY_SET); }
void TestArith::div2_04() { check_div2(Interval(2,3),   Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestArith::div2_05() { check_div2(Interval(-3,-2), Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestArith::div2_06() { check_div2(Interval(2,3),   Interval(NEG_INFINITY,1),  Interval::NEG_REALS,       Interval(2,POS_INFINITY)); }
void TestArith::div2_07() { check_div2(Interval(2,3),   Interval(-1,POS_INFINITY), Interval(NEG_INFINITY,-2), Interval::POS_REALS); }
void TestArith::div2_08() { check_div2(Interval(-3,-2), Interval(NEG_INFINITY,1),  Interval(NEG_INFINITY,-2), Interval::POS_REALS); }
void TestArith::div2_09() { check_div2(Interval(-3,-2), Interval(-1,POS_INFINITY), Interval::NEG_REALS,       Interval(2,POS_INFINITY)); }
void TestArith::div2_10() { check_div2(Interval(2,3),   Interval::NEG_REALS,       Interval::NEG_REALS,       Interval::EMPTY_SET); }
void TestArith::div2_11() { check_div2(Interval(2,3),   Interval::POS_REALS,       Interval::POS_REALS,       Interval::EMPTY_SET); }
void TestArith::div2_12() { check_div2(Interval(-3,-2), Interval::NEG_REALS,       Interval::POS_REALS,       Interval::EMPTY_SET); }
void TestArith::div2_13() { check_div2(Interval(-3,-2), Interval::POS_REALS,       Interval::NEG_REALS,       Interval::EMPTY_SET); }
