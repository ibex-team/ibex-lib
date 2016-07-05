/* ============================================================================
 * I B E X - Affine2 Test
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
#include "utils.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "ibex_Function.h"

using namespace ibex;

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

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_Default>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_Other>);

#ifdef _IBEX_WITH_AFFINE_EXTENDED_

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_sAF>);

#endif
//#include "TestAffineArith.cpp"

#endif
