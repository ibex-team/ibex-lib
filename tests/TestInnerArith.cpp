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

void TestInnerArith::add01bis() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-3.0,POS_INFINITY),x,y);
	TEST_ASSERT(x.ub()==-1.0)
	TEST_ASSERT(y.ub()==-1.0);
	cout << "x=" << x << " y=" << y << endl;
	TEST_ASSERT_DELTA(y.lb()+x.lb(),-3.0,error);
	TEST_ASSERT(y.lb()+x.lb()>=-3.0);
}

void TestInnerArith::add02() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,4.0),x,y);
	TEST_ASSERT(x==Interval(1,2));
	TEST_ASSERT(y==Interval(1,2));
}

void TestInnerArith::add02bis() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-4.0,POS_INFINITY),x,y);
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

void TestInnerArith::add03bis() {
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

void TestInnerArith::add04bis() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-2.0,POS_INFINITY),x,y);
	TEST_ASSERT(x==Interval(-1,-1));
	TEST_ASSERT(y==Interval(-1,-1));
}

void TestInnerArith::add05() {
	Interval x(1,2);
	Interval y(1,2);
	Interval xin(1.8,1.9);
	Interval yin(1.0,1.05);
	iproj_add(Interval(NEG_INFINITY,3.0),x,y,xin,yin);
	TEST_ASSERT(x.lb()==1.0)
	TEST_ASSERT(y.lb()==1.0);
	TEST_ASSERT_DELTA(y.ub()+x.ub(),3.0,error);
	TEST_ASSERT(y.ub()+x.ub()<=3.0);
	TEST_ASSERT(x.ub()>=xin.ub());
	TEST_ASSERT(y.ub()>=yin.ub());
}

void TestInnerArith::add05bis() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	Interval xin(-1.9,-1.8);
	Interval yin(-1.05,-1.0);
	iproj_add(Interval(-3.0,POS_INFINITY),x,y,xin,yin);
	TEST_ASSERT(x.ub()==-1.0)
	TEST_ASSERT(y.ub()==-1.0);
	TEST_ASSERT_DELTA(y.lb()+x.lb(),-3.0,error);
	TEST_ASSERT(y.lb()+x.lb()>=-3.0);
	TEST_ASSERT(x.lb()<=xin.lb());
	TEST_ASSERT(y.lb()<=yin.lb());
}

void TestInnerArith::add06() {
	Interval x(1,2);
	Interval y(1,2);
	double three=2.0+1.0;
	Interval xin(2.0,2.0);
	Interval yin(1.0,1.0);
	iproj_add(Interval(NEG_INFINITY,three),x,y,xin,yin);
	TEST_ASSERT(x==Interval(1,2));
	TEST_ASSERT(y==Interval(1,1));
}


void TestInnerArith::add06bis() {
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

} // end namespace
