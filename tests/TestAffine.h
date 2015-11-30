/* ============================================================================
 * I B E X - Affine2 Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#ifndef __TEST_AFFINE_H__
#define __TEST_AFFINE_H__

#include "cpptest.h"
#include "ibex_Interval.h"
#include "ibex_Affine2.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "ibex_Function.h"

using namespace ibex;

template<class T>
class TestAffine : public TestIbex {
public:
	TestAffine() {
		TEST_ADD(TestAffine<T>::test01);
		TEST_ADD(TestAffine<T>::test02);
		TEST_ADD(TestAffine<T>::test03);
		TEST_ADD(TestAffine<T>::test04);
		TEST_ADD(TestAffine<T>::test05);
		TEST_ADD(TestAffine<T>::test06);
		TEST_ADD(TestAffine<T>::test07);
		TEST_ADD(TestAffine<T>::test08);
		TEST_ADD(TestAffine<T>::test09);
		TEST_ADD(TestAffine<T>::test10);
		TEST_ADD(TestAffine<T>::test11);
		TEST_ADD(TestAffine<T>::test12);
		TEST_ADD(TestAffine<T>::test13);
		TEST_ADD(TestAffine<T>::test14);
		TEST_ADD(TestAffine<T>::test15);
		TEST_ADD(TestAffine<T>::test16);
		TEST_ADD(TestAffine<T>::test17);
		TEST_ADD(TestAffine<T>::test18);
		TEST_ADD(TestAffine<T>::test19);
		TEST_ADD(TestAffine<T>::test20);
		TEST_ADD(TestAffine<T>::test21);
		TEST_ADD(TestAffine<T>::test22);
		TEST_ADD(TestAffine<T>::test23);
		TEST_ADD(TestAffine<T>::test24);
		TEST_ADD(TestAffine<T>::test25);
		TEST_ADD(TestAffine<T>::test26);
		TEST_ADD(TestAffine<T>::test27);
		TEST_ADD(TestAffine<T>::test28);
		TEST_ADD(TestAffine<T>::test29);
		TEST_ADD(TestAffine<T>::test30);
		TEST_ADD(TestAffine<T>::test31);
		TEST_ADD(TestAffine<T>::test32);
		TEST_ADD(TestAffine<T>::test33);
		TEST_ADD(TestAffine<T>::test34);
		TEST_ADD(TestAffine<T>::test35);
		TEST_ADD(TestAffine<T>::test36);
		TEST_ADD(TestAffine<T>::test37);
		TEST_ADD(TestAffine<T>::test38);
		TEST_ADD(TestAffine<T>::test39);
//		TEST_ADD(TestAffine<T>::test40);
//		TEST_ADD(TestAffine<T>::test41);
//		TEST_ADD(TestAffine<T>::test42);
//		TEST_ADD(TestAffine<T>::test43);
//		TEST_ADD(TestAffine<T>::test44);
//		TEST_ADD(TestAffine<T>::test45);
//		TEST_ADD(TestAffine<T>::test46);
//		TEST_ADD(TestAffine<T>::test47);
//		TEST_ADD(TestAffine<T>::test48);
		TEST_ADD(TestAffine<T>::test49);
		TEST_ADD(TestAffine<T>::test50);
		TEST_ADD(TestAffine<T>::test51);
		TEST_ADD(TestAffine<T>::test52);
		TEST_ADD(TestAffine<T>::test53);
		TEST_ADD(TestAffine<T>::test54);
		TEST_ADD(TestAffine<T>::test55);
		TEST_ADD(TestAffine<T>::test56);
		TEST_ADD(TestAffine<T>::test57);
		TEST_ADD(TestAffine<T>::test58);
		TEST_ADD(TestAffine<T>::test59);
		TEST_ADD(TestAffine<T>::test60);
		TEST_ADD(TestAffine<T>::test61);
		TEST_ADD(TestAffine<T>::test62);
		TEST_ADD(TestAffine<T>::test63);
		TEST_ADD(TestAffine<T>::test64);
		TEST_ADD(TestAffine<T>::test65);
		TEST_ADD(TestAffine<T>::test66);
		TEST_ADD(TestAffine<T>::test67);
		TEST_ADD(TestAffine<T>::test68);
		TEST_ADD(TestAffine<T>::test69);
		TEST_ADD(TestAffine<T>::test70);
		TEST_ADD(TestAffine<T>::test71);
		TEST_ADD(TestAffine<T>::test72);
		TEST_ADD(TestAffine<T>::test73);
		TEST_ADD(TestAffine<T>::test74);
		TEST_ADD(TestAffine<T>::test75);
		TEST_ADD(TestAffine<T>::test76);
		TEST_ADD(TestAffine<T>::test77);
		TEST_ADD(TestAffine<T>::test78);
		TEST_ADD(TestAffine<T>::test79);
		TEST_ADD(TestAffine<T>::test80);
		TEST_ADD(TestAffine<T>::test81);
		TEST_ADD(TestAffine<T>::test82);
		TEST_ADD(TestAffine<T>::test83);
		TEST_ADD(TestAffine<T>::test84);
		TEST_ADD(TestAffine<T>::test85);
		TEST_ADD(TestAffine<T>::test86);
		TEST_ADD(TestAffine<T>::test87);
		TEST_ADD(TestAffine<T>::test88);
		TEST_ADD(TestAffine<T>::test89);
		TEST_ADD(TestAffine<T>::test90);
		TEST_ADD(TestAffine<T>::test91);
		TEST_ADD(TestAffine<T>::test92);
		TEST_ADD(TestAffine<T>::test93);
		TEST_ADD(TestAffine<T>::test94);
		TEST_ADD(TestAffine<T>::test95);
		TEST_ADD(TestAffine<T>::test96);
		TEST_ADD(TestAffine<T>::test97);
		TEST_ADD(TestAffine<T>::test98);
		TEST_ADD(TestAffine<T>::test99);
		TEST_ADD(TestAffine<T>::test100);
		TEST_ADD(TestAffine<T>::test101);

	}

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


#include "TestAffine.h_"

#endif
