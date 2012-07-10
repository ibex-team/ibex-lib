//============================================================================
//                                  I B E X                                   
// File        : TestInnerArith.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 6, 2012
// Last Update : Jul 6, 2012
//============================================================================

#include "TestInnerArith.h"
#include "ibex_InnerArith.h"

using namespace std;

namespace ibex {

static double error=1e-10;

void TestInnerArith::add01() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,3.0),x,y);
	TEST_ASSERT(x.lb()==1.0)
	TEST_ASSERT(y.lb()==1.0);
	TEST_ASSERT_DELTA(y.ub()+x.ub(),3.0,error);
	TEST_ASSERT(y.ub()+x.ub()<=3.0);
}

void TestInnerArith::add01_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-3.0,POS_INFINITY),x,y);
	//cout << "x=" << x << "y=" << y << endl;
	TEST_ASSERT(x.ub()==-1.0)
	TEST_ASSERT(y.ub()==-1.0);

	TEST_ASSERT_DELTA(y.lb()+x.lb(),-3.0,error);
	TEST_ASSERT(y.lb()+x.lb()>=-3.0);
}

void TestInnerArith::add01_3() {
	Interval x(-1,1);
	Interval y(-1,1);
	iproj_add(Interval(-1,1),x,y);
	TEST_ASSERT_DELTA(x.lb()+y.lb(),-1,error);
	TEST_ASSERT(x.lb()+y.lb()>=-1);
	TEST_ASSERT_DELTA(x.ub()+y.ub(),+1,error);
	TEST_ASSERT(x.ub()+y.ub()<=1);
}

void TestInnerArith::add02() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,4.0),x,y);
	TEST_ASSERT(x==Interval(1,2));
	TEST_ASSERT(y==Interval(1,2));
}

void TestInnerArith::add02_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-4.0,POS_INFINITY),x,y);
	TEST_ASSERT(x==Interval(-2,-1));
	TEST_ASSERT(y==Interval(-2,-1));
}

void TestInnerArith::add02_3() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-4.0,4.0),x,y);
	TEST_ASSERT(x==Interval(-2,-1));
	TEST_ASSERT(y==Interval(-2,-1));
}

void TestInnerArith::add03() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,1.0),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::add03_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-1.0,POS_INFINITY),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::add04() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,2.0),x,y);
	TEST_ASSERT(x==Interval(1,1));
	TEST_ASSERT(y==Interval(1,1));
}

void TestInnerArith::add04_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-2.0,POS_INFINITY),x,y);
	TEST_ASSERT(x==Interval(-1,-1));
	TEST_ASSERT(y==Interval(-1,-1));
}

void TestInnerArith::add04_3() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-2.0,2.0),x,y);
	TEST_ASSERT(x==Interval(-1,-1));
	TEST_ASSERT(y==Interval(-1,-1));
}

void TestInnerArith::add05_1() {
	Interval x(1,2);
	Interval y(1,2);
	Interval xin(1.8,1.9);
	Interval yin(1.0,1.05);
	iproj_add(Interval(NEG_INFINITY,3.0),x,y,xin,yin);
	TEST_ASSERT(x.lb()==1.0)
	TEST_ASSERT(y.lb()==1.0);
	TEST_ASSERT_DELTA(y.ub()+x.ub(),3.0,error);
	TEST_ASSERT(y.ub()+x.ub()<=3.0);
	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));
}

void TestInnerArith::add05_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	Interval xin(-1.9,-1.8);
	Interval yin(-1.05,-1.0);
	iproj_add(Interval(-3.0,POS_INFINITY),x,y,xin,yin);
	TEST_ASSERT(x.ub()==-1.0);
	TEST_ASSERT(y.ub()==-1.0);
	TEST_ASSERT_DELTA(y.lb()+x.lb(),-3.0,error);
	TEST_ASSERT(y.lb()+x.lb()>=-3.0);
	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));
}

// check that the lower bound of (xin,yin) is inside (x,y)
#define check_05(xinL,xinU,yinL,yinU) \
		Interval x(-1,1); \
		Interval y(-1,1); \
		Interval xin(xinL,xinU); \
		Interval yin(yinL,yinU); \
		iproj_add(Interval(-1.0,1.0),x,y,xin,yin); \
		TEST_ASSERT_DELTA(y.ub()+x.ub(),1.0,error); \
		TEST_ASSERT_DELTA(y.lb()+x.lb(),-1.0,error); \
		TEST_ASSERT(y.ub()+x.ub()<=1.0); \
		TEST_ASSERT(y.lb()+x.lb()>=-1.0); \
		TEST_ASSERT(xin.is_subset(x)); \
		TEST_ASSERT(yin.is_subset(y));

void TestInnerArith::add05_3() {
	check_05(0.9,0.95,-0.95,-0.9);
}

void TestInnerArith::add05_4() {
	check_05(-0.95,-0.9,0.9,0.95);
}

void TestInnerArith::add05_5() {
	check_05(0.85,0.9,0.0,0.05);
}

void TestInnerArith::add05_6() {
	check_05(-0.9,-0.85,-0.05,0.0);
}

void TestInnerArith::add05_7() {
	check_05(0,0,0,0);
}

void TestInnerArith::add05_8() {
	check_05(1,1,0,0);
}

void TestInnerArith::add05_9() {
	check_05(-1,-1,0,0);
}

void TestInnerArith::add05_10() {
	check_05(0,0,1,1);
}

void TestInnerArith::add05_11() {
	check_05(0,0,-1,-1);
}

void TestInnerArith::add06_1() {
	Interval x(1,2);
	Interval y(1,2);
	double three=2.0+1.0;
	Interval xin(2.0,2.0);
	Interval yin(1.0,1.0);
	iproj_add(Interval(NEG_INFINITY,three),x,y,xin,yin);
	TEST_ASSERT(x==Interval(1,2));
	TEST_ASSERT(y==Interval(1,1));
}


void TestInnerArith::add06_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	double mthree=-2.0-1.0;
	Interval xin(-2.0,-2.0);
	Interval yin(-1.0,-1.0);
	iproj_add(Interval(mthree,POS_INFINITY),x,y,xin,yin);
	cout << "x=" << x << " y=" << y << endl;
	TEST_ASSERT(x==Interval(-2,-1));
	TEST_ASSERT(y==Interval(-1,-1));
}

void TestInnerArith::add07() {
	Interval x(-1,1);
	Interval y(-1,1);
	iproj_add(0,x,y);
	TEST_ASSERT(x.is_empty() || (x+y==Interval::ZERO));
}

} // end namespace
