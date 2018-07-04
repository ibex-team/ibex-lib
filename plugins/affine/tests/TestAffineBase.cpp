/* ============================================================================
 * I B E X - Test of Affine operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan NININ
 * Created     : Dec 07, 2017
 * ---------------------------------------------------------------------------- */

#include "TestAffineBase.h"
#include "utils.h"
#include <float.h>

using namespace std;

template<class T>
void  TestAffineBase<T>::change_mode_MinRange() {
	AffineMain<T>::change_mode(AffineMain<T>::AF_MinRange);
	CPPUNIT_ASSERT(true);
}


template<class T>
void  TestAffineBase<T>::check_affine_eq(const AffineMain<T>& y_actual, const Interval& y_expected, double err) {
	//std::cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< std::endl;
	if (y_expected.is_empty()) { CPPUNIT_ASSERT(y_actual.is_empty()); return; }

	CPPUNIT_ASSERT(!y_actual.is_empty());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.lb(),y_actual.itv().lb(),err);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y_expected.ub(),y_actual.itv().ub(),err);
}

template<class T>
void  TestAffineBase<T>::check_affine_inclu(const AffineMain<T>& y_actual, const Interval& y_expected, double err) {
	//std::cout << "check:    " << y_expected << " (expected)        " << y_actual << " (actual)"<< std::endl;
	if (y_expected.is_empty()) { CPPUNIT_ASSERT(y_actual.is_empty()); return; }

	CPPUNIT_ASSERT(!y_actual.is_empty());
	CPPUNIT_ASSERT(y_expected.lb()>=y_actual.itv().lb());
	CPPUNIT_ASSERT(y_expected.ub()<=y_actual.itv().ub());
}

template<class T>
void  TestAffineBase<T>::check_add(const Interval& x, const Interval& z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	AffineVarMain<T> za(z);
	check_add(xa,za,y_expected);
}

template<class T>
void  TestAffineBase<T>::check_add(const Interval& xi, double z, const Interval& y_expected) {
	AffineVarMain<T> x(xi);
	AffineMain<T>  y_actual=x+z;
	check_affine_eq(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z+x;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator
	y_actual = x;
	y_actual += z;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator in the other direction
	y_actual = z;
	y_actual += x;
	check_affine_eq(y_actual, y_expected);

	// test subtraction
	y_actual=(-x)-z;
	check_affine_eq(y_actual, -y_expected);

	// test the symmetrical case
	y_actual = (-z)-x;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator
	y_actual = -x;
	y_actual -= z;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator in the other direction
	y_actual = -z;
	y_actual -= x;
	check_affine_eq(y_actual, -y_expected);

	// test the linear simplification
	if (x.is_actif() && fabs(z)<1.e100) {
		y_actual = z ;
		y_actual += x ;
		y_actual -= x ;
		y_actual += x ;
		check_affine_eq(y_actual, y_expected);

		y_actual = x ;
		y_actual += z ;
		y_actual -= z ;
		y_actual += z ;
		check_affine_eq(y_actual, y_expected);
	}
}

template<class T>
void  TestAffineBase<T>::check_add(const AffineMain<T>& x, const AffineMain<T>& z, const Interval& y_expected) {
	AffineMain<T>  y_actual=x+z;
	check_affine_eq(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z+x;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator
	y_actual = x;
	y_actual += z;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator in the other direction
	y_actual = z;
	y_actual += x;
	check_affine_eq(y_actual, y_expected);

	// test subtraction
	y_actual=(-x)-z;
	check_affine_eq(y_actual, -y_expected);

	// test the symmetrical case
	y_actual = (-z)-x;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator
	y_actual = -x;
	y_actual -= z;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator in the other direction
	y_actual = -z;
	y_actual -= x;
	check_affine_eq(y_actual, -y_expected);

	// test the linear simplification
	if (x.is_actif() && z.is_actif()) {
		y_actual = z ;
		y_actual += x ;
		y_actual -= x ;
		y_actual += x ;
		check_affine_eq(y_actual, y_expected);

		y_actual = x ;
		y_actual += z ;
		y_actual -= z ;
		y_actual += z ;
		check_affine_eq(y_actual, y_expected);
	}
}

template<class T>
void  TestAffineBase<T>::check_add(const AffineMain<T>& x, const Interval& z, const Interval& y_expected) {
	AffineMain<T>  y_actual=x+z;
	check_affine_eq(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z+x;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator
	y_actual = x;
	y_actual += z;
	check_affine_eq(y_actual, y_expected);

	// test the +=operator in the other direction
	y_actual = z;
	y_actual += x;
	check_affine_eq(y_actual, y_expected);

	// test subtraction
	y_actual=(-x)-z;
	check_affine_eq(y_actual, -y_expected);

	// test the symmetrical case
	y_actual = (-z)-x;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator
	y_actual = -x;
	y_actual -= z;
	check_affine_eq(y_actual, -y_expected);

	// test the -=operator in the other direction
	y_actual = -z;
	y_actual -= x;
	check_affine_eq(y_actual, -y_expected);

	// test the linear simplification
	if (x.is_actif() && !z.is_unbounded() && !z.is_empty()) {
		y_actual = z ;
		y_actual += x ;
		y_actual -= x ;
		y_actual += x ;
		check_affine_eq(y_actual, y_expected);

		y_actual = x ;
		y_actual += z ;
		y_actual -= z ;
		y_actual += z ;
		check_affine_eq(y_actual, y_expected);
	}
}

template<class T>
void  TestAffineBase<T>::check_add_scal(const Interval& x, double z, const Interval& y_expected) {
	check_add(x,z, y_expected);
	check_add((-x),-z, -y_expected);
}

template<class T>
void  TestAffineBase<T>::check_add_scal(const Interval& x, const Interval& z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	check_add(xa,z,y_expected);
	check_add((-xa),-z, -y_expected);
}

template<class T>
void  TestAffineBase<T>::check_mul(const Interval& x, const Interval& z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	AffineVarMain<T> za(z);
	check_mul(xa,za,y_expected);

}

template<class T>
void  TestAffineBase<T>::check_mul(const AffineMain<T>& x, const AffineMain<T>& z, const Interval& y_expected) {
	AffineMain<T> y_actual=x*z;
	//cout << "check:    " << x << " * " << z << ", " << y_expected << endl;
	//cout << "      out:" << y_actual << endl;
	check_affine_inclu(y_actual, y_expected);

	// test the symmetrical case
	y_actual = z*x;
	check_affine_inclu(y_actual, y_expected);

	// test the *=operator
	y_actual = x;
	y_actual *= z;
	check_affine_inclu(y_actual, y_expected);
}

template<class T>
void  TestAffineBase<T>::check_mul_scal(const Interval& x, double z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	check_affine_eq(xa*z, y_expected);
	check_affine_eq(z*xa, y_expected);
}

template<class T>
void  TestAffineBase<T>::check_div(const Interval& x, const Interval& z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	AffineVarMain<T> za(z);
	check_div(xa,za,y_expected);

}

template<class T>
void  TestAffineBase<T>::check_div(const AffineMain<T>& x, const AffineMain<T>& z, const Interval& y_expected) {
	AffineMain<T> y_actual=x/z;
	//cout << "check:    " << x << " / " << z << ", " << y_expected << " (expected)        " << y_actual << " (actual)"<< endl;
	check_affine_inclu(y_actual, y_expected);

	// test the /=operator
	y_actual = x;
	y_actual /= z;
	check_affine_inclu(y_actual, y_expected);
}

template<class T>
void  TestAffineBase<T>::check_div_scal(const Interval& x, double z, const Interval& y_expected) {
	AffineVarMain<T> xa(x);
	AffineMain<T> y_actual=xa/z;
	check_affine_inclu(y_actual, y_expected);

	// test the /=operator
	y_actual = xa;
	y_actual /= z;
	check_affine_inclu(y_actual, y_expected);
}


template<class T>
void  TestAffineBase<T>::minus01() { check_affine_eq(-(AffineVarMain<T>(Interval(0,1))), Interval(-1,0)); }
template<class T>
void  TestAffineBase<T>::minus02() { check_affine_eq(-(AffineVarMain<T>(Interval::ALL_REALS)), Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::minus03() { check_affine_eq(-(AffineVarMain<T>(Interval::NEG_REALS)), Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::minus04() { check_affine_eq(-(AffineVarMain<T>(Interval(NEG_INFINITY,1))), Interval(-1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::add01() { check_add(Interval::EMPTY_SET,      Interval(0,1),      Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add02() { check_add(Interval(0,1),            Interval::EMPTY_SET, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add03() {check_add(Interval(NEG_INFINITY,1), Interval(0,1),      Interval(NEG_INFINITY, 2)); }
template<class T>
void  TestAffineBase<T>::add04() {check_add(Interval(1,POS_INFINITY), Interval(0,1),      Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::add05() { check_add(Interval::ALL_REALS,      Interval(0,1),      Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::add06() { check_add_scal(Interval::EMPTY_SET,      POS_INFINITY,       Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add07() { check_add_scal(Interval::EMPTY_SET,      0,                  Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add08() { check_add_scal(Interval(0,1),            1,                  Interval(1,2)); }
template<class T>
void  TestAffineBase<T>::add09() { check_add_scal(Interval(0,1),            NEG_INFINITY,       Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add10() { check_add_scal(Interval(0,1),            POS_INFINITY,       Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add11() { check_add_scal(Interval(NEG_INFINITY,1), 1,                  Interval(NEG_INFINITY,2)); }
template<class T>
void  TestAffineBase<T>::add12() { check_add(Interval(DBL_MAX,POS_INFINITY), 1, Interval(DBL_MAX,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::add13() { check_add(Interval(DBL_MAX,POS_INFINITY), -1, Interval(ibex::previous_float(DBL_MAX),POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::add14() { check_add(Interval(DBL_MAX,POS_INFINITY), Interval(DBL_MAX,POS_INFINITY), Interval(DBL_MAX,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::add15() { check_add(Interval(DBL_MAX,POS_INFINITY), NEG_INFINITY, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add16() { check_add(Interval(DBL_MAX,POS_INFINITY), POS_INFINITY, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::add17() { check_add(Interval(NEG_INFINITY,-DBL_MAX),  1, Interval(NEG_INFINITY,ibex::next_float(-DBL_MAX))); }
template<class T>
void  TestAffineBase<T>::add18() { check_add(Interval(NEG_INFINITY,-DBL_MAX), -1, Interval(NEG_INFINITY,-DBL_MAX)); }
template<class T>
void  TestAffineBase<T>::add19() { check_add(Interval(NEG_INFINITY,-DBL_MAX),  Interval(NEG_INFINITY,-DBL_MAX), Interval(NEG_INFINITY,-DBL_MAX)); }

template<class T>
void  TestAffineBase<T>::mul01() { check_mul(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::mul02() { check_mul(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
template<class T>
void  TestAffineBase<T>::mul03() { check_mul(Interval(-1,1),              Interval::NEG_REALS, 	     Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::mul04() { check_mul(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::mul05() { check_mul(Interval(NEG_INFINITY, 1),   Interval(-1,0),              Interval(-1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::mul06() { check_mul(Interval(0, 1),              Interval(1,POS_INFINITY), 	 Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::mul07() { check_mul(Interval(0, 1),              Interval(-1,POS_INFINITY),   Interval(-1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::mul08() { check_mul(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval::NEG_REALS); }
template<class T>
void  TestAffineBase<T>::mul09() { check_mul(Interval(NEG_INFINITY, 1),   Interval(0,1),               Interval(NEG_INFINITY,1)); }
template<class T>
void  TestAffineBase<T>::mul10() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,-1),   Interval::NEG_REALS); }
template<class T>
void  TestAffineBase<T>::mul11() { check_mul(Interval(0, 1),              Interval(NEG_INFINITY,1),    Interval(NEG_INFINITY,1)); }
template<class T>
void  TestAffineBase<T>::mul12() { check_mul(Interval(1,POS_INFINITY),    Interval(0,1),               Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::mul13() { check_mul(Interval(-1,POS_INFINITY),   Interval(0,1),               Interval(-1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::mul14() { check_mul(Interval(1,2),               Interval(1,2), 	        	 Interval(1,4)); }
template<class T>
void  TestAffineBase<T>::mul15() { check_mul(Interval(1,2),               Interval(-2,3), 	         Interval(-4,6)); }
template<class T>
void  TestAffineBase<T>::mul16() { check_mul(Interval(-1,1),              Interval(-2,3), 	         Interval(-3,3)); }
template<class T>
void  TestAffineBase<T>::mul17() { check_mul_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::mul18() { check_mul_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::mul19() { check_mul_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }
template<class T>
void  TestAffineBase<T>::mulMM01() {
	double _tab[] = { 1, 2, 3, 4, 5, 6 };
	Matrix M(3,2,_tab);
	M*=M.transpose();
	double _res[] = {5, 11 , 17,
			11 , 25 , 39,
			17 , 39 , 61 };
	Matrix M_expected(3,3,_res);
	CPPUNIT_ASSERT(M==M_expected);
}
template<class T>
void  TestAffineBase<T>::div01() { check_div(Interval::EMPTY_SET,         Interval(0,1),               Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div02() { check_div(Interval::ZERO,              Interval::ZERO,              Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div03() { check_div(Interval(1,2),               Interval::ZERO,              Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div04() { check_div(Interval::ALL_REALS,         Interval::ZERO,              Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div05() { check_div(Interval::ZERO,              Interval(0,1),               Interval::ZERO); }
template<class T>
void  TestAffineBase<T>::div06() { check_div(Interval::ZERO,              Interval::ALL_REALS,         Interval::ZERO); }
template<class T>
void  TestAffineBase<T>::div07() { check_div(Interval(6,12),              Interval(2,3),               Interval(2,6)); }
template<class T>
void  TestAffineBase<T>::div08() { check_div(Interval(6,12),              Interval(-3,-2),             Interval(-6,-2)); }
template<class T>
void  TestAffineBase<T>::div09() { check_div(Interval(6,12),              Interval(-2,3),              Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::div10() { check_div(Interval(NEG_INFINITY,-1),   Interval(-1,0),              Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::div11() { check_div(Interval(NEG_INFINITY,-1),   Interval(0,1),               Interval(NEG_INFINITY,-1)); }
template<class T>
void  TestAffineBase<T>::div12() { check_div(Interval(1,POS_INFINITY),    Interval(-1,0),              Interval(NEG_INFINITY,-1)); }
template<class T>
void  TestAffineBase<T>::div13() { check_div(Interval(1,POS_INFINITY),    Interval(0,1),               Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::div14() { check_div(Interval(-1,1),              Interval(-1,1), 	         Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::div15() { check_div_scal(Interval(1,2),          NEG_INFINITY, 	        	 Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div16() { check_div_scal(Interval(1,2),          POS_INFINITY, 	        	 Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::div17() { check_div_scal(Interval(1,2),          -1, 	        	         Interval(-2,-1)); }





template<class T>
void  TestAffineBase<T>::sqrt01() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval::ALL_REALS)), Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::sqrt02() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval::NEG_REALS)), Interval::ZERO); }
template<class T>
void  TestAffineBase<T>::sqrt03() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval(-9,4))),      Interval(0,2)); }
template<class T>
void  TestAffineBase<T>::sqrt04() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval(4,9))),       Interval(2,3)); }
template<class T>
void  TestAffineBase<T>::sqrt05() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval(-9,-4))),     Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::sqrt06() { check_affine_inclu(sqrt(AffineVarMain<T>(Interval(-9,POS_INFINITY))),     Interval::POS_REALS); }

#define piL Interval::PI.lb()
#define piU Interval::PI.ub()
template<class T>
void  TestAffineBase<T>::check_sinh(const Interval& x) {
	double xl=x.lb();
	double xu=x.ub();
	double yl=xl==NEG_INFINITY? NEG_INFINITY : 0.5*(exp(xl)-exp(-xl));
	double yu=xu==POS_INFINITY? POS_INFINITY : 0.5*(exp(xu)-exp(-xu));

	//cout << AffineVarMain<T>(x)<<endl<< "  /// actual: "<<sinh(AffineVarMain<T>(x))<< "  // expected:  "   <<Interval(yl,yu)<<endl;
	//cout << -AffineVarMain<T>(x)<<endl<<  "  /// actual: "<<sinh(-AffineVarMain<T>(x))<< "  // expected:  "   <<Interval(-yu,-yl)<<endl;

	check_affine_inclu(sinh(AffineVarMain<T>(x)), Interval(yl,yu));
	check_affine_inclu(sinh(-AffineVarMain<T>(x)), Interval(-yu,-yl));
}
template<class T>
void  TestAffineBase<T>::sinh01() { check_sinh(Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::sinh02() { check_sinh(Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::sinh03() { check_sinh(Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::sinh04() { check_sinh(Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::sinh05() { check_sinh(Interval(1,1)); }
template<class T>
void  TestAffineBase<T>::sinh06() { check_sinh(Interval(2,3)); }
template<class T>
void  TestAffineBase<T>::sinh07() { check_sinh(Interval(4,5)); }

template<class T>
void  TestAffineBase<T>::check_cosh(const Interval& x) {

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
	check_affine_inclu(cosh(AffineVarMain<T>(x)), y);
	check_affine_inclu(cosh(-AffineVarMain<T>(x)), y);
//	check(x,acosh(cosh(x)));
//	check(x,acosh(cosh(-x)));
}
template<class T>
void  TestAffineBase<T>::cosh01() { check_cosh(Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::cosh02() { check_cosh(Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::cosh03() { check_cosh(Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::cosh04() { check_cosh(Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::cosh05() { check_cosh(Interval(1,1)); }
template<class T>
void  TestAffineBase<T>::cosh06() { check_cosh(Interval(2,3)); }
template<class T>
void  TestAffineBase<T>::cosh07() { check_cosh(Interval(4,5)); }

template<class T>
void  TestAffineBase<T>::check_trigo(const Interval& x, const Interval& sin_x_expected) {
	check_affine_inclu(sin(AffineVarMain<T>(x)), sin_x_expected);
	check_affine_inclu(sin(Interval::PI-AffineVarMain<T>(x)), sin_x_expected);
	check_affine_inclu(sin(AffineVarMain<T>(x)+Interval::TWO_PI), sin_x_expected);
	check_affine_inclu(sin(-AffineVarMain<T>(x)), -sin_x_expected);
	check_affine_inclu(cos(AffineVarMain<T>(x)-Interval::HALF_PI), sin_x_expected);
	check_affine_inclu(cos(Interval::HALF_PI-AffineVarMain<T>(x)), sin_x_expected);
	check_affine_inclu(cos(AffineVarMain<T>(x)+Interval::HALF_PI), -sin_x_expected);
	check_affine_inclu(cos(AffineVarMain<T>(x)+Interval::TWO_PI-Interval::HALF_PI), sin_x_expected);
}
template<class T>
void  TestAffineBase<T>::log01() { check_affine_inclu(log(AffineVarMain<T>(Interval::EMPTY_SET)), Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::log02() { check_affine_inclu(log(AffineVarMain<T>(Interval::ALL_REALS)), Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::log03() { check_affine_inclu(log(AffineVarMain<T>(Interval::POS_REALS)), Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::log04() { check_affine_inclu(log(AffineVarMain<T>(Interval::NEG_REALS)),  Interval::EMPTY_SET); /*Interval(NEG_INFINITY,-DBL_MAX));*/ }
template<class T>
void  TestAffineBase<T>::log05() { check_affine_inclu(log(AffineVarMain<T>(Interval(1,2))),       Interval(0,::log(2))); }
template<class T>
void  TestAffineBase<T>::log06() { check_affine_inclu(log(AffineVarMain<T>(Interval(-1,1))),      Interval(NEG_INFINITY,0)); }
template<class T>
void  TestAffineBase<T>::log07() { CPPUNIT_ASSERT((log(AffineVarMain<T>(Interval(0,ibex::next_float(0))))).itv().ub()> -744.5); }
template<class T>
void  TestAffineBase<T>::log08() { check_affine_inclu(log(AffineVarMain<T>(Interval(0,1))),       Interval(NEG_INFINITY,0)); }
template<class T>
void  TestAffineBase<T>::log09() { check_affine_inclu(log(AffineVarMain<T>(Interval(1,POS_INFINITY))), Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::log10() { check_affine_inclu(log(AffineVarMain<T>(Interval(0))), Interval::EMPTY_SET); /* Interval(NEG_INFINITY,-DBL_MAX)); */ }
template<class T>
void  TestAffineBase<T>::log11() { check_affine_inclu(log(AffineVarMain<T>(Interval(-2,-1))), Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::exp01() { check_affine_inclu(exp(AffineVarMain<T>(Interval::EMPTY_SET)), Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::exp02() { check_affine_inclu(exp(AffineVarMain<T>(Interval::ALL_REALS)), Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::exp03() { check_affine_inclu(exp(AffineVarMain<T>(Interval::POS_REALS)), Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::exp04() { check_affine_inclu(exp(AffineVarMain<T>(Interval::NEG_REALS)), Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::exp05() { check_affine_inclu(exp(AffineVarMain<T>(Interval(0,2))),       Interval(1,::exp(2))); }
template<class T>
void  TestAffineBase<T>::exp06() { check_affine_inclu(exp(AffineVarMain<T>(Interval(-1,1))),      Interval(::exp(-1),::exp(1))); }
template<class T>
void  TestAffineBase<T>::exp07() { check_affine_inclu(exp(AffineVarMain<T>(Interval(1.e100,1.e111))),Interval(DBL_MAX,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::exp08() { check_affine_inclu(exp(AffineVarMain<T>(Interval(DBL_MAX,POS_INFINITY))),Interval(DBL_MAX,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::exp09() { check_affine_inclu(exp(AffineVarMain<T>(Interval(0,DBL_MAX))),Interval(1,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::sin01() { check_trigo(Interval::ALL_REALS, Interval(-1,1)); }
template<class T>
void  TestAffineBase<T>::sin02() { check_trigo(Interval::EMPTY_SET, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::sin03() { check_trigo(Interval(0,piU/2.0), Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::sin04() { check_trigo(Interval(0,piU), Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::sin05() { check_trigo(Interval(0,3*piU/2.0), Interval(-1,1)); }
template<class T>
void  TestAffineBase<T>::sin06() { check_trigo(Interval(piL,3*piU/2.0), Interval(-1,0)); }
template<class T>
void  TestAffineBase<T>::sin07() { check_trigo(Interval(0.5,1.5), Interval(sin(0.5),sin(1.5))); }
template<class T>
void  TestAffineBase<T>::sin08() { check_trigo(Interval(1.5,3), Interval(sin(3.0),1)); }
template<class T>
void  TestAffineBase<T>::sin09() { check_trigo(Interval(3,4), Interval(sin(4.0),sin(3.0))); }
template<class T>
void  TestAffineBase<T>::sin10() { check_trigo(Interval(3,5), Interval(-1,sin(3.0))); }
template<class T>
void  TestAffineBase<T>::sin11() { check_trigo(Interval(3,2*piU+1.5), Interval(-1,sin(1.5))); }
template<class T>
void  TestAffineBase<T>::sin12() { check_trigo(Interval(5,2*piU+1.5), Interval(sin(5.0),sin(1.5))); }
template<class T>
void  TestAffineBase<T>::sin13() { check_trigo(Interval(5,2*piU+3), Interval(sin(5.0),1)); }
template<class T>
void  TestAffineBase<T>::tan01() { check_affine_inclu(tan(AffineVarMain<T>(Interval::ALL_REALS)), Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::tan02() { check_affine_inclu(tan(AffineVarMain<T>((-Interval::PI/4.0 | Interval::PI/4.0))), Interval(-1,1)); }
template<class T>
void  TestAffineBase<T>::tan03() {	// tan(pi/4,pi/2)=[1,+oo)
	AffineVarMain<T> x(Interval(piL/4.0,(1-1e-10)*piL/2.0)); // upper bound of x is close to pi/2
	AffineMain<T>  y=tan(x);
	CPPUNIT_ASSERT(y.itv().lb()<=1.0);
	CPPUNIT_ASSERT(y.itv().ub()>1000); // upper bound of tan(x) is close to +oo
}
template<class T>
void  TestAffineBase<T>::tan04() {	// tan(-pi/2,pi/4)=(-oo,1]
	Interval y= (tan(AffineVarMain<T>(Interval(-(1-1e-10)*piL/2.0,piL/4.0)))).itv();
	CPPUNIT_ASSERT(y.lb()<-1000); // lower bound is close to -oo
	CPPUNIT_ASSERT(y.ub()>=1.0);
}
template<class T>
void  TestAffineBase<T>::tan05() { check_affine_inclu(tan(AffineVarMain<T>(Interval::PI/2.0)),Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::tan06() { check_affine_inclu(tan(-AffineVarMain<T>(Interval::PI)),Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::tan07() { check_affine_inclu(tan(AffineVarMain<T>((3*Interval::PI/4.0 | 5*Interval::PI/4.0))), Interval(-1,1)); }

template<class T>
void  TestAffineBase<T>::check_pow(const Interval& x, int p, const Interval& y_expected) {
	check_affine_inclu(pow(AffineVarMain<T>(x),p),y_expected);
	check_affine_inclu(pow(-AffineVarMain<T>(x),p),(p%2==0)? y_expected : -y_expected);
}
template<class T>
void  TestAffineBase<T>::pow01() { check_pow(Interval::ALL_REALS, 4, Interval::POS_REALS); }
template<class T>
void  TestAffineBase<T>::pow02() { check_pow(Interval::EMPTY_SET, 4, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::pow03() { check_pow(Interval(2,3), 4, Interval(16,81)); }
template<class T>
void  TestAffineBase<T>::pow04() { check_pow(Interval(-2,3), 4, Interval(0,81)); }
template<class T>
void  TestAffineBase<T>::pow05() { check_pow(Interval(-3,2), 4, Interval(0,81)); }
template<class T>
void  TestAffineBase<T>::pow06() { check_pow(Interval(2,POS_INFINITY), 4, Interval(16,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::pow07() { check_pow(Interval::ALL_REALS, 3, Interval::ALL_REALS); }
template<class T>
void  TestAffineBase<T>::pow08() { check_pow(Interval::EMPTY_SET, 3, Interval::EMPTY_SET); }
template<class T>
void  TestAffineBase<T>::pow09() { check_pow(Interval(2,3), 3, Interval(8,27)); }
template<class T>
void  TestAffineBase<T>::pow10() { check_pow(Interval(-2,3), 3, Interval(-8,27)); }
template<class T>
void  TestAffineBase<T>::pow11() { check_pow(Interval(-3,2), 3, Interval(-27,8)); }
template<class T>
void  TestAffineBase<T>::pow12() { check_pow(Interval(2,POS_INFINITY), 3, Interval(8,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::pow13() { check_pow(Interval(-10,10), -2, Interval(1.0/100,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::root01() { check_affine_inclu(root(AffineVarMain<T>(Interval(0,1)),-1), Interval(1.0,POS_INFINITY)); }
template<class T>
void  TestAffineBase<T>::root02() {	check_affine_inclu(root(AffineVarMain<T>(Interval(-27,-8)), 3),Interval(-3,-2),1e-10); }
template<class T>
void  TestAffineBase<T>::root03() { check_affine_inclu(root(AffineVarMain<T>(Interval(-4,1)),2), Interval(0,1)); }
template<class T>
void  TestAffineBase<T>::root04() { check_affine_inclu(root(AffineVarMain<T>(Interval(-8,1)),3), Interval(-2,1)); }


template<class T>
void  TestAffineBase<T>::tan_issue248() {
	//Interval itv = Interval(-Interval::PI.lb()/2,3*Interval::PI.ub()/8);
	AffineVarMain<T> aff(Interval(-1.57079632679489678, 1.1780972450961728626));
	CPPUNIT_ASSERT(!(tan(aff).is_empty()));
}
