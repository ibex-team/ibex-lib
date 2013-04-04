/*
 * TestAffine2.h
 *
 *  Created on: 6 mars 2013
 *      Author: nininjo
 */


#ifndef __TEST_AFFINE2_H__
#define __TEST_AFFINE2_H__

#include "cpptest.h"
#include "ibex_Affine2.h"
#include "utils.h"

using namespace ibex;

class TestAffine2 : public TestIbex {
public:
	TestAffine2() {
		TEST_ADD(TestAffine2::test_sqrt);
		TEST_ADD(TestAffine2::test_exp);
		TEST_ADD(TestAffine2::test_log);
		TEST_ADD(TestAffine2::test_inv);
		TEST_ADD(TestAffine2::test_cos);
		TEST_ADD(TestAffine2::test_sin);
		TEST_ADD(TestAffine2::test_tan);
		TEST_ADD(TestAffine2::test_abs);
		TEST_ADD(TestAffine2::test_acos);
		TEST_ADD(TestAffine2::test_asin);
		TEST_ADD(TestAffine2::test_atan);
		TEST_ADD(TestAffine2::test_cosh);
		TEST_ADD(TestAffine2::test_sinh);
		TEST_ADD(TestAffine2::test_tanh);



	}

	bool check_af2 (Function& f, IntervalVector& I);
	bool check_af2 (Function& f, Interval& I);

//	void test_root2();
//	void test_root4();
//	void test_root5();
	void test_pow2();
	void test_pow4();
	void test_pow5();
	void test_powINT1();
	void test_powINT2();


	void test_sqrt();
	void test_exp();
	void test_log();
	void test_inv();
	void test_cos();
	void test_sin();
	void test_tan();
	void test_abs();
	void test_acos();
	void test_asin();
	void test_atan();
	void test_cosh();
	void test_sinh();
	void test_tanh();


	void test01();
	void test02();

};


#endif
