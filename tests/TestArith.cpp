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
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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


void TestArith::float01() { CPPUNIT_ASSERT(0 < ibex::next_float(0)); }
void TestArith::float02() { CPPUNIT_ASSERT(0 > ibex::previous_float(0)); }
void TestArith::float03() { CPPUNIT_ASSERT( 1 < ibex::next_float(1)); }
void TestArith::float04() { CPPUNIT_ASSERT(1 > ibex::previous_float(1)); }
void TestArith::float05() { CPPUNIT_ASSERT( POS_INFINITY == ibex::next_float(POS_INFINITY)); }
void TestArith::float06() { CPPUNIT_ASSERT( -DBL_MAX == ibex::next_float(NEG_INFINITY)); }
void TestArith::float07() { CPPUNIT_ASSERT( DBL_MAX == ibex::previous_float(POS_INFINITY)); }
void TestArith::float08() { CPPUNIT_ASSERT( NEG_INFINITY == ibex::previous_float(NEG_INFINITY)); }
void TestArith::float09() { CPPUNIT_ASSERT( POS_INFINITY == ibex::next_float(DBL_MAX)); }
void TestArith::float10() {
	const double CNEG_INF = NEG_INFINITY;
    CPPUNIT_ASSERT( CNEG_INF == ibex::previous_float(-DBL_MAX));
}

void TestArith::minus01() { check(-Interval(0,1), Interval(-1,0)); }
void TestArith::minus02() { check(-Interval::all_reals(), Interval::all_reals()); }
void TestArith::minus03() { check(-Interval::neg_reals(), Interval::pos_reals()); }
void TestArith::minus04() { check(-Interval(NEG_INFINITY,1), Interval(-1,POS_INFINITY)); }

void TestArith::add01() { check_add(Interval::empty_set(),      Interval(0,1),      Interval::empty_set()); }
void TestArith::add02() { check_add(Interval(0,1),            Interval::empty_set(), Interval::empty_set()); }
void TestArith::add03() { check_add(Interval(NEG_INFINITY,1), Interval(0,1),      Interval(NEG_INFINITY, 2)); }
void TestArith::add04() { check_add(Interval(1,POS_INFINITY), Interval(0,1),      Interval(1,POS_INFINITY)); }
void TestArith::add05() { check_add(Interval::all_reals(),      Interval(0,1),      Interval::all_reals()); }

void TestArith::add06() { check_add_scal(Interval::empty_set(),      POS_INFINITY,       Interval::empty_set()); }
void TestArith::add07() { check_add_scal(Interval::empty_set(),      0,                  Interval::empty_set()); }
void TestArith::add08() { check_add_scal(Interval(0,1),            1,                  Interval(1,2)); }
void TestArith::add09() { check_add_scal(Interval(0,1),            NEG_INFINITY,       Interval::empty_set()); }
void TestArith::add10() { check_add_scal(Interval(0,1),            POS_INFINITY,       Interval::empty_set()); }
void TestArith::add11() { check_add_scal(Interval(NEG_INFINITY,1), 1,                  Interval(NEG_INFINITY,2)); }

void TestArith::add12() { check_add(Interval(DBL_MAX,POS_INFINITY), 1, Interval(DBL_MAX,POS_INFINITY)); }
void TestArith::add13() { check_add(Interval(DBL_MAX,POS_INFINITY), -1, Interval(ibex::previous_float(DBL_MAX),POS_INFINITY)); }
void TestArith::add14() { check_add(Interval(DBL_MAX,POS_INFINITY), Interval(DBL_MAX,POS_INFINITY), Interval(DBL_MAX,POS_INFINITY)); }
void TestArith::add15() { check_add(Interval(DBL_MAX,POS_INFINITY), NEG_INFINITY, Interval::empty_set()); }
void TestArith::add16() { check_add(Interval(DBL_MAX,POS_INFINITY), POS_INFINITY, Interval::empty_set()); }
void TestArith::add17() { check_add(Interval(NEG_INFINITY,-DBL_MAX),  1, Interval(NEG_INFINITY,ibex::next_float(-DBL_MAX))); }
void TestArith::add18() { check_add(Interval(NEG_INFINITY,-DBL_MAX), -1, Interval(NEG_INFINITY,-DBL_MAX)); }
void TestArith::add19() { check_add(Interval(NEG_INFINITY,-DBL_MAX),  Interval(NEG_INFINITY,-DBL_MAX), Interval(NEG_INFINITY,-DBL_MAX)); }


void TestArith::mul01() { check_mul(Interval::empty_set(),         Interval(0,1),               Interval::empty_set()); }
void TestArith::mul02() { check_mul(Interval::zero(),              Interval::all_reals(),         Interval::zero()); }
void TestArith::mul03() { check_mul(Interval(-1,1),              Interval::neg_reals(), 	     Interval::all_reals()); }

void TestArith::mul04() { check_mul(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval::pos_reals()); }
void TestArith::mul05() { check_mul(Interval(NEG_INFINITY, 1),   Interval(-1,0),              Interval(-1,POS_INFINITY)); }
void TestArith::mul06() { check_mul(Interval(0, 1),              Interval(1,POS_INFINITY), 	 Interval::pos_reals()); }
void TestArith::mul07() { check_mul(Interval(0, 1),              Interval(-1,POS_INFINITY),   Interval(-1,POS_INFINITY)); }

void TestArith::mul08() { check_mul(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval::neg_reals()); }
void TestArith::mul09() { check_mul(Interval(NEG_INFINITY, 1),   Interval(0,1),               Interval(NEG_INFINITY,1)); }
void TestArith::mul10() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,-1),   Interval::neg_reals()); }
void TestArith::mul11() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,1),    Interval(NEG_INFINITY,1)); }

void TestArith::mul12() { check_mul(Interval(1,POS_INFINITY),    Interval(0,1),               Interval::pos_reals()); }
void TestArith::mul13() { check_mul(Interval(-1,POS_INFINITY),   Interval(0,1),               Interval(-1,POS_INFINITY)); }

void TestArith::mul14() { check_mul(Interval(1,2),               Interval(1,2), 	        	 Interval(1,4)); }
void TestArith::mul15() { check_mul(Interval(1,2),               Interval(-2,3), 	         Interval(-4,6)); }
void TestArith::mul16() { check_mul(Interval(-1,1),              Interval(-2,3), 	         Interval(-3,3)); }

void TestArith::mul17() { check_mul_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::empty_set()); }
void TestArith::mul18() { check_mul_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::empty_set()); }
void TestArith::mul19() { check_mul_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

void TestArith::mulMM01() {
	double _tab[] = { 1, 2, 3, 4, 5, 6 };
	Matrix M(3,2,_tab);
	M*=M.transpose();
	double _res[] = {5, 11 , 17,
			11 , 25 , 39,
			17 , 39 , 61 };
	Matrix M_expected(3,3,_res);
	CPPUNIT_ASSERT(M==M_expected);
}

void TestArith::div01() { check_div(Interval::empty_set(),         Interval(0,1),               Interval::empty_set()); }
void TestArith::div02() { check_div(Interval::zero(),              Interval::zero(),              Interval::empty_set()); }
void TestArith::div03() { check_div(Interval(1,2),               Interval::zero(),              Interval::empty_set()); }
void TestArith::div04() { check_div(Interval::all_reals(),         Interval::zero(),              Interval::empty_set()); }
void TestArith::div05() { check_div(Interval::zero(),              Interval(0,1),               Interval::zero()); }
void TestArith::div06() { check_div(Interval::zero(),              Interval::all_reals(),         Interval::zero()); }
void TestArith::div07() { check_div(Interval(6,12),              Interval(2,3),               Interval(2,6)); }
void TestArith::div08() { check_div(Interval(6,12),              Interval(-3,-2),             Interval(-6,-2)); }
void TestArith::div09() { check_div(Interval(6,12),              Interval(-2,3),              Interval::all_reals()); }
void TestArith::div10() { check_div(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval(1,POS_INFINITY)); }
void TestArith::div11() { check_div(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval(NEG_INFINITY,-1)); }
void TestArith::div12() { check_div(Interval(1,POS_INFINITY),    Interval(-1,0),              Interval(NEG_INFINITY,-1)); }
void TestArith::div13() { check_div(Interval(1,POS_INFINITY),    Interval(0,1),               Interval(1,POS_INFINITY)); }
void TestArith::div14() { check_div(Interval(-1,1),              Interval(-1,1), 	         Interval::all_reals()); }
void TestArith::div15() { check_div_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::empty_set()); }
void TestArith::div16() { check_div_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::empty_set()); }
void TestArith::div17() { check_div_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }

void TestArith::sqrt01() { check(sqrt(Interval::all_reals()), Interval::pos_reals()); }
void TestArith::sqrt02() { check(sqrt(Interval::neg_reals()), Interval::zero()); }
void TestArith::sqrt03() { check(sqrt(Interval(-9,4)),      Interval(0,2)); }
void TestArith::sqrt04() { check(sqrt(Interval(4,9)),       Interval(2,3)); }
void TestArith::sqrt05() { check(sqrt(Interval(-9,-4)),     Interval::empty_set()); }
void TestArith::sqrt06() { check(sqrt(Interval(-9,POS_INFINITY)),     Interval::pos_reals()); }

#define piL Interval::pi().lb()
#define piU Interval::pi().ub()

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

void TestArith::sinh01() { check_sinh(Interval::all_reals()); }
void TestArith::sinh02() { check_sinh(Interval::pos_reals()); }
void TestArith::sinh03() { check_sinh(Interval(0,1)); }
void TestArith::sinh04() { check_sinh(Interval(1,POS_INFINITY)); }
void TestArith::sinh05() { check_sinh(Interval(1,1)); }
void TestArith::sinh06() { check_sinh(Interval(2,3)); }
void TestArith::sinh07() { check_sinh(Interval(4,5)); }


void TestArith::check_cosh(const Interval& x) {

	Interval y ;
	if (x.ub()==POS_INFINITY) {
		if (x.lb()<=0) y=Interval(1,POS_INFINITY);
		else y=Interval((cosh(x.lb())),POS_INFINITY);
	}
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()>=0) y=Interval(1,POS_INFINITY);
		else y=Interval((cosh(x.ub())),POS_INFINITY);
	}
	else if (x.lb()>=0)
		y=Interval((cosh(x.lb())),(cosh(x.ub())));
	else if (x.ub()<=0)
		y=Interval((cosh(x.ub())),(cosh(x.lb())));
	else
		y=((fabs(x.lb())> fabs(x.ub())) ? Interval(1,(cosh(x.lb()))) :Interval(1,(cosh(x.ub()))));


//	std::cout << x<<" : " << cosh(x)<< "|||  "<<y<<std::endl;
	check(cosh(x), y);
	check(cosh(-x), y);
//	check(x,acosh(cosh(x)));
//	check(x,acosh(cosh(-x)));
}

void TestArith::cosh01() { check_cosh(Interval::all_reals()); }
void TestArith::cosh02() { check_cosh(Interval::pos_reals()); }
void TestArith::cosh03() { check_cosh(Interval(0,1)); }
void TestArith::cosh04() { check_cosh(Interval(1,POS_INFINITY)); }
void TestArith::cosh05() { check_cosh(Interval(1,1)); }
void TestArith::cosh06() { check_cosh(Interval(2,3)); }
void TestArith::cosh07() { check_cosh(Interval(4,5)); }


void TestArith::check_trigo(const Interval& x, const Interval& sin_x_expected) {
	check(sin(x), sin_x_expected);
	check(sin(Interval::pi()-x), sin_x_expected);
	check(sin(x+Interval::two_pi()), sin_x_expected);
	check(sin(-x), -sin_x_expected);
	check(cos(x-Interval::half_pi()), sin_x_expected);
	check(cos(Interval::half_pi()-x), sin_x_expected);
	check(cos(x+Interval::half_pi()), -sin_x_expected);
	check(cos(x+Interval::two_pi()-Interval::half_pi()), sin_x_expected);
}

void TestArith::log01() { check(log(Interval::empty_set()), Interval::empty_set()); }
void TestArith::log02() { check(log(Interval::all_reals()), Interval::all_reals()); }
void TestArith::log03() { check(log(Interval::pos_reals()), Interval::all_reals()); }
void TestArith::log04() { check(log(Interval::neg_reals()),  Interval::empty_set()); /*Interval(NEG_INFINITY,-DBL_MAX));*/ }
void TestArith::log05() { check(log(Interval(1,2)),       Interval(0,::log(2))); }
void TestArith::log06() { check(log(Interval(-1,1)),      Interval(NEG_INFINITY,0)); }
void TestArith::log07() { CPPUNIT_ASSERT((log(Interval(0,ibex::next_float(0)))).ub()> -744.5); }
void TestArith::log08() { check(log(Interval(0,1)),       Interval(NEG_INFINITY,0)); }
void TestArith::log09() { check(log(Interval(1,POS_INFINITY)), Interval::pos_reals()); }
void TestArith::log10() { check(log(Interval(0)), Interval::empty_set()); /* Interval(NEG_INFINITY,-DBL_MAX)); */ }
void TestArith::log11() { check(log(Interval(-2,-1)), Interval::empty_set()); }

void TestArith::exp01() { check(exp(Interval::empty_set()), Interval::empty_set()); }
void TestArith::exp02() { check(exp(Interval::all_reals()), Interval::pos_reals()); }
void TestArith::exp03() { check(exp(Interval::pos_reals()), Interval(1,POS_INFINITY)); }
void TestArith::exp04() { check(exp(Interval::neg_reals()), Interval(0,1)); }
void TestArith::exp05() { check(exp(Interval(0,2)),       Interval(1,::exp(2))); }
void TestArith::exp06() { check(exp(Interval(-1,1)),      Interval(::exp(-1),::exp(1))); }
void TestArith::exp07() { check(exp(Interval(1.e100,1.e111)),Interval(DBL_MAX,POS_INFINITY)); }
void TestArith::exp08() { check(exp(Interval(DBL_MAX,POS_INFINITY)),Interval(DBL_MAX,POS_INFINITY)); }
void TestArith::exp09() { check(exp(Interval(0,DBL_MAX)),Interval(1,POS_INFINITY)); }

void TestArith::sin01() { check_trigo(Interval::all_reals(), Interval(-1,1)); }
void TestArith::sin02() { check_trigo(Interval::empty_set(), Interval::empty_set()); }
void TestArith::sin03() { check_trigo(Interval(0,piU/2.0), Interval(0,1)); }
void TestArith::sin04() { check_trigo(Interval(0,piU), Interval(0,1)); }
void TestArith::sin05() { check_trigo(Interval(0,3*piU/2.0), Interval(-1,1)); }
void TestArith::sin06() { check_trigo(Interval(piL,3*piU/2.0), Interval(-1,0)); }
void TestArith::sin07() { check_trigo(Interval(0.5,1.5), Interval(sin(0.5),sin(1.5))); }
void TestArith::sin08() { check_trigo(Interval(1.5,3), Interval(sin(3.0),1)); }
void TestArith::sin09() { check_trigo(Interval(3,4), Interval(sin(4.0),sin(3.0))); }
void TestArith::sin10() { check_trigo(Interval(3,5), Interval(-1,sin(3.0))); }
void TestArith::sin11() { check_trigo(Interval(3,2*piU+1.5), Interval(-1,sin(1.5))); }
void TestArith::sin12() { check_trigo(Interval(5,2*piU+1.5), Interval(sin(5.0),sin(1.5))); }
void TestArith::sin13() { check_trigo(Interval(5,2*piU+3), Interval(sin(5.0),1)); }

void TestArith::tan01() { check(tan(Interval::all_reals()), Interval::all_reals()); }
void TestArith::tan02() { check(tan(-Interval::pi()/4.0 | Interval::pi()/4.0), Interval(-1,1)); }
void TestArith::tan03() {	// tan(pi/4,pi/2)=[1,+oo)
	Interval x=Interval(piL/4.0,(1-1e-10)*piL/2.0); // upper bound of x is close to pi/2
	Interval y=tan(x);
	check(y.lb(),1.0);
	CPPUNIT_ASSERT(y.ub()>1000); // upper bound of tan(x) is close to +oo
}
void TestArith::tan04() {	// tan(-pi/2,pi/4)=(-oo,1]
	Interval y=tan(Interval(-(1-1e-10)*piL/2.0,piL/4.0));
	CPPUNIT_ASSERT(y.lb()<-1000); // lower bound is close to -oo
	check(y.ub(),1.0);
}
void TestArith::tan05() { check(tan(Interval::pi()/2.0),Interval::all_reals()); }
void TestArith::tan06() { check(tan(-Interval::pi()),Interval::all_reals()); }
void TestArith::tan07() { check(tan(3*Interval::pi()/4.0 | 5*Interval::pi()/4.0), Interval(-1,1)); }

void TestArith::floor01() { CPPUNIT_ASSERT(floor(Interval::empty_set())==Interval::empty_set()); }
void TestArith::floor02() { CPPUNIT_ASSERT(floor(Interval(NEG_INFINITY,-0.000001))==Interval(NEG_INFINITY,-1)); }
void TestArith::floor03() { CPPUNIT_ASSERT(floor(Interval(0.00000001,POS_INFINITY))==Interval::pos_reals()); }
void TestArith::floor04() { CPPUNIT_ASSERT(floor(Interval::all_reals())==Interval::all_reals()); }
void TestArith::floor05() { CPPUNIT_ASSERT(floor(Interval(0.01,2.99))==Interval(0,2)); }
void TestArith::floor06() { CPPUNIT_ASSERT(floor(Interval(-0.01,2.99))==Interval(-1,2)); }
void TestArith::floor07() { CPPUNIT_ASSERT(floor(Interval(1.000000001,1.9999999999))==Interval::one()); }
void TestArith::floor08() { CPPUNIT_ASSERT(floor(Interval(1e8, DBL_MAX))==Interval(1e8,DBL_MAX)); }

void TestArith::ceil01() { CPPUNIT_ASSERT(ceil(Interval::empty_set())==Interval::empty_set()); }
void TestArith::ceil02() { CPPUNIT_ASSERT(ceil(Interval(NEG_INFINITY,-0.000001))==Interval::neg_reals()); }
void TestArith::ceil03() { CPPUNIT_ASSERT(ceil(Interval(0.00000001,POS_INFINITY))==Interval(1.0,POS_INFINITY)); }
void TestArith::ceil04() { CPPUNIT_ASSERT(ceil(Interval::all_reals())==Interval::all_reals()); }
void TestArith::ceil05() { CPPUNIT_ASSERT(ceil(Interval(0.01,2.99))==Interval(1,3)); }
void TestArith::ceil06() { CPPUNIT_ASSERT(ceil(Interval(-0.01,2.99))==Interval(0,3)); }
void TestArith::ceil07() { CPPUNIT_ASSERT(ceil(Interval(1.000000001,1.9999999999))==Interval(2,2)); }
void TestArith::ceil08() { CPPUNIT_ASSERT(ceil(Interval(1e8, DBL_MAX))==Interval(1e8,DBL_MAX)); }

void TestArith::integer01() { CPPUNIT_ASSERT(integer(Interval::empty_set())==Interval::empty_set()); }
void TestArith::integer02() { CPPUNIT_ASSERT(integer(Interval(NEG_INFINITY,-0.000001))==Interval(NEG_INFINITY,-1)); }
void TestArith::integer03() { CPPUNIT_ASSERT(integer(Interval(0.00000001,POS_INFINITY))==Interval(1.0,POS_INFINITY)); }
void TestArith::integer04() { CPPUNIT_ASSERT(integer(Interval::all_reals())==Interval::all_reals()); }
void TestArith::integer05() { CPPUNIT_ASSERT(integer(Interval(0.01,2.99))==Interval(1,2)); }
void TestArith::integer06() { CPPUNIT_ASSERT(integer(Interval(-0.01,2.99))==Interval(0,2)); }
void TestArith::integer07() { CPPUNIT_ASSERT(integer(Interval(1.000000001,1.9999999999))==Interval::empty_set()); }
void TestArith::integer08() { CPPUNIT_ASSERT(integer(Interval(1e8, DBL_MAX))==Interval(1e8,DBL_MAX)); }

void TestArith::atan2_01() { check(atan2(Interval::all_reals(),Interval::empty_set()),Interval::empty_set()); }
void TestArith::atan2_02() { check(atan2(Interval::one(),Interval::one()),     Interval::pi()/4.0); }
void TestArith::atan2_03() { check(atan2(-Interval::one(),-Interval::one()),-3*Interval::pi()/4.0); }
void TestArith::atan2_04() { check(atan2(-Interval::one(),Interval::one()),   -Interval::pi()/4.0); }
void TestArith::atan2_05() { check(atan2(Interval::one(),-Interval::one()),  3*Interval::pi()/4.0); }
void TestArith::atan2_06() { check(atan2(Interval::pos_reals(),Interval::pos_reals()), Interval(0,1)*Interval::half_pi()); }
void TestArith::atan2_07() { check(atan2(Interval(NEG_INFINITY,ibex::previous_float(0.0)),Interval::neg_reals()), -(Interval::half_pi() | Interval::pi())); }
void TestArith::atan2_08() { check(atan2(Interval::neg_reals(),Interval::pos_reals()), Interval(-1,0)*Interval::half_pi()); }
void TestArith::atan2_09() { check(atan2(Interval::pos_reals(),Interval::neg_reals()), (Interval::half_pi() | Interval::pi())); }
void TestArith::atan2_10() { check(atan2(Interval(1,POS_INFINITY),Interval(-1,1)), Interval::pi()/4.0 | 3*Interval::pi()/4.0); }
void TestArith::atan2_11() { check(atan2(Interval(NEG_INFINITY,-1),Interval(-1,1)), -(Interval::pi()/4.0 | 3*Interval::pi()/4.0)); }
void TestArith::atan2_12() { check(atan2(Interval(-1,1),Interval(1,POS_INFINITY)), Interval(-1,1)*Interval::pi()/4.0); }
void TestArith::atan2_13() { check(atan2(Interval(-1,1),Interval(NEG_INFINITY,-1)), Interval(-1,1)*Interval::pi()); }
void TestArith::atan2_14() { check(atan2(Interval::all_reals(),Interval::all_reals()),Interval(-1,1)*Interval::pi()); }
void TestArith::atan2_15() { check(atan2(Interval::zero(),Interval::zero()),Interval::empty_set()); }

void TestArith::check_pow(const Interval& x, int p, const Interval& y_expected) {
	check(pow(x,p),y_expected);
	check(pow(-x,p),(p%2==0)? y_expected : -y_expected);
}

void TestArith::pow01() { check_pow(Interval::all_reals(), 4, Interval::pos_reals()); }
void TestArith::pow02() { check_pow(Interval::empty_set(), 4, Interval::empty_set()); }
void TestArith::pow03() { check_pow(Interval(2,3), 4, Interval(16,81)); }
void TestArith::pow04() { check_pow(Interval(-2,3), 4, Interval(0,81)); }
void TestArith::pow05() { check_pow(Interval(-3,2), 4, Interval(0,81)); }
void TestArith::pow06() { check_pow(Interval(2,POS_INFINITY), 4, Interval(16,POS_INFINITY)); }
void TestArith::pow07() { check_pow(Interval::all_reals(), 3, Interval::all_reals()); }
void TestArith::pow08() { check_pow(Interval::empty_set(), 3, Interval::empty_set()); }
void TestArith::pow09() { check_pow(Interval(2,3), 3, Interval(8,27)); }
void TestArith::pow10() { check_pow(Interval(-2,3), 3, Interval(-8,27)); }
void TestArith::pow11() { check_pow(Interval(-3,2), 3, Interval(-27,8)); }
void TestArith::pow12() { check_pow(Interval(2,POS_INFINITY), 3, Interval(8,POS_INFINITY)); }
void TestArith::pow13() { check_pow(Interval(-10,10), -2, Interval(1.0/100,POS_INFINITY)); }

void TestArith::root01() { check(root(Interval(0,1),-1), Interval(1.0,POS_INFINITY)); }
void TestArith::root02() { CPPUNIT_ASSERT(almost_eq(root(Interval(-27,-8), 3),Interval(-3,-2),1e-10)); }
void TestArith::root03() { check(root(Interval(-4,1),2), Interval(0,1)); }
void TestArith::root04() { check(root(Interval(-8,1),3), Interval(-2,1)); }

#define checkproj(func,y,xbefore,xafter) { Interval x=xbefore; NAME2(bwd_,func)(y,x); check(x,xafter); }
#define NAME2(a,b)         NAME2_HIDDEN(a,b)
#define NAME2_HIDDEN(a,b)  a ## b

void TestArith::checkbwd_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter) {
    checkproj(sin,  y, xbefore, xafter);
	checkproj(sin, -y,-xbefore,-xafter);
	checkproj(cos,  y, xbefore-Interval::half_pi(), xafter-Interval::half_pi());
	checkproj(cos, -y, xbefore+Interval::half_pi(), xafter+Interval::half_pi());
}

#define checkprojpow(p,y,xbefore,xafter) {

void TestArith::checkbwd_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p) {
	Interval x;
	x=xbefore; bwd_pow(y,p,x); check(x,xafter);
	x=-xbefore; bwd_pow(p%2==0? y : -y,p,x); check(x,-xafter);
}

bool TestArith::checkbwd_mul(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
								const Interval& x1_aft, const Interval& x2_aft) {
	bool res=true;
	Interval x1;
	Interval x2;

	x1=x1_bef;
	x2=x2_bef;
	bwd_mul(y,x1,x2);
	//cout << "x1=" << x1 << " x2=" << x2 << endl;
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=x1_bef;
	x2=x2_bef;
	bwd_mul(y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=-x1_bef;
	x2=x2_bef;
	bwd_mul(-y,x2,x1);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=x1_bef;
	x2=-x2_bef;
	bwd_mul(-y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	return res;
}

void TestArith::bwd_mul01() {
	CPPUNIT_ASSERT(checkbwd_mul(Interval(1,2),Interval(0.1,2.0),Interval(-10,2),Interval(0.5,2.0),Interval(0.5,2)));
}

void TestArith::bwd_mul02() {
	CPPUNIT_ASSERT(checkbwd_mul(Interval::empty_set(),Interval(0.1,2.0),Interval(-10,2),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_mul03() {
	CPPUNIT_ASSERT(checkbwd_mul(Interval(0,0),Interval(0.1,2.0),Interval(-10,2),Interval(0.1,2.0),Interval(0,0)));
}

void TestArith::bwd_mul04() {
	CPPUNIT_ASSERT(checkbwd_mul(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1)));
}

void TestArith::bwd_mul05() {
	CPPUNIT_ASSERT(checkbwd_mul(Interval(1,1),Interval(0,10),Interval(0,10),Interval(0.1,10.0),Interval(0.1,10.0)));
}

void TestArith::bwd_mulVV01() {
	IntervalVector a(2);
	a[0]=Interval(2,4);
	a[1]=Interval(-0.1,0.1);

	Interval b(2,3);

	IntervalVector x(2,Interval(-10,10));

	bwd_mul(b,a,x);
	check(x[0],Interval(1.0/4,2));
	check(x[1],Interval(-10,10));
}

void TestArith::bwd_mulMV01() {
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
	bwd_mul(b,A,x,epsilon);

	CPPUNIT_ASSERT(almost_eq(x,x2,epsilon));
}

void TestArith::checkbwd_div(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
		const Interval& x1_aft, const Interval& x2_aft) {
	Interval x1;
	Interval x2;

	x1=x1_bef;
	x2=x2_bef;
	bwd_div(y,x1,x2);
	check(x1,x1_aft);
	check(x2,x2_aft);

	x1=-x1_bef;
	x2=-x2_bef;
	bwd_div(y,x1,x2);
	check(x1,-x1_aft);
	check(x2,-x2_aft);

	x1=-x1_bef;
	x2=x2_bef;
	bwd_div(-y,x1,x2);
	check(x1,-x1_aft);
	check(x2,x2_aft);

	x1=x1_bef;
	x2=-x2_bef;
	bwd_div(-y,x1,x2);
	check(x1,x1_aft);
	check(x2,-x2_aft);
}

void TestArith::bwd_div01() { checkbwd_div(Interval(1,2),  Interval(0,1),   Interval(2,3),  Interval::empty_set(), Interval::empty_set()); }
void TestArith::bwd_div02() { checkbwd_div(Interval(1,2),  Interval(0,1),   Interval(1,3),  1,                   1); }
void TestArith::bwd_div03() { checkbwd_div(Interval(1,2),  Interval(1,3),   Interval(0,1),  Interval(1,2),       Interval(0.5,1)); }
void TestArith::bwd_div04() { checkbwd_div(Interval(-1,1), Interval(-2,2),  Interval(0,1),  Interval(-1,1),      Interval(0,1)); }
void TestArith::bwd_div05() { checkbwd_div(Interval(-1,1), Interval(-2,2),  Interval::zero(), Interval::zero(),      Interval::zero()); }
void TestArith::bwd_div06() { checkbwd_div(Interval::zero(), Interval(-2,2),  Interval(-2,2), Interval::zero(),      Interval(-2,2)); }
void TestArith::bwd_div07() { checkbwd_div(Interval::pos_reals(), Interval(0,1),  Interval(-1,0), Interval::zero(),   Interval(-1,0)); }
// note: 0/0 can be any number.
void TestArith::bwd_div08() { checkbwd_div(Interval(ibex::next_float(0),POS_INFINITY), Interval(0,1), Interval(-1,0), Interval::zero(), Interval::zero()); }
void TestArith::bwd_div09() { checkbwd_div(Interval::pos_reals(), Interval(ibex::next_float(0),1),  Interval(-1,0),  Interval::empty_set(), Interval::empty_set()); }


void TestArith::checkbwd_max(const Interval& z,  const Interval& x, const Interval& y, const Interval& x_expected, const Interval& y_expected) {

	Interval _x=x;
	Interval _y=y;
	bwd_max(z,_x,_y);

	CPPUNIT_ASSERT(x_expected==_x);
	CPPUNIT_ASSERT(y_expected==_y);

	// swap the inputs
	_x=x;
	_y=y;
	bwd_max(z,_y,_x);

	CPPUNIT_ASSERT(x_expected==_x);
	CPPUNIT_ASSERT(y_expected==_y);

	_x=-x;
	_y=-y;
	bwd_min(-z,_x,_y);

	CPPUNIT_ASSERT(x_expected==-_x);
	CPPUNIT_ASSERT(y_expected==-_y);

	// swap the inputs
	_x=-x;
	_y=-y;
	bwd_min(-z,_y,_x);

	CPPUNIT_ASSERT(x_expected==-_x);
	CPPUNIT_ASSERT(y_expected==-_y);
}

void TestArith::bwd_maxmin01() { checkbwd_max(Interval::empty_set(), Interval(-2,-1),Interval(-2,3),Interval::empty_set(),Interval::empty_set()); }
void TestArith::bwd_maxmin02() { checkbwd_max(Interval(0,1),       Interval(-2,-1),Interval(-2,3),Interval(-2,-1),    Interval(0,1)); }
void TestArith::bwd_maxmin03() { checkbwd_max(Interval(0,1),       Interval(-2,-1),Interval(2,3), Interval::empty_set(),Interval::empty_set()); }
void TestArith::bwd_maxmin04() { checkbwd_max(Interval(0,1),       Interval(-2,0), Interval(-2,3),Interval(-2,0),    Interval(-2,1)); }
void TestArith::bwd_maxmin05() { checkbwd_max(Interval(0,1),       Interval(-2,2), Interval(-2,3),Interval(-2,1),    Interval(-2,1)); }

void TestArith::bwd_sqr01() { checkproj(sqr, Interval(1,9),            Interval(0,4),       Interval(1,3)); }
void TestArith::bwd_sqr02() { checkproj(sqr, Interval(1,9),            Interval(0,2),       Interval(1,2)); }
void TestArith::bwd_sqr03() { checkproj(sqr, Interval(1,9),            Interval(-4,2),      Interval(-3,2)); }
void TestArith::bwd_sqr04() { checkproj(sqr, Interval(1,9),            Interval(-4,-3),     Interval(-3,-3)); }
void TestArith::bwd_sqr05() { checkproj(sqr, Interval(NEG_INFINITY,9), Interval(-4,1),      Interval(-3,1)); }
void TestArith::bwd_sqr06() { checkproj(sqr, Interval(4,9),            Interval(-1,5),      Interval(2,3)); }
void TestArith::bwd_sqr07() { checkproj(sqr, Interval(-4,-2),          Interval::all_reals(), Interval::empty_set()); }

void TestArith::bwd_log01() { checkproj(log, Interval::all_reals(),      Interval::all_reals(), Interval::pos_reals()); }
void TestArith::bwd_log02() { checkproj(log, Interval::neg_reals(),      Interval::all_reals(), Interval(0,1)); }
void TestArith::bwd_log03() { checkproj(log, Interval::pos_reals(),      Interval(0,1),       1); }
void TestArith::bwd_log04() { checkproj(log, Interval(0,1),      		Interval(-1,3),      Interval(1,::exp(1))); }
void TestArith::bwd_log05() { checkproj(log, Interval(NEG_INFINITY,1), Interval(-1,3),      Interval(0,::exp(1))); }
void TestArith::bwd_log06() { checkproj(log, Interval(NEG_INFINITY,1), Interval(-1,2),      Interval(0,2)); }
void TestArith::bwd_log07() { checkproj(log, Interval(NEG_INFINITY,1), Interval(3,4),       Interval::empty_set()); }
void TestArith::bwd_log08() { checkproj(log, Interval(-1,1),           Interval(-0.01,0.01),Interval::empty_set()); }


void TestArith::bwd_sin01() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(0,piU/2.0),        Interval(0.5,1.5)); }
void TestArith::bwd_sin02() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(0,5*piU/2.0),      Interval(0.5,2*piU+1.5)); }
void TestArith::bwd_sin03() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,1.5)); }
void TestArith::bwd_sin04() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,2*piU+1.5)); }
void TestArith::bwd_sin05() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-piL-0.4,piU/2.0), Interval(0.5,1.5)); }
void TestArith::bwd_sin06() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,1.5)); }
void TestArith::bwd_sin07() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU/2.0), Interval(-piU-1.5,1.5)); }
void TestArith::bwd_sin08() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piL-1.6),      Interval(0.5,1.5)); }
void TestArith::bwd_sin09() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU-1.5),      Interval(0.5,piU-1.5)); }
void TestArith::bwd_sin10() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestArith::bwd_sin11() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-piU-0.5,piU-1.5), Interval(-piU-0.5,piU-1.5)); }
void TestArith::bwd_sin12() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*piU/2,piU),     Interval(-piU-1.5,piU-0.5)); }
void TestArith::bwd_sin13() { checkbwd_trigo(sin(Interval(0.5,1.5)), Interval(-piU/2,piU/2.0),   Interval(0.5,1.5)); }
void TestArith::bwd_sin14() { checkbwd_trigo(Interval(2,3),          Interval::all_reals(),        Interval::empty_set()); }

void TestArith::bwd_sin15() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(0,piU/2.0),        Interval(0.5,piU/2.0)); }
void TestArith::bwd_sin16() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(0,5*piU/2.0),      Interval(0.5,5*piU/2.0)); }
void TestArith::bwd_sin17() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,piU/2.0),   Interval(-2*piU+0.5,piU/2.0)); }
void TestArith::bwd_sin18() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-2*piU,5*piU/2.0), Interval(-2*piU+0.5,5*piU/2.0)); }
void TestArith::bwd_sin19() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-piL-0.4,piU/2.0), Interval(0.5,piU/2.0)); }
void TestArith::bwd_sin20() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-piU-0.5,piU/2.0), Interval(-piU-0.5,piU/2.0)); }
void TestArith::bwd_sin21() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU/2.0), Interval(-3*piU/2,piU/2.0)); }
void TestArith::bwd_sin22() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(0.5,piU),          Interval(0.5,piU-0.5)); }
void TestArith::bwd_sin23() { checkbwd_trigo(Interval(sin(0.5),1.0), Interval(-3*piU/2,piU),     Interval(-3*piU/2,piU-0.5)); }

// not the same case as before because
// when the image is > 1 or < -1 the box x is
// emptied by the call to asin (not by the
//  intersection scheme)
void TestArith::bwd_sin24() { checkbwd_trigo(Interval(1.0),          Interval(4.0,6.0),          Interval::empty_set()); }

void TestArith::bwd_pow01() { checkbwd_pow(Interval(16,81), Interval(1,4), Interval(2,3), 4); }
void TestArith::bwd_pow02() { checkbwd_pow(Interval(16,81), Interval(3,4), Interval(3,3), 4); }
void TestArith::bwd_pow03() { checkbwd_pow(Interval(16,81), Interval(-1,4), Interval(2,3), 4); }
void TestArith::bwd_pow04() { checkbwd_pow(Interval(16,81), Interval(-2,4), Interval(-2,3), 4); }
void TestArith::bwd_pow05() { checkbwd_pow(Interval(16,81), Interval(-5,4), Interval(-3,3), 4); }
void TestArith::bwd_pow06() { checkbwd_pow(Interval(16,81), Interval(1,1), Interval::empty_set(), 4); }
void TestArith::bwd_pow07() { checkbwd_pow(Interval(16,81), Interval(4,4), Interval::empty_set(), 4); }
void TestArith::bwd_pow08() { checkbwd_pow(Interval(8,27), Interval(1,4), Interval(2,3), 3); }
void TestArith::bwd_pow09() { checkbwd_pow(Interval(8,27), Interval(3,4), Interval(3,3), 3); }
void TestArith::bwd_pow10() { checkbwd_pow(Interval(8,27), Interval(-5,4), Interval(2,3), 3); }
void TestArith::bwd_pow11() { checkbwd_pow(Interval(-8,27), Interval(-5,4), Interval(-2,3), 3); }
void TestArith::bwd_pow12() { checkbwd_pow(Interval(-27,27), Interval(-5,4), Interval(-3,3), 3); }
void TestArith::bwd_pow13() { checkbwd_pow(Interval(8,27), Interval(1,1), Interval::empty_set(), 3); }
void TestArith::bwd_pow14() { checkbwd_pow(Interval(8,27), Interval(4,4), Interval::empty_set(), 3); }
void TestArith::bwd_pow15() { checkbwd_pow(Interval(-27,-8), Interval(-1,-1), Interval::empty_set(), 3); }
void TestArith::bwd_pow16() { checkbwd_pow(Interval(-27,-8), Interval(-4,-4), Interval::empty_set(), 3); }
void TestArith::bwd_pow17() { checkbwd_pow(Interval(0,1), Interval(-10,10), Interval(-10,10), -2); }

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
	CPPUNIT_ASSERT(_out2.is_empty());

	_inout=out2;
	_inout.div2_inter(x,y);
	check(_inout,out2);

	_inout=out2;
	_inout.div2_inter(x,y,_out2);
	check(_inout,out2);
	CPPUNIT_ASSERT(_out2.is_empty());

	_inout=Interval::all_reals();
	_inout.div2_inter(x,y);
	check(_inout,out1 | out2);

	_inout=Interval::all_reals();
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
		CPPUNIT_ASSERT(_out2.is_empty());

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

void TestArith::div2_01() { check_div2(Interval(6,12),  Interval(0,0),             Interval::empty_set(),       Interval::empty_set()); }
void TestArith::div2_02() { check_div2(Interval(6,12),  Interval(2,3),             Interval(2,6),             Interval::empty_set()); }
void TestArith::div2_03() { check_div2(Interval(-2,3),  Interval(-1,1),            Interval::all_reals(),       Interval::empty_set()); }
void TestArith::div2_04() { check_div2(Interval(2,3),   Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestArith::div2_05() { check_div2(Interval(-3,-2), Interval(-1,1),            Interval(NEG_INFINITY,-2), Interval(2,POS_INFINITY)); }
void TestArith::div2_06() { check_div2(Interval(2,3),   Interval(NEG_INFINITY,1),  Interval::neg_reals(),       Interval(2,POS_INFINITY)); }
void TestArith::div2_07() { check_div2(Interval(2,3),   Interval(-1,POS_INFINITY), Interval(NEG_INFINITY,-2), Interval::pos_reals()); }
void TestArith::div2_08() { check_div2(Interval(-3,-2), Interval(NEG_INFINITY,1),  Interval(NEG_INFINITY,-2), Interval::pos_reals()); }
void TestArith::div2_09() { check_div2(Interval(-3,-2), Interval(-1,POS_INFINITY), Interval::neg_reals(),       Interval(2,POS_INFINITY)); }
void TestArith::div2_10() { check_div2(Interval(2,3),   Interval::neg_reals(),       Interval::neg_reals(),       Interval::empty_set()); }
void TestArith::div2_11() { check_div2(Interval(2,3),   Interval::pos_reals(),       Interval::pos_reals(),       Interval::empty_set()); }
void TestArith::div2_12() { check_div2(Interval(-3,-2), Interval::neg_reals(),       Interval::pos_reals(),       Interval::empty_set()); }
void TestArith::div2_13() { check_div2(Interval(-3,-2), Interval::pos_reals(),       Interval::neg_reals(),       Interval::empty_set()); }

bool TestArith::checkbwd_atan2(const Interval& a, const Interval& y_bef, const Interval& x_bef,
								const Interval& y_aft, const Interval& x_aft) {
	bool res=true;
	Interval y;
	Interval x;

	y=y_bef;
	x=x_bef;
	bwd_atan2(a,y,x);
	//cout << "x1=" << x1 << " x2=" << x2 << endl;
	res &= almost_eq(x,x_aft,ERROR) && almost_eq(y,y_aft,ERROR);

	return res;
}

void TestArith::bwd_atan2_01() { //first quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(M_PI/6.,M_PI/3.),Interval(.5,10.),Interval(.5,2.),Interval(.5,2.*sqrt(3.)),Interval(.5,2.)));
}
void TestArith::bwd_atan2_02() { // first quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(M_PI/6.,M_PI/3.),Interval(.5,2.),Interval(.5,10.),Interval(.5,2.),Interval(.5,2.*sqrt(3.))));
}
void TestArith::bwd_atan2_03() { // left half
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-M_PI/4.,M_PI/4.),Interval(1.,2.),Interval(.5,2.),Interval(1.,2.),Interval(1.,2.)));
}
void TestArith::bwd_atan2_04() { //fourth quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-M_PI/2.,0.),Interval(.5,2.),Interval(.5,10.),Interval::empty_set(),Interval::empty_set()));
}
void TestArith::bwd_atan2_05() { //upper half
	// ** confusion with angle constraint **
	//CPPUNIT_ASSERT(checkbwd_atan2(Interval(2.*M_PI,3.*M_PI),Interval(-.5,2.),Interval(.5,10.),Interval(0. ,2.),Interval(.5,10.)));
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(2.*M_PI,3.*M_PI),Interval(-.5,2.),Interval(.5,10.),Interval::empty_set(), Interval::empty_set()));
}
void TestArith::bwd_atan2_06() { //second quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(2*M_PI/3.,5.*M_PI/6.),Interval(0.,100.),Interval(-20.,-sqrt(3.)/2.),Interval(.5,20.*sqrt(3.)),Interval(-20.,-sqrt(3.)/2)));
}
void TestArith::bwd_atan2_07() { //third quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-3*M_PI/4.,-2*M_PI/3.),Interval(-sqrt(3.)/2.,2.),Interval(-sqrt(2.)/2.,0.),Interval(-sqrt(3.)/2.,0.),Interval(-sqrt(2.)/2.,0.)));
}
void TestArith::bwd_atan2_08() { //third quadrant
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-3*M_PI/4.,-2*M_PI/3.),Interval(-sqrt(3.)/2.,2.),Interval(-1.,-.5),Interval(-sqrt(3.)/2.,-.5),Interval(-sqrt(3.)/2.,-.5)));
}
void TestArith::bwd_atan2_09() { //lower half
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-3*M_PI/4.,-M_PI/4.),Interval(-5.,-.5),Interval::all_reals(),Interval(-5.,-.5),Interval(-5.,5.)));
}
void TestArith::bwd_atan2_10() { //right half
	CPPUNIT_ASSERT(checkbwd_atan2(Interval(-M_PI/3.,M_PI/4.),Interval::all_reals(),Interval(sqrt(3.)/2.),Interval(-1.5,sqrt(3.)/2.),Interval(sqrt(3.)/2.)));
}

void TestArith::bwd_atan2_issue134() {
	CPPUNIT_ASSERT(checkbwd_atan2(Interval::half_pi(),Interval::one(), Interval::zero(), Interval::one(), Interval::zero()));
}

void TestArith::bwd_sqrt01() { checkproj(sqrt, Interval(1,3),				Interval(0,15),		Interval(1,9)); }
void TestArith::bwd_sqrt02() { checkproj(sqrt, Interval(1,3),				Interval(0,2),			Interval(1,2)); }
void TestArith::bwd_sqrt03() { checkproj(sqrt, Interval(1,3),				Interval(-4,2),		Interval(1,2)); }
void TestArith::bwd_sqrt04() { checkproj(sqrt, Interval(1,3),				Interval(-4,-3),		Interval::empty_set()); }
void TestArith::bwd_sqrt05() { checkproj(sqrt, Interval(NEG_INFINITY,9),	Interval(-4,1),		Interval(0,1)); }
void TestArith::bwd_sqrt06() { checkproj(sqrt, Interval(2,5),				Interval(-1,5),		Interval(4,5)); }
void TestArith::bwd_sqrt07() { checkproj(sqrt, Interval(-4,-2),				Interval::all_reals(),	Interval::empty_set()); }

bool TestArith::checkbwd_atan(const Interval& y, const Interval& xbefore, const Interval& xafter) {
	bool res=true;
	Interval x;

	// tan(y)=x
	x=xbefore;
	bwd_atan(y,x);
	//cout << "xbefore= " << xbefore << " y=" << y << " x=" << x << " versus " << xafter << endl;
	res &= almost_eq(x,xafter,ERROR);

	// tan(-y)=-x
	x=-xbefore;
	bwd_atan(-y,x);
	//cout << "xbefore= " << (-xbefore) << " y=" << (-y) << " x=" << x << " versus " << (-xafter) << endl;
	res &= almost_eq(-x,xafter,ERROR);

	return res;
}

void TestArith::bwd_atan01() { CPPUNIT_ASSERT(checkbwd_atan(Interval(0.,M_PI/6.),		  Interval(-1.,3.),			tan(Interval(0.,M_PI/6.)))); }
void TestArith::bwd_atan02() { CPPUNIT_ASSERT(checkbwd_atan(Interval(-M_PI,1.5), 		  Interval(0,5*M_PI/2.0),	Interval(0,5*M_PI/2.0))); }
void TestArith::bwd_atan03() { CPPUNIT_ASSERT(checkbwd_atan(Interval(0.,M_PI/6.),		  Interval(.2,.5),			Interval(.2,.5))); }
void TestArith::bwd_atan04() { CPPUNIT_ASSERT(checkbwd_atan(Interval(-M_PI/2-0.1,M_PI/2+0.1), Interval(-100,100),	Interval(-100,100))); }
void TestArith::bwd_atan05() { CPPUNIT_ASSERT(checkbwd_atan(Interval(M_PI/2+0.1,M_PI),	       Interval(-100,100),	Interval::empty_set())); }
void TestArith::bwd_atan06() { CPPUNIT_ASSERT(checkbwd_atan(Interval(-M_PI,-M_PI/2-0.1),      Interval(-100,100),	Interval::empty_set())); }

void TestArith::bwd_atan07() {
	Interval x=Interval::all_reals();
	bwd_atan(Interval(-M_PI/4,M_PI/2.), x);
	check(x.lb(), -1);
	CPPUNIT_ASSERT(x.ub() > 1000); // upper bound is close to +oo
}

void TestArith::bwd_atan08() {
	Interval x=Interval::all_reals();
	bwd_atan(Interval(-M_PI/2,M_PI/4.), x);
	check(x.ub(), +1);
	CPPUNIT_ASSERT(x.lb() < -1000); // lower bound is close to -oo
}


bool TestArith::checkbwd_add(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
								const Interval& x1_aft, const Interval& x2_aft) {
	bool res=true;
	Interval x1;
	Interval x2;

	x1=x1_bef;
	x2=x2_bef;
	bwd_add(y,x1,x2);
	//cout << "x1=" << x1 << " x2=" << x2 << endl;
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=x1_bef;
	x2=x2_bef;
	bwd_add(y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	x1=-x1_bef;
	x2=-x2_bef;
	bwd_add(-y,x1,x2);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	x1=-x1_bef;
	x2=-x2_bef;
	bwd_add(-y,x2,x1);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	return res;
}

void TestArith::bwd_add01() {
	CPPUNIT_ASSERT(checkbwd_add(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-1,2)));
}

void TestArith::bwd_add02() {
	CPPUNIT_ASSERT(checkbwd_add(Interval::empty_set(),Interval(0.1,2.0),Interval(-10,2),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_add03() {
	CPPUNIT_ASSERT(checkbwd_add(Interval(0,0),Interval(0,0),Interval(.1,5),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_add04() {
	CPPUNIT_ASSERT(checkbwd_add(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1)));
}

void TestArith::bwd_add05() {
	CPPUNIT_ASSERT(checkbwd_add(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-3,0)));
}

bool TestArith::checkbwd_sub(const Interval& y, const Interval& x1_bef, const Interval& x2_bef,
								const Interval& x1_aft, const Interval& x2_aft) {
	bool res=true;
	Interval x1;
	Interval x2;

	// y = x1 - x2
	x1=x1_bef;
	x2=x2_bef;
	bwd_sub(y,x1,x2);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	// -y = -x1 - -x2
	x1=-x1_bef;
	x2=-x2_bef;
	bwd_sub(-y,x1,x2);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	// -y = x2 - x1
	x1=x1_bef;
	x2=x2_bef;
	bwd_sub(-y,x2,x1);
	res &= almost_eq(x1,x1_aft,ERROR) && almost_eq(x2,x2_aft,ERROR);

	// y = -x2 - -x1
	x1=-x1_bef;
	x2=-x2_bef;
	bwd_sub(y,x2,x1);
	res &= almost_eq(x1,-x1_aft,ERROR) && almost_eq(x2,-x2_aft,ERROR);

	return res;
}

void TestArith::bwd_sub01() {
	CPPUNIT_ASSERT(checkbwd_sub(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-2,1)));
}

void TestArith::bwd_sub02() {
	CPPUNIT_ASSERT(checkbwd_sub(Interval::empty_set(),Interval(0.1,2.0),Interval(-10,2),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_sub03() {
	CPPUNIT_ASSERT(checkbwd_sub(Interval(0,0),Interval(0,0),Interval(.1,5),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_sub04() {
	CPPUNIT_ASSERT(checkbwd_sub(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1)));
}

void TestArith::bwd_sub05() {
	CPPUNIT_ASSERT(checkbwd_sub(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(0,3)));
}

bool TestArith::checkbwd_imod(const double& period, const Interval& x_bef, const Interval& y_bef,
								const Interval& x_aft, const Interval& y_aft) {
	bool res=true;
	Interval y;
	Interval x;

	y=y_bef;
	x=x_bef;
	bwd_imod(x,y,period);

	res &= almost_eq(x,x_aft,ERROR) && almost_eq(y,y_aft,ERROR);

	return res;
}

void TestArith::bwd_imod_01() {
	CPPUNIT_ASSERT(checkbwd_imod(3.,Interval(3.,5.),Interval(1.,2.),Interval(4.,5.),Interval(1.,2.)));
}

void TestArith::bwd_imod_02() {
	CPPUNIT_ASSERT(checkbwd_imod(2.,Interval(7.,8.),Interval(.5,2.),Interval(7.,8.),Interval(1.,2.)));
}

void TestArith::bwd_imod_03() {
	CPPUNIT_ASSERT(checkbwd_imod(2.,Interval(7.,8.),Interval(0.,2.),Interval(7.,8.),Interval(0.,2.)));
}

void TestArith::bwd_imod_04() {
	CPPUNIT_ASSERT(checkbwd_imod(2.*M_PI,Interval(2.*M_PI,3.*M_PI),Interval(M_PI/6,M_PI/2.),Interval(13.*M_PI/6.,5.*M_PI/2.),Interval(M_PI/6,M_PI/2.)));
}

void TestArith::bwd_imod_05() {
	CPPUNIT_ASSERT(checkbwd_imod(2.*M_PI,Interval(3.*M_PI,4.*M_PI),Interval(M_PI/3,M_PI/2.),Interval::empty_set(),Interval::empty_set()));
}

void TestArith::bwd_imod_06() {
	CPPUNIT_ASSERT(checkbwd_imod(2.*M_PI,Interval(3.*M_PI,4.*M_PI),Interval(0.,M_PI/2.),Interval(4*M_PI),Interval(0.)));
}

void TestArith::bwd_imod_07() {
	CPPUNIT_ASSERT(checkbwd_imod(2.*M_PI,Interval(2.*M_PI,4.*M_PI),Interval(-M_PI/6,M_PI/2.),Interval(2.*M_PI,4.*M_PI),Interval(-M_PI/6,M_PI/2.)));
}

void TestArith::bwd_imod_08() {
	CPPUNIT_ASSERT(checkbwd_imod(2.*M_PI,Interval(7.*M_PI/4.,8.*M_PI/3),Interval(-M_PI/2,M_PI/2.),Interval(7.*M_PI/4.,5.*M_PI/2.),Interval(-M_PI/4,M_PI/2.)));
}

bool TestArith::checkbwd_floor(const Interval& y, const Interval& xbefore, const Interval& x_after_expected) {
	Interval x(xbefore);
	bwd_floor(y,x);
	return x==x_after_expected;
}

void TestArith::bwd_floor_01() { CPPUNIT_ASSERT(checkbwd_floor(Interval::empty_set(), Interval::all_reals(), Interval::empty_set())); }
void TestArith::bwd_floor_02() { CPPUNIT_ASSERT(checkbwd_floor(Interval(NEG_INFINITY,-1), Interval(NEG_INFINITY,-0.000001), Interval(NEG_INFINITY,-0.000001))); }
void TestArith::bwd_floor_03() { CPPUNIT_ASSERT(checkbwd_floor(Interval(NEG_INFINITY,-1), Interval(NEG_INFINITY, 0.000001), Interval::neg_reals())); }
void TestArith::bwd_floor_04() { CPPUNIT_ASSERT(checkbwd_floor(Interval(NEG_INFINITY,-0.000001), Interval(NEG_INFINITY, 0.000001), Interval::neg_reals())); }
void TestArith::bwd_floor_05() { CPPUNIT_ASSERT(checkbwd_floor(Interval(1,POS_INFINITY), Interval(0.000001, POS_INFINITY), Interval(1,POS_INFINITY))); }
void TestArith::bwd_floor_06() { CPPUNIT_ASSERT(checkbwd_floor(Interval(1,POS_INFINITY), Interval(-0.000001, POS_INFINITY), Interval(1,POS_INFINITY))); }
void TestArith::bwd_floor_07() { CPPUNIT_ASSERT(checkbwd_floor(Interval(0.000001,POS_INFINITY), Interval(-0.000001, POS_INFINITY), Interval(1,POS_INFINITY))); }
void TestArith::bwd_floor_08() { CPPUNIT_ASSERT(checkbwd_floor(Interval(1,2), Interval(0.01,2.99), Interval(1,2.99))); }
void TestArith::bwd_floor_09() { CPPUNIT_ASSERT(checkbwd_floor(Interval(1,2), Interval(2.99,3.01), Interval(2.99,3))); }
void TestArith::bwd_floor_10() { CPPUNIT_ASSERT(checkbwd_floor(Interval(1,2), Interval(3.01,3.99), Interval::empty_set())); }
void TestArith::bwd_floor_11() { CPPUNIT_ASSERT(checkbwd_floor(Interval(0.01,2.99), Interval(0.01,2.99), Interval(1,2.99))); }
void TestArith::bwd_floor_12() { /*CPPUNIT_ASSERT(checkbwd_floor(Interval(0,numeric_limits<double>::max()), Interval(numeric_limits<double>::max(),POS_INFINITY), Interval(numeric_limits<double>::max(),numeric_limits<double>::max()))); */}
void TestArith::bwd_floor_13() { /*CPPUNIT_ASSERT(checkbwd_floor(Interval(numeric_limits<double>::min(),0), Interval(NEG_INFINITY,numeric_limits<double>::min()), Interval(numeric_limits<double>::min(),numeric_limits<double>::min()))); */ }

bool TestArith::checkbwd_ceil(const Interval& y, const Interval& xbefore, const Interval& x_after_expected) {
	Interval x(xbefore);
	bwd_ceil(y,x);
	return x==x_after_expected;
}

void TestArith::bwd_ceil_01() { CPPUNIT_ASSERT(checkbwd_ceil(Interval::empty_set(), Interval::all_reals(), Interval::empty_set())); }
void TestArith::bwd_ceil_02() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(NEG_INFINITY,-1), Interval(NEG_INFINITY,-0.000001), Interval(NEG_INFINITY,-1))); }
void TestArith::bwd_ceil_03() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(NEG_INFINITY,-1), Interval(NEG_INFINITY, 0.000001), Interval(NEG_INFINITY,-1))); }
void TestArith::bwd_ceil_04() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(NEG_INFINITY,-0.000001), Interval(NEG_INFINITY, 0.000001), Interval(NEG_INFINITY,-1))); }
void TestArith::bwd_ceil_05() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(1,POS_INFINITY), Interval(0.000001, POS_INFINITY), Interval(0.000001,POS_INFINITY))); }
void TestArith::bwd_ceil_06() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(1,POS_INFINITY), Interval(-0.000001, POS_INFINITY), Interval::pos_reals())); }
void TestArith::bwd_ceil_07() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(0.000001,POS_INFINITY), Interval(-0.000001, POS_INFINITY), Interval::pos_reals())); }
void TestArith::bwd_ceil_08() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(1,2), Interval(0.01,2.99), Interval(0.01,2))); }
void TestArith::bwd_ceil_09() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(1,2), Interval(-0.01,0.01), Interval(0,0.01))); }
void TestArith::bwd_ceil_10() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(1,2), Interval(-1.01,-0.01), Interval::empty_set())); }
void TestArith::bwd_ceil_11() { CPPUNIT_ASSERT(checkbwd_ceil(Interval(0.01,2.99), Interval(0.01,2.99), Interval(0.01,2))); }
void TestArith::bwd_ceil_12() { /*CPPUNIT_ASSERT(checkbwd_ceil(Interval(0,numeric_limits<double>::max()), Interval(numeric_limits<double>::max(),POS_INFINITY), Interval(numeric_limits<double>::max(),numeric_limits<double>::max()))); */}
void TestArith::bwd_ceil_13() { /*CPPUNIT_ASSERT(checkbwd_ceil(Interval(numeric_limits<double>::min(),0), Interval(NEG_INFINITY,numeric_limits<double>::min()), Interval(numeric_limits<double>::min(),numeric_limits<double>::min()))); */}

void TestArith::tan_issue248() {
	//Interval itv = Interval(-Interval::pi().lb()/2,3*Interval::pi().ub()/8);
	Interval itv = Interval(-1.57079632679489678, 1.1780972450961728626);
	CPPUNIT_ASSERT(!(tan(itv).is_empty()));
}
