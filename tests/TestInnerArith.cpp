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

// check that the lower bound of (xin,yin) is inside (x,y)
// if lb=true then x.lb+y.lb == z.lb else x.lb and y.lb must be unchanged
// if ub=true then x.ub+y.ub == z.ub else x.ub and y.ub must be unchanged
void TestInnerArith::check_add_sub(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub) {
	Interval x(-1,1);
	Interval y(-1,1);

	iproj_add(z,x,y,xin,yin);

	if (ub)	{
		TEST_ASSERT_DELTA(y.ub()+x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()+x.ub()<=1.0);
	}
	else    {
		TEST_ASSERT(x.ub()==1);
		TEST_ASSERT(y.ub()==1);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()+x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()+x.lb()>=-1.0);
	}
	else {
		TEST_ASSERT(x.lb()==-1);
		TEST_ASSERT(y.lb()==-1);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));

	x=Interval(-1,1);
	y=Interval(-1,1);

	iproj_sub(z,x,y,xin,-yin);
	y=-y;
	//cout << "x=" << x << " y=" << y << " x+y=" << x+y << endl;


	if (ub)	{
		TEST_ASSERT_DELTA(y.ub()+x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()+x.ub()<=1.0);
	}
	else    {
		TEST_ASSERT(x.ub()==1);
		TEST_ASSERT(y.ub()==1);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()+x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()+x.lb()>=-1.0);
	}
	else {
		TEST_ASSERT(x.lb()==-1);
		TEST_ASSERT(y.lb()==-1);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));
}

void TestInnerArith::add_sub01() {
	check_add_sub(Interval(NEG_INFINITY,1.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,true);
}

void TestInnerArith::add_sub01_2() {
	check_add_sub(Interval(-1.0,POS_INFINITY),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::add_sub01_3() {
	check_add_sub(Interval(-1.0,1.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,true);
}

void TestInnerArith::add_sub02() {
	check_add_sub(Interval(NEG_INFINITY,2.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::add_sub02_2() {
	check_add_sub(Interval(-2.0,POS_INFINITY),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::add_sub02_3() {
	check_add_sub(Interval(-2.0,2.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::add_sub03() {
	Interval x(1,2);
	Interval y(1,2);
	iproj_add(Interval(NEG_INFINITY,1.0),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::add_sub03_2() {
	Interval x(-2,-1);
	Interval y(-2,-1);
	iproj_add(Interval(-1.0,POS_INFINITY),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::add_sub04() {
	check_add_sub(Interval(NEG_INFINITY,-2.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,true);
}

void TestInnerArith::add_sub04_2() {
	check_add_sub(Interval(2.0,POS_INFINITY),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::add_sub04_3() {
	check_add_sub(Interval(2.0,3.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::add_sub05_1() {
	check_add_sub(Interval(NEG_INFINITY,1.0),Interval(0.8,0.9),Interval(0,0.05),false,true);
}

void TestInnerArith::add_sub05_2() {
	check_add_sub(Interval(-1.0,POS_INFINITY),Interval(-0.9,-0.8),Interval(-0.05,0),true,false);
}

void TestInnerArith::add_sub05_3() {
	check_add_sub(Interval(-1,1),Interval(0.9,0.95),Interval(-0.95,-0.9), true, true);
}

void TestInnerArith::add_sub05_4() {
	check_add_sub(Interval(-1,1),Interval(-0.95,-0.9),Interval(0.9,0.95), true, true);
}

void TestInnerArith::add_sub05_5() {
	check_add_sub(Interval(-1,1),Interval(0.85,0.9),Interval(0.0,0.05), true, true);
}

void TestInnerArith::add_sub05_6() {
	check_add_sub(Interval(-1,1),Interval(-0.9,-0.85),Interval(-0.05,0.0), true, true);
}

void TestInnerArith::add_sub05_7() {
	check_add_sub(Interval(-1,1),Interval(0,0),Interval(0,0), true, true);
}

void TestInnerArith::add_sub05_8() {
	check_add_sub(Interval(-1,1),Interval(1,1),Interval(0,0), true, true);
}

void TestInnerArith::add_sub05_9() {
	check_add_sub(Interval(-1,1),Interval(-1,-1),Interval(0,0), true, true);
}

void TestInnerArith::add_sub05_10() {
	check_add_sub(Interval(-1,1),Interval(0,0),Interval(1,1), true, true);
}

void TestInnerArith::add_sub05_11() {
	check_add_sub(Interval(-1,1),Interval(0,0),Interval(-1,-1), true, true);
}

void TestInnerArith::add_sub06_1() {
	check_add_sub(Interval(NEG_INFINITY,0),Interval(1,1),Interval(-1,-1), false, true);
}


void TestInnerArith::add_sub06_2() {
	check_add_sub(Interval(0,POS_INFINITY),Interval(1,1),Interval(-1,-1), true, false);
}

void TestInnerArith::add_sub07() {
	Interval x(-1,1);
	Interval y(-1,1);
	iproj_add(0,x,y);
	TEST_ASSERT(x.is_empty() || (x+y==Interval::ZERO));
}

} // end namespace
