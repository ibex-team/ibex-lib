/* ============================================================================
 * I B E X - Affine Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#ifndef __TEST_AFFINE_ARITH_H__
#define __TEST_AFFINE_ARITH_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "ibex_Affine.h"
#include "ibex_AffineEval.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "ibex_Function.h"

namespace ibex {

template<class T>
class TestAffineArith : public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE(TestAffineArith<T>);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST(test05);
	CPPUNIT_TEST(test06);
	CPPUNIT_TEST(test07);
	CPPUNIT_TEST(test08);
	CPPUNIT_TEST(test09);
	CPPUNIT_TEST(test10);
	CPPUNIT_TEST(test11);
	CPPUNIT_TEST(test12);
	CPPUNIT_TEST(test13);
	CPPUNIT_TEST(test14);
	CPPUNIT_TEST(test15);
	CPPUNIT_TEST(test16);
	CPPUNIT_TEST(test17);
	CPPUNIT_TEST(test18);
	CPPUNIT_TEST(test19);
	CPPUNIT_TEST(test20);
	CPPUNIT_TEST(test21);
	CPPUNIT_TEST(test22);
	CPPUNIT_TEST(test23);
	CPPUNIT_TEST(test24);
	CPPUNIT_TEST(test25);
	CPPUNIT_TEST(test26);
	CPPUNIT_TEST(test27);
	CPPUNIT_TEST(test28);
	CPPUNIT_TEST(test29);
	CPPUNIT_TEST(test30);
	CPPUNIT_TEST(test31);
	CPPUNIT_TEST(test32);
	CPPUNIT_TEST(test33);
	CPPUNIT_TEST(test34);
	CPPUNIT_TEST(test35);
	CPPUNIT_TEST(test36);
	CPPUNIT_TEST(test37);
	CPPUNIT_TEST(test38);
	CPPUNIT_TEST(test39);
	//		CPPUNIT_TEST(test40);
	//		CPPUNIT_TEST(test41);
	//		CPPUNIT_TEST(test42);
	//		CPPUNIT_TEST(test43);
	//		CPPUNIT_TEST(test44);
	//		CPPUNIT_TEST(test45);
	//		CPPUNIT_TEST(test46);
	//		CPPUNIT_TEST(test47);
	//		CPPUNIT_TEST(test48);
	CPPUNIT_TEST(test49);
	CPPUNIT_TEST(test50);
	CPPUNIT_TEST(test51);
	CPPUNIT_TEST(test52);
	CPPUNIT_TEST(test53);
	CPPUNIT_TEST(test54);
	CPPUNIT_TEST(test55);
	CPPUNIT_TEST(test56);
	CPPUNIT_TEST(test57);
	CPPUNIT_TEST(test58);
	CPPUNIT_TEST(test59);
	CPPUNIT_TEST(test60);
	CPPUNIT_TEST(test61);
	CPPUNIT_TEST(test62);
	CPPUNIT_TEST(test63);
	CPPUNIT_TEST(test64);
	CPPUNIT_TEST(test65);
	CPPUNIT_TEST(test66);
	CPPUNIT_TEST(test67);
	CPPUNIT_TEST(test68);
	CPPUNIT_TEST(test69);
	CPPUNIT_TEST(test70);
	CPPUNIT_TEST(test71);
	CPPUNIT_TEST(test72);
	CPPUNIT_TEST(test73);
	CPPUNIT_TEST(test74);
	CPPUNIT_TEST(test75);
	CPPUNIT_TEST(test76);
	CPPUNIT_TEST(test77);
	CPPUNIT_TEST(test78);
	CPPUNIT_TEST(test79);
	CPPUNIT_TEST(test80);
	CPPUNIT_TEST(test81);
	CPPUNIT_TEST(test82);
	CPPUNIT_TEST(test83);
	CPPUNIT_TEST(test84);
	CPPUNIT_TEST(test85);
	CPPUNIT_TEST(test86);
	CPPUNIT_TEST(test87);
	CPPUNIT_TEST(test88);
	CPPUNIT_TEST(test89);
	CPPUNIT_TEST(test90);
	CPPUNIT_TEST(test91);
	CPPUNIT_TEST(test92);
	CPPUNIT_TEST(test93);
	CPPUNIT_TEST(test94);
	CPPUNIT_TEST(test95);
	CPPUNIT_TEST(test96);
	CPPUNIT_TEST(test97);
	CPPUNIT_TEST(test98);
	CPPUNIT_TEST(test99);
	CPPUNIT_TEST(test100);
	CPPUNIT_TEST(test101);
	CPPUNIT_TEST_SUITE_END();

	typedef enum { EQUALITY, INCLUSION, INCLUSION_TIGHT, INTERSECTION } comp_t;

	bool compare_results (comp_t c, Interval r, AffineMain<T>  a) ;


	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
	void test07();
	void test08();
	void test09();
	void test10();
	void test11();
	void test12();
	void test13();
	void test14();
	void test15();
	void test16();
	void test17();
	void test18();
	void test19();
	void test20();
	void test21();
	void test22();
	void test23();
	void test24();
	void test25();
	void test26();
	void test27();
	void test28();
	void test29();
	void test30();
	void test31();
	void test32();
	void test33();
	void test34();
	void test35();
	void test36();
	void test37();
	void test38();
	void test39();
//	void test40();
//	void test41();
//	void test42();
//	void test43();
//	void test44();
//	void test45();
//	void test46();
//	void test47();
//	void test48();
	void test49();
	void test50();
	void test51();
	void test52();
	void test53();
	void test54();
	void test55();
	void test56();
	void test57();
	void test58();
	void test59();
	void test60();
	void test61();
	void test62();
	void test63();
	void test64();
	void test65();
	void test66();
	void test67();
	void test68();
	void test69();
	void test70();
	void test71();
	void test72();
	void test73();
	void test74();
	void test75();
	void test76();
	void test77();
	void test78();
	void test79();
	void test80();
	void test81();
	void test82();
	void test83();
	void test84();
	void test85();
	void test86();
	void test87();
	void test88();
	void test89();
	void test90();
	void test91();
	void test92();
	void test93();
	void test94();
	void test95();
	void test96();
	void test97();
	void test98();
	void test99();
	void test100();
	void test101();
};




////////////////////
// IMPLEMENTATION
////////////////////


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
	AffineEval<T> eval_af(ydot);
	AffineMainVector<T> resa = eval_af.eval(va).v();

	for (int j = 0; j < 3; ++j) {
		CPPUNIT_ASSERT(compare_results (INCLUSION, res[j], resa[j]));
	}
}




} // end namespace ibex

#endif
