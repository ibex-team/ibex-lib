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

class TestAffine : public TestIbex {
public:
	TestAffine() {
		TEST_ADD(TestAffine::test01);
		TEST_ADD(TestAffine::test02);
		TEST_ADD(TestAffine::test03);
		TEST_ADD(TestAffine::test04);
		TEST_ADD(TestAffine::test05);
		TEST_ADD(TestAffine::test06);
		TEST_ADD(TestAffine::test07);
		TEST_ADD(TestAffine::test08);
		TEST_ADD(TestAffine::test09);
		TEST_ADD(TestAffine::test10);
		TEST_ADD(TestAffine::test11);
		TEST_ADD(TestAffine::test12);
		TEST_ADD(TestAffine::test13);
		TEST_ADD(TestAffine::test14);
		TEST_ADD(TestAffine::test15);
		TEST_ADD(TestAffine::test16);
		TEST_ADD(TestAffine::test17);
		TEST_ADD(TestAffine::test18);
		TEST_ADD(TestAffine::test19);
		TEST_ADD(TestAffine::test20);
		TEST_ADD(TestAffine::test21);
		TEST_ADD(TestAffine::test22);
		TEST_ADD(TestAffine::test23);
		TEST_ADD(TestAffine::test24);
		TEST_ADD(TestAffine::test25);
		TEST_ADD(TestAffine::test26);
		TEST_ADD(TestAffine::test27);
		TEST_ADD(TestAffine::test28);
		TEST_ADD(TestAffine::test29);
		TEST_ADD(TestAffine::test30);
		TEST_ADD(TestAffine::test31);
		TEST_ADD(TestAffine::test32);
		TEST_ADD(TestAffine::test33);
		TEST_ADD(TestAffine::test34);
		TEST_ADD(TestAffine::test35);
		TEST_ADD(TestAffine::test36);
		TEST_ADD(TestAffine::test37);
		TEST_ADD(TestAffine::test38);
		TEST_ADD(TestAffine::test39);
//		TEST_ADD(TestAffine::test40);
//		TEST_ADD(TestAffine::test41);
//		TEST_ADD(TestAffine::test42);
//		TEST_ADD(TestAffine::test43);
//		TEST_ADD(TestAffine::test44);
//		TEST_ADD(TestAffine::test45);
//		TEST_ADD(TestAffine::test46);
//		TEST_ADD(TestAffine::test47);
//		TEST_ADD(TestAffine::test48);
		TEST_ADD(TestAffine::test49);
		TEST_ADD(TestAffine::test50);
		TEST_ADD(TestAffine::test51);
		TEST_ADD(TestAffine::test52);
		TEST_ADD(TestAffine::test53);
		TEST_ADD(TestAffine::test54);
		TEST_ADD(TestAffine::test55);
		TEST_ADD(TestAffine::test56);
		TEST_ADD(TestAffine::test57);
		TEST_ADD(TestAffine::test58);
		TEST_ADD(TestAffine::test59);
		TEST_ADD(TestAffine::test60);
		TEST_ADD(TestAffine::test61);
		TEST_ADD(TestAffine::test62);
		TEST_ADD(TestAffine::test63);
		TEST_ADD(TestAffine::test64);
		TEST_ADD(TestAffine::test65);
		TEST_ADD(TestAffine::test66);
		TEST_ADD(TestAffine::test67);
		TEST_ADD(TestAffine::test68);
		TEST_ADD(TestAffine::test69);
		TEST_ADD(TestAffine::test70);
		TEST_ADD(TestAffine::test71);
		TEST_ADD(TestAffine::test72);
		TEST_ADD(TestAffine::test73);
		TEST_ADD(TestAffine::test74);
		TEST_ADD(TestAffine::test75);
		TEST_ADD(TestAffine::test76);
		TEST_ADD(TestAffine::test77);
		TEST_ADD(TestAffine::test78);
		TEST_ADD(TestAffine::test79);
		TEST_ADD(TestAffine::test80);
		TEST_ADD(TestAffine::test81);
		TEST_ADD(TestAffine::test82);
		TEST_ADD(TestAffine::test83);
		TEST_ADD(TestAffine::test84);
		TEST_ADD(TestAffine::test85);
		TEST_ADD(TestAffine::test86);
		TEST_ADD(TestAffine::test87);
		TEST_ADD(TestAffine::test88);
		TEST_ADD(TestAffine::test89);
		TEST_ADD(TestAffine::test90);
		TEST_ADD(TestAffine::test91);
		TEST_ADD(TestAffine::test92);
		TEST_ADD(TestAffine::test93);
		TEST_ADD(TestAffine::test94);
		TEST_ADD(TestAffine::test95);
		TEST_ADD(TestAffine::test96);
		TEST_ADD(TestAffine::test97);
		TEST_ADD(TestAffine::test98);
		TEST_ADD(TestAffine::test99);
		TEST_ADD(TestAffine::test100);
		TEST_ADD(TestAffine::test101);

	}

	typedef enum { EQUALITY, INCLUSION, INCLUSION_TIGHT, INTERSECTION } comp_t;

	bool compare_results (comp_t c, Interval r, Affine2  a) ;


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

#endif
