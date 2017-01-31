/* ============================================================================
 * I B E X - Affine2 Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#ifndef __TEST_AFFINE_EVAL_H__
#define __TEST_AFFINE_EVAL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Affine.h"
#include "ibex_AffineEval.h"
#include "utils.h"

namespace ibex  {

template<class T>
class TestAffineEval : public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE(TestAffineEval<T>);

	CPPUNIT_TEST(test_tan_issue);

	CPPUNIT_TEST(test_sqrt);
	CPPUNIT_TEST(test_exp);
	CPPUNIT_TEST(test_log);
	CPPUNIT_TEST(test_inv);
	CPPUNIT_TEST(test_cos);
	CPPUNIT_TEST(test_sin);
	CPPUNIT_TEST(test_tan);
	CPPUNIT_TEST(test_abs);
	CPPUNIT_TEST(test_acos);
	CPPUNIT_TEST(test_asin);
	CPPUNIT_TEST(test_atan);
	CPPUNIT_TEST(test_cosh);
	CPPUNIT_TEST(test_sinh);
	CPPUNIT_TEST(test_tanh);


	CPPUNIT_TEST(dist01);
	CPPUNIT_TEST(apply01);
	CPPUNIT_TEST(apply02);
	CPPUNIT_TEST(apply03);
	CPPUNIT_TEST(apply04);
	CPPUNIT_TEST(issue242);



	CPPUNIT_TEST_SUITE_END();


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

	void dist01();

	void apply01();
	void apply02();
	void apply03();
	void apply04();

	void issue242();
	void test_tan_issue();


};


//////////////////////////////



template<class T>
void TestAffineEval<T>::test01() {
	Variable x(2);
	Function f(x,x[0]*pow(x[1],2)+exp(x[1]*x[0]));
	IntervalVector itv(2,Interval(1,2));
	CPPUNIT_ASSERT(check_af2(f,itv));

}

template<class T>
void TestAffineEval<T>::test02() {
	Variable x;
	Function f(x,cosh(x)-x);
	IntervalVector itv(1,Interval(1,2));
	CPPUNIT_ASSERT(check_af2(f,itv));

}

template<class T>
void TestAffineEval<T>::test_pow2() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,2));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_pow4() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,4));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}



template<class T>
void TestAffineEval<T>::test_pow5() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,5));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

/*
template<class T>
void TestAffine2<T>::test_root2() {
	Variable x;
	Interval itv;
	Function f(x,root(x,2));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffine2<T>::test_root4() {
	Variable x;
	Interval itv;
	Function f(x,root(x,4));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}



template<class T>
void TestAffine2<T>::test_root5() {
	Variable x;
	Interval itv;
	Function f(x,root(x,5));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

*/

template<class T>
void TestAffineEval<T>::test_powINT1() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,Interval(2,3)));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_powINT2() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,Interval(-2,3)));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}


template<class T>
void TestAffineEval<T>::test_sqrt() {
	Variable x;
	Interval itv;
	Function f(x,sqrt(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_exp() {
	Variable x;
	Interval itv;
	Function f(x,exp(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-10,50000000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-50000000,1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1000000000.,1000000000.001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_log() {
	Variable x;
	Interval itv;
	Function f(x,log(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1000000000.,1000000000.001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10,200000000);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_inv() {
	Variable x;
	Interval itv;
	Function f(x,1.0/x);
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10,20);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-20000000,-1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1.e-15,1);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_cos() {
	Variable x;
	Interval itv;
	Function f(x,cos(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.0000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));


	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}

}

template<class T>
void TestAffineEval<T>::test_sin() {
	Variable x;
	Interval itv;
	Function f(x,sin(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));


	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}


}


template<class T>
void TestAffineEval<T>::test_tan_issue() {
	Variable x;
	Interval itv;
	Function f(x,tan(x));
	itv =Interval(-Interval::PI.lb()/2,3*Interval::PI.ub()/8);
	CPPUNIT_ASSERT(check_af2(f,itv));
}

template<class T>
void TestAffineEval<T>::test_tan() {
	Variable x;
	Interval itv;
	Function f(x,tan(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));

	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}

}

template<class T>
void TestAffineEval<T>::test_abs() {
	Variable x;
	Interval itv;
	Function f(x,abs(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}

template<class T>
void TestAffineEval<T>::test_acos() {
	Variable x;
	Interval itv;
	Function f(x,acos(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-10,20);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-10,0.2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(0.5,0.7);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}

}

template<class T>
void TestAffineEval<T>::test_asin() {
	Variable x;
	Interval itv;
	Function f(x,asin(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-10,20);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-10,0.2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(0.5,0.7);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}

}

template<class T>
void TestAffineEval<T>::test_atan() {
	Variable x;
	Interval itv;
	Function f(x,atan(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	CPPUNIT_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		CPPUNIT_ASSERT(check_af2(f,itv));
	}

}

template<class T>
void TestAffineEval<T>::test_cosh() {
	Variable x;
	Interval itv;
	Function f(x,cosh(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}

template<class T>
void TestAffineEval<T>::test_sinh() {
	Variable x;
	Interval itv;
	Function f(x,sinh(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}



template<class T>
void TestAffineEval<T>::test_tanh() {
	Variable x;
	Interval itv;
	Function f(x,tanh(x));
	itv =Interval(1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	CPPUNIT_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}



template<class T>
bool TestAffineEval<T>::check_af2 (Function& f, IntervalVector& I){
	return check_af2(f,I[0]);
}

template<class T>
bool TestAffineEval<T>::check_af2 (Function& f, Interval& I){
	double n = 100; // number of try
	AffineMain<T> faa;
	Interval itv2;
	Interval itv;
	double epsilon = 1.e-10;
	AffineEval<T> eval_af(f);

	IntervalVector x(1);

	if (!((I.is_unbounded())||(I.is_degenerated())||(!I.is_bisectable())||(I.is_empty()))){
		for (double ii= I.lb(); ii<I.ub(); ii += I.diam()/n) {
			x[0] = Interval(ii,ii+epsilon);
			itv2 =f.eval(x);

			faa = eval_af.eval(AffineMainVector<T>(x)).i();

			if (!(itv2.is_subset(faa.itv())))
			{
//				std::cout  << " DEP = "<< ii<< "  "  << f<< std::endl;
				//std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
//				std::cout  << " RES = "<< itv2 << " ///// " << faa << std::endl;

				return false;
			}
		}
	}


	x[0] = I;
	itv2 =f.eval(x);

	faa = eval_af.eval(AffineMainVector<T>(x)).i();
	Interval itv3= eval_af.d.top->i();
	itv = eval_af.eval(x).i();

	if (!(itv2.is_subset(faa.itv())))
	{
/*		std::cout  << " DEP = "<< I<< "  "  << f<< std::endl;
//		std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
		std::cout  << " RES = eval  "<< itv2 << " ///// " << itv3<< std::endl;
		std::cout  << " RES = evalaffine  "<< itv << " ///// " << faa << std::endl;
*/
		return false;
	}
/*	if (faa.size()>0) {
		Variable x;
		Function lininf(x,(faa.val(0)+faa.val(1)*(2* (x)-(I.lb()+I.ub()))/(I.diam())));

		Function linsup(x, lininf(x)+faa.err().lb());

		Function c_inf(x,lininf(x)+faa.err().lb()  -f(x));
		Function c_sup(x, f(x) -linsup(x));

		CtcFwdBwd ct1(c_inf,GT,AFFINE2_MODE);
		CtcFixPoint ft1(ct1,0.1);

		CtcFwdBwd ct2(c_sup,GT,AFFINE2_MODE);
		CtcFixPoint ft2(ct2,0.1);

		LargestFirst bbb;
		CellStack ccc;
		Solver sol1(ft1,bbb, ccc, 0.1 );
		Solver sol2(ft2,bbb, ccc, 0.1 );


		std::cout  << " DEP = "<< I<< "  "  << f<< std::endl;
		std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
		std::cout<< " PAVING = "<< I.diam()<< " /// " << sol1.solve(IntervalVector(1,I)).size() << " et "<<sol2.solve(IntervalVector(1,I)).size() << std::endl;
//		return ((sol1.solve(IntervalVector(1,I)).empty()) && (sol2.solve(IntervalVector(1,I)).empty()));
	}
*/
	return true;

}





template<class T>
void TestAffineEval<T>::dist01() {

	const ExprSymbol& xa = ExprSymbol::new_("xa");
	const ExprSymbol& ya = ExprSymbol::new_("ya");
	const ExprSymbol& xb = ExprSymbol::new_("xb");
	const ExprSymbol& yb = ExprSymbol::new_("yb");

	const ExprSymbol* args[4]={&xa, &ya, &xb, &yb};
	Function f(Array<const ExprSymbol>(args,4),sqrt(sqr(xa-ya)+sqr(xb-yb)));

	double _xy[][2] = { {3,3}, {4,4},
						{4,4}, {5,5} };
	IntervalVector box(4,_xy);

	AffineEval<T> evaluator(f);
	Interval res=evaluator.eval(box).i();
	//cout << e.f << endl;
	check(res,Interval(::sqrt(2),::sqrt(2)));
	CPPUNIT_ASSERT(res.is_superset(Interval(::sqrt(2),::sqrt(2))));
}

template<class T>
void TestAffineEval<T>::apply01() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& x2 = ExprSymbol::new_("x2");

	Function f1(x1,x1,"f1");

	Function f2(x2,f1(x2));

	IntervalVector _x2(1,Interval(2,2));

	AffineEval<T> evaluator(f2);
	Interval res=evaluator.eval(_x2).i();
	check(res, Interval(2,2));
	CPPUNIT_ASSERT((f2.eval(_x2)).is_superset(Interval(2,2)));
}

template<class T>
void TestAffineEval<T>::apply02() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& y1 = ExprSymbol::new_("y1");

	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& y2 = ExprSymbol::new_("y2");

	Function f1(x1,y1,x1+y1,"f1");
	Function f2(x2,y2,f1(x2,x2+y2)+y2,"f2");

	//cout << f1 << endl;
	//cout << f2 << endl;

	IntervalVector x(2);
	x[0]=Interval(2,2);
	x[1]=Interval(3,3);

	AffineEval<T> evaluator(f2);
	Interval res=evaluator.eval(x).i();

	check(res, Interval(10,10));
	CPPUNIT_ASSERT((f2.eval_domain(x).i()).is_superset(Interval(10,10)));
}

template<class T>
void TestAffineEval<T>::apply03() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& y1 = ExprSymbol::new_("y1");

	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& y2 = ExprSymbol::new_("y2");

	const ExprSymbol& x3 = ExprSymbol::new_("x3");

	Function f1(x1,y1,x1+y1);
	Function f2(x2,y2,x2*y2);
	Function f3(x3,f1(x3,x3)-f2(x3,x3));

	//cout << f3 << endl;
	IntervalVector _x3(1,Interval(3,3));

	/*
	e.eval().i();
	cout << "f1:---------\n";
	f1.cf.print<Domain>();
	cout << "f2:---------\n";
	f2.cf.print<Domain>();
	cout << "f3:---------\n";
	f3.cf.print<Domain>();
	*/
	AffineEval<T> evaluator(f3);
	Interval res=evaluator.eval(_x3).i();

	check(res, Interval(-3,-3));
	CPPUNIT_ASSERT((f3.eval_domain(_x3).i()).is_superset(Interval(-3,-3)));
}

template<class T>
void TestAffineEval<T>::apply04() {

	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& x3 = ExprSymbol::new_("x3");

	Function f1(x1,sqr(x1));
	Function f2(x2,x2+Interval(1,1));
	Function f3(x3,f2(f1(x3)));

	IntervalVector _x3(1,Interval(3,3));

	AffineEval<T> evaluator(f3);
	Interval res=evaluator.eval(_x3).i();

	check(res, Interval(10,10));
	CPPUNIT_ASSERT((f3.eval_domain(_x3).i()).is_superset(Interval(10,10)));
}

template<class T>
void TestAffineEval<T>::issue242() {
	Function f("x[3]","-x");
	IntervalVector x(3,Interval::ONE);

	AffineEval<T> evaluator(f);
	IntervalVector res=evaluator.eval(x).v();

	CPPUNIT_ASSERT(almost_eq(res,-x,0));
}


//#include "TestAffineEval.cpp"
}

#endif
