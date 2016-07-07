/* ============================================================================
 * D Y N I B E X - Test of the Affine forms
 * ============================================================================
 * Copyright   : ENSTA ParisTech
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Julien Alexandre dit Sandretto and Alexandre Chapoutot
 * Created     : Jul 18, 2014
 * Sponsored   : This research benefited from the support of the "Chair Complex Systems Engineering - Ecole Polytechnique, THALES, DGA, FX, DASSAULT AVIATION, DCNS Research, ENSTA ParisTech, Telecom ParisTech, Fondation ParisTech and FDO ENSTA"
 * ---------------------------------------------------------------------------- */

#include "TestAffineArith.h"
#include "ibex_AffineEval.h"

template<class T>
bool TestAffineArith<T>::compare_results (comp_t c, Interval r, AffineMain<T>  a) {
	Interval ra = a.itv();
	Interval tmp;
	switch (c) {
	case EQUALITY:
		if (ra == r) {
			return true;
		} else if (r.is_subset(ra) && ra.is_subset(r.inflate(abs(r).ub()*1.e-14)) ) {
			return true;
		} else {
			return false;
		}

	case INCLUSION:
		if (! r.is_subset(ra)) {
			return false;
		}
		else {
			return true;
		}
	case INCLUSION_TIGHT:
		if (! ra.is_subset(r)) {
			std::cout.precision(20);
			std::cout<<"Affine:  "<<a<<std::endl;
			std::cout<<"AffineI: "<<ra<<std::endl;
			std::cout<<"Interval:"<<r<<std::endl;
			return false;
		}
		else {
			return true;
		}

	case INTERSECTION:
		tmp = ra & r;
		if (tmp.is_empty()) {
			return false;
		}
		else {
			return true;
		}

	default:
		return false;
	}

}



template<class T>
void TestAffineArith<T>::test01(){
	Interval x(-1, 1);
	AffineMain<T> ax (1,1,x);
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test02() {
	Interval x(-1, 2.13);
	AffineMain<T> ax (1,1,x);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test03()  {
	Interval x(5, 7.43);
	AffineMain<T> ax (1,1,x);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test04()  {
	Interval x(-15, -7.45);
	AffineMain<T> ax (1,1,x);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

// Float addition
template<class T>
void TestAffineArith<T>::test05()  {
	Interval x(1.0, 2.0);
	AffineMain<T> ax (1,1,x);
	x = x + 1.0;
	ax = ax + 1.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test06()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x + 0.0;
	ax = ax + 0.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test07()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x + 3.14;
	ax = ax + 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test08()  {
	Interval x(-5, -2);
	AffineMain<T> ax (1,1,x);
	x = x + 3.14;
	ax = ax + 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test09()  {
	Interval x(-5, 4);
	AffineMain<T> ax (1,1,x);
	x = x + 3.14;
	ax = ax + 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

// Float subtraction

template<class T>
void TestAffineArith<T>::test10()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x - 1.0;
	ax = ax - 1.0;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test11()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x - 0.0;
	ax = ax - 0.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test12()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x - 3.14;
	ax = ax - 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test13() {
	Interval x(-5, -2);
	AffineMain<T> ax (1,1,x);
	x = x - 3.14;
	ax = ax - 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test14() {
	Interval x(-5, 4);
	AffineMain<T> ax (1,1,x);
	x = x - 3.14;
	ax = ax - 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


// Scalar multiplication

template<class T>
void TestAffineArith<T>::test15()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x * 1.0;
	ax = ax * 1.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test16()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x * 0.0;
	ax = ax * 0.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}


template<class T>
void TestAffineArith<T>::test17()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x * 3.14;
	ax = ax * 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test18()   {
	Interval x(-5, -2);
	AffineMain<T> ax (1,1,x);
	x = x * 3.14;
	ax = ax * 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test19()   {
	Interval x(-5, 4);
	AffineMain<T> ax (1,1,x);
	x = x * 3.14;
	ax = ax * 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

// Float division

template<class T>
void TestAffineArith<T>::test20()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x / 1.0;
	ax = ax / 1.0;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test21()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	x = x / 3.14;
	ax = ax / 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test22()   {
	Interval x(-5, -2);
	AffineMain<T> ax (1,1,x);
	x = x / 3.14;
	ax = ax / 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test23()  {
	Interval x(-5, 4);
	AffineMain<T> ax (1,1,x);
	x = x / 3.14;
	ax = ax / 3.14;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

/* ****************************** */

// Interval addition

template<class T>
void TestAffineArith<T>::test24()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(1.0, 1.0);
	x = x + prod;
	ax = ax + prod;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test25()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(0.0, 0.0);
	x = x + prod;
	ax = ax + prod;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test26()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.0, 1.0);
	x = x + prod;
	ax = ax + prod;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test27()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.7, 1.7);
	x = x + prod;
	ax = ax + prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test28()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.08, 1.7);
	x = x + prod;
	ax = ax + prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


// Interval multiplication

template<class T>
void TestAffineArith<T>::test29()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(1.0, 1.0);
	x = x * prod;
	ax = ax * prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test30()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(0.0, 0.0);
	x = x * prod;
	ax = ax * prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test31()  {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.0, 1.0);
	x = x * prod;
	ax = ax * prod;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test32()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.7, 1.7);
	x = x * prod;
	ax = ax * prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test33()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	Interval prod(-1.08, 1.7);
	x = x * prod;
	ax = ax * prod;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


/* ********************************************** */
template<class T>
void TestAffineArith<T>::test34()   {
	Interval x(1, 2);
	AffineMain<T> ax (1,1,x);
	AffineMain<T> ay = ax;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}


template<class T>
void TestAffineArith<T>::test35()   {
	Interval x(-11.3, -4.3);
	AffineMain<T> ax (1,1,x);
	AffineMain<T> ay = ax;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test36()   {
	Interval x(-11.3, 4.3);
	AffineMain<T> ax (1,1,x);
	AffineMain<T> ay = ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test37()   {
	Interval x(0, 4.3);
	AffineMain<T> ax (1,1,x);
	AffineMain<T> ay = ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

/* ********************************************** */

// Assign-addition with interval
template<class T>
void TestAffineArith<T>::test38()   {
	Interval x(1.57, 2.23);
	AffineMain<T> ax (1,1,x);
	ax += x;
	x += x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


template<class T>
void TestAffineArith<T>::test39()  {
	Interval x(-11.3, -4.3);
	AffineMain<T> ax (1,1,x);
	ax += x;
	x += x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test49()  {
	Interval x(-11.3, 4.3);
	AffineMain<T> ax (1,1,x);
	ax += x;
	x += x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test50()  {
	Interval x(0, 4.3);
	AffineMain<T> ax (1,1,x);
	ax += x;
	x += x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

// Assign-multiplication with interval
template<class T>
void TestAffineArith<T>::test51()   {
	Interval x(1.57, 2.23);
	AffineMain<T> ax (1,1,x);
	ax *= x;
	x *= x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


template<class T>
void TestAffineArith<T>::test52()  {
	Interval x(-11.3, -4.3);
	AffineMain<T> ax (1,1,x);
	ax *= x;
	x *= x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test53()  {
	Interval x(-11.3, 4.3);
	AffineMain<T> ax (1,1,x);
	ax *= x;
	x *= x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test54()  {
	Interval x(0, 4.3);
	AffineMain<T> ax (1,1,x);
	ax *= x;
	x *= x;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

/* ********************************************* */


// Assign-addition with float
template<class T>
void TestAffineArith<T>::test55()  {
	Interval x(1,2);
	AffineMain<T> ax (1,1,x);
	ax += 3.14159;
	x += 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


template<class T>
void TestAffineArith<T>::test56()  {
	Interval x(-11.3, -4.3);
	AffineMain<T> ax (1,1,x);
	ax += 3.14159;
	x += 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test57()   {
	Interval x(-11.3, 4.3);
	AffineMain<T> ax (1,1,x);
	ax += 3.14159;
	x += 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test58()  {
	Interval x(0, 4.3);
	AffineMain<T> ax (1,1,x);
	ax += 3.14159;
	x += 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test59()  {
	Interval x(0, 4.31);
	AffineMain<T> ax (1,1,x);
	ax += 0.;
	x += 0.;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

// Assign-multiplication with float
template<class T>
void TestAffineArith<T>::test60()  {
	Interval x(1.57, 2.23);
	AffineMain<T> ax (1,1,x);
	ax *= 1.;
	x *= 1.;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test61()  {
	Interval x(1.57, 2.23);
	AffineMain<T> ax (1,1,x);
	ax *= 0.;
	x *= 0.;
	CPPUNIT_ASSERT(compare_results (EQUALITY, x, ax));
}

template<class T>
void TestAffineArith<T>::test62()  {
	Interval x(1.57, 2.23);
	AffineMain<T> ax (1,1,x);
	ax *= 3.14159;
	x *= 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


template<class T>
void TestAffineArith<T>::test63()  {
	Interval x(-11.3, -4.3);
	AffineMain<T> ax (1,1,x);
	ax *= 3.14159;
	x *= 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test64()   {
	Interval x(-11.3, 4.3);
	AffineMain<T> ax (1,1,x);
	ax *= 3.14159;
	x *= 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

template<class T>
void TestAffineArith<T>::test65()   {
	Interval x(0, 4.3);
	AffineMain<T> ax (1,1,x);
	ax *= 3.14159;
	x *= 3.14159;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

/* ****************************************** */

// Addition
template<class T>
void TestAffineArith<T>::test66() {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x+y, ax1+ax2));
}

template<class T>
void TestAffineArith<T>::test67()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x+y, ax1+ax2));
}

template<class T>
void TestAffineArith<T>::test68()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x+y, ax1+ax2));
}

template<class T>
void TestAffineArith<T>::test69()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	compare_results (INCLUSION, x+y, ax1+ax2);
}

// Subtraction

template<class T>
void TestAffineArith<T>::test70()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x-y, ax1-ax2));
}

template<class T>
void TestAffineArith<T>::test71()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	compare_results (INCLUSION, x-y, ax1-ax2);
}

template<class T>
void TestAffineArith<T>::test72()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	compare_results (INCLUSION, x-y, ax1-ax2);
}

template<class T>
void TestAffineArith<T>::test73()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x-y,  ax1-ax2));;
}

// Multiplication


template<class T>
void TestAffineArith<T>::test74()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x*y,  ax1*ax2));;
}

template<class T>
void TestAffineArith<T>::test75()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x*y,  ax1*ax2));;
}

template<class T>
void TestAffineArith<T>::test76()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x*y,  ax1*ax2));;
}

template<class T>
void TestAffineArith<T>::test77()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x*y,  ax1*ax2));;
}

// Division

template<class T>
void TestAffineArith<T>::test78()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x/y,  ax1/ax2));;
}

template<class T>
void TestAffineArith<T>::test79()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x/y,  ax1/ax2));;
}

template<class T>
void TestAffineArith<T>::test80()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x/y,  ax1/ax2));;
}

template<class T>
void TestAffineArith<T>::test81()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	CPPUNIT_ASSERT(compare_results (INCLUSION, x/y,  ax1/ax2));;
}

/* ********************************************** */

// Assign-addition with affine forms
template<class T>
void TestAffineArith<T>::test82()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 += ax2;
	x += y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test83()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	ax1 += ax2;
	x += y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test84()  {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 += ax2;
	x += y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test85()   {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 += ax2;
	x += y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

// Assign-multiplication with affine forms
template<class T>
void TestAffineArith<T>::test86() {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 *= ax2;
	x *= y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test87() {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-7.2,-3.77);
	AffineMain<T> ax2(2,2,y);
	ax1 *= ax2;
	x *= y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test88() {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(-3.77,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 *= ax2;
	x *= y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

template<class T>
void TestAffineArith<T>::test89() {
	Interval x(1.2,4.5);
	AffineMain<T> ax1(2,1,x);
	Interval y(0,7.2);
	AffineMain<T> ax2(2,2,y);
	ax1 *= ax2;
	x *= y;
	CPPUNIT_ASSERT(compare_results (INCLUSION,  x,  ax1));;
}

/* **************************** */
template<class T>
void TestAffineArith<T>::test90() {
	Interval x(92.4,1909.3);
	AffineMain<T> ax1(2,1,x);
	Interval y(92.4,1909.3);
	AffineMain<T> ax2(2,2,y);
	Interval res = x - y;
	AffineMain<T> resa = ax1 - ax2;
	CPPUNIT_ASSERT(compare_results (INCLUSION, res,  resa));;
}


template<class T>
void TestAffineArith<T>::test91() {
	Interval x(92.4,1909.3);
	AffineMain<T> ax (1,1,x);
	Interval res = x - x;
	AffineMain<T> resa = ax - ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, res,  resa));;
}

template<class T>
void TestAffineArith<T>::test92() {
	Interval x(-92.4,-10.3);
	AffineMain<T> ax (1,1,x);
	Interval res = x - x;
	AffineMain<T> resa = ax - ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, res,  resa));;
}

template<class T>
void TestAffineArith<T>::test93() {
	Interval x(-92.4, 10.3);
	AffineMain<T> ax (1,1,x);
	Interval res = x - x;
	AffineMain<T> resa = ax - ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, res,  resa));;
}

template<class T>
void TestAffineArith<T>::test94() {
	Interval x(0.0, 10.3);
	AffineMain<T> ax (1,1,x);
	Interval res = x - x;
	AffineMain<T> resa = ax - ax;
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, res,  resa));;
}

/* **************************** */
template<class T>
void TestAffineArith<T>::test95() {

	int iter = 1000;
	Interval x(1.3,78.4);
	AffineMain<T> ax (1,1,x);
	for (int i=0; i < iter; i++)
	{
		//cout << "ax : "<< ax << endl;
		ax += -0.5*ax;//+0.01;
		x += -0.5*x;//+0.01;
	}
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, x, ax));

}

template<class T>
void TestAffineArith<T>::test96()  {
	int iter = 1000;
	Interval x(-78.4,-1.3);
	AffineMain<T> ax (1,1,x);
	for (int i=0;i < iter;i++)
	{
		//cout << "ax : "<< ax << endl;
		ax +=-0.5*ax;//+0.01;
		x +=-0.5*x;//+0.01;
	}
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, x, ax));
}

template<class T>
void TestAffineArith<T>::test97() {
	int iter = 1000;
	Interval x(-1.3,78.4);
	AffineMain<T> ax (1,1,x);
	for (int i=0;i < iter; i++)
	{
		//cout << "ax : "<< ax << endl;
		ax +=-0.5*ax;//+0.01;
		x +=-0.5*x;//+0.01;
	}
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, x, ax));
}

template<class T>
void TestAffineArith<T>::test98()   {
	int iter = 1000;
	Interval x(0.,78.4);
	AffineMain<T> ax (1,1,x);
	for (int i=0;i < iter;i++)
	{
		//cout << "ax : "<< ax << endl;
		ax += -0.5 * ax;// + 0.01;
		x += -0.5 * x;// + 0.01;
	}
	//    cout << ax << endl;
	//    cout << ax.itv() << endl;
	CPPUNIT_ASSERT(compare_results (INCLUSION_TIGHT, x, ax));
}

/* ********************* */
//test of sqrt
template<class T>
void TestAffineArith<T>::test99()   {
	Interval x(10,11);
	AffineMain<T> ax (1,1,x);
	for (int i=0;i < 1;i++)
	{
		ax = sqrt(ax);
		x = sqrt(x);
	}
	// cout << x << endl;
	// cout << ax.itv() << endl;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}

/* ********************* */
//test of 1/x
template<class T>
void TestAffineArith<T>::test100()   {
	Interval x(3,21);
	AffineMain<T> ax (1,1,x);
	for (int i=0;i < 1;i++)
	{
		ax = 1/ax;
		x = 1/x;
	}
	//  cout << x << endl;
	//  cout << ax.itv() << endl;
	CPPUNIT_ASSERT(compare_results (INCLUSION, x, ax));
}


/* ********************* */
template<class T>
void TestAffineArith<T>::test101()   {
	Variable yd(3);
	Interval sigma(10.0);
	Interval rho(28.0);
	Interval beta = Interval(8.0)/3.0;

	Function ydot = Function(yd,Return(sigma*(yd[1]-yd[0]),
			yd[0]*(rho-yd[2])-yd[1],
			yd[0]*yd[1]-beta*yd[2]));

	IntervalVector v(3);
	v[0] = Interval(-1, 1);
	v[1] = Interval(2, 3);
	v[2] = Interval(-4,-4);
	AffineMainVector<T> va(v);

	IntervalVector res = ydot.eval_vector(v);
	AffineEval<T> eval_af2(ydot);
	AffineMainVector<T> resa = eval_af2.eval(va).v();

	for (int j = 0; j < 3; ++j) {
		CPPUNIT_ASSERT(compare_results (INCLUSION, res[j], resa[j]));
	}
}



