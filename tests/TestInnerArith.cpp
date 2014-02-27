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
#include "ibex_Function.h"

using namespace std;

namespace ibex {

static double error=1e-10;

// All the addition/subtraction are tested on the box [-1,1]x[-1,1]
//
// check that the lower bound of (xin,yin) is inside (x,y)
// if lb=true then x.lb+y.lb == z.lb else x.lb and y.lb must be unchanged
// if ub=true then x.ub+y.ub == z.ub else x.ub and y.ub must be unchanged
void TestInnerArith::check_add_sub(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub) {
	const double L=-1.0;
	const double U= 1.0;
	Interval x(L,U);
	Interval y(L,U);

	if (!iproj_add(z,x,y,xin,yin)) TEST_ASSERT(false);

	//cout << "x=" << x << " y=" << y << " z=" << z << endl;
	if (ub)	{
		TEST_ASSERT_DELTA(y.ub()+x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()+x.ub()<=z.ub());
	}
	else    {
		TEST_ASSERT(x.ub()==U);
		TEST_ASSERT(y.ub()==U);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()+x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()+x.lb()>=z.lb());
	}
	else {
		TEST_ASSERT(x.lb()==L);
		TEST_ASSERT(y.lb()==L);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));

	x=Interval(L,U);
	y=Interval(L,U);

	iproj_sub(z,x,y,xin,-yin);
	y=-y;
	//cout << "x=" << x << " y=" << y << " x+y=" << x+y << endl;


	if (ub)	{
		TEST_ASSERT_DELTA(y.ub()+x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()+x.ub()<=z.ub());
	}
	else    {
		TEST_ASSERT(x.ub()==U);
		TEST_ASSERT(y.ub()==U);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()+x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()+x.lb()>=z.lb());
	}
	else {
		TEST_ASSERT(x.lb()==L);
		TEST_ASSERT(y.lb()==L);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));
}

// All the monotonous multiplication/division are tested on the box [0.5,2]x[0.5,2]
// All the non-mono multiplication/division are tested on the box [-2,2]x[-2,2]
//
// check that the lower bound of (xin,yin) is inside (x,y)
// if lb=true then x.lb*y.lb == z.lb else x.lb and y.lb must be unchanged
// if ub=true then x.ub*y.ub == z.ub else x.ub and y.ub must be unchanged
void TestInnerArith::check_mul_div_mono(const Interval& z, const Interval& xin, const Interval& yin, bool lb, bool ub) {
	const double L=0.5;
	const double U=2.0;
	Interval x(L,U);
	Interval y(L,U);

	iproj_mul(z,x,y,xin,yin);

	if (ub)	{
		TEST_ASSERT_DELTA(y.ub()*x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()*x.ub()<=z.ub());
	}
	else    {
		TEST_ASSERT(x.ub()==U);
		TEST_ASSERT(y.ub()==U);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()*x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()*x.lb()>=z.lb());
	}
	else {
		TEST_ASSERT(x.lb()==L);
		TEST_ASSERT(y.lb()==L);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));

	Interval x2(L,U);
	Interval z2=1/z;
	Interval xin2;
	if (xin.is_empty())
		xin2=Interval::EMPTY_SET;
	else {
		xin2=1/xin; // we must have xin2 \superset 1/xin
		assert((xin2/yin).is_subset(z2)); // and, at the same time, xin2/yin \subset 1/z
	}
	x=Interval(L,U);
	y=Interval(L,U);

	// we check now (1/x) / y = 1/z
	// note: we take the midpoint of xin
	// because we need an "inner division"

	iproj_div(z2,x2,y,xin2,yin);
	Interval one=1.0;
	//cout << "x2=" << x2 << " y=" << y << endl;
	iproj_div(x2,one,x); // means x=1/x;

	if (ub)	{
		//cout << "x=" << x << " y=" << y << " x*y=" << x.ub()*y.ub() << " " << z.ub() << endl;
		TEST_ASSERT_DELTA(y.ub()*x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()*x.ub()<=z.ub());
	}
	else    {
		TEST_ASSERT(x.ub()==U);
		TEST_ASSERT(y.ub()==U);
	}

	if (lb) {
		TEST_ASSERT_DELTA(y.lb()*x.lb(),z.lb(),error);
		TEST_ASSERT(y.lb()*x.lb()>=z.lb());
	}
	else {
		TEST_ASSERT(x.lb()==L);
		TEST_ASSERT(y.lb()==L);
	}

	TEST_ASSERT(xin.is_subset(x));
	TEST_ASSERT(yin.is_subset(y));
}

// All the monotonous multiplication/division are tested on the box [0.5,2]x[0.5,2]
// All the non-mono multiplication/division are tested on the box [-2,2]x[-2,2]
//
// check that the lower bound of (xin,yin) is inside (x,y)
// if lb=true then x.lb*y.lb == z.lb else x.lb and y.lb must be unchanged
// if ub=true then x.ub*y.ub == z.ub else x.ub and y.ub must be unchanged
void TestInnerArith::check_mul_div(const Interval& z, const Interval& xin, const Interval& yin, bool ll, bool lr, bool ul, bool ur) {
	const double L=-2.0;
	const double U=2.0;
	Interval x(L,U);
	Interval y(L,U);

	iproj_mul(z,x,y,xin,yin);
  	if (ll)	{
  		//
  		// !!!!!!!!!!!!!! the current code is not optimal.... !!!!!!!!!!!!!
  		//
		//TEST_ASSERT_DELTA(y.lb()*x.lb(),z.ub(),error);


  		TEST_ASSERT(y.lb()*x.lb()<=z.ub());
	}
	if (lr) {
		//TEST_ASSERT_DELTA(y.lb()*x.ub(),z.lb(),error);
		TEST_ASSERT(y.lb()*x.ub()>=z.lb());
	}
	if (ul)	{
		//TEST_ASSERT_DELTA(y.ub()*x.lb(),z.lb(),error);
		TEST_ASSERT(y.ub()*x.lb()>=z.lb());
	}
	if (ur) {
		//TEST_ASSERT_DELTA(y.ub()*x.ub(),z.ub(),error);
		TEST_ASSERT(y.ub()*x.ub()<=z.ub());
	}

	if (!ll && !ul) TEST_ASSERT(x.lb()==L);
	if (!ll && !lr) TEST_ASSERT(y.lb()==L);
	if (!lr && !ur) TEST_ASSERT(x.ub()==U);
	if (!ul && !ur) TEST_ASSERT(y.ub()==U);

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
	check_add_sub(Interval(-1,1),0,0, true, true);
}

void TestInnerArith::add_sub05_8() {
	check_add_sub(Interval(-1,1),1,0, true, true);
}

void TestInnerArith::add_sub05_9() {
	check_add_sub(Interval(-1,1),-1,0, true, true);
}

void TestInnerArith::add_sub05_10() {
	check_add_sub(Interval(-1,1),0,1, true, true);
}

void TestInnerArith::add_sub05_11() {
	check_add_sub(Interval(-1,1),0,-1, true, true);
}

void TestInnerArith::add_sub06_1() {
	check_add_sub(Interval(NEG_INFINITY,0),1,-1, false, true);
}


void TestInnerArith::add_sub06_2() {
	check_add_sub(Interval(0,POS_INFINITY),1,-1, true, false);
}

void TestInnerArith::add_sub07() {
	Interval x(-1,1);
	Interval y(-1,1);
	iproj_add(0,x,y);
	TEST_ASSERT(x.is_empty() || (x+y==Interval::ZERO));
}

void TestInnerArith::mul_div_mono01() {
	// warning: do not set lower bound to a negative number (ex:NEG_INFINITY) because we also test division
	// so we chose lower bound < 0.25
	check_mul_div_mono(Interval(0.2,2.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,true);
}

void TestInnerArith::mul_div_mono01_2() {
	check_mul_div_mono(Interval(0.5,4.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::mul_div_mono01_3() {
	check_mul_div_mono(Interval(0.5,2.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,true);
}

void TestInnerArith::mul_div_mono02() {
	check_mul_div_mono(Interval(0.2,4.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::mul_div_mono02_2() {
	check_mul_div_mono(Interval(0.25,4.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::mul_div_mono02_3() {
	check_mul_div_mono(Interval(0.25,4.0),Interval::EMPTY_SET,Interval::EMPTY_SET,false,false);
}

void TestInnerArith::mul_div_mono03() {
	Interval x(0.5,2);
	Interval y(0.5,2);
	iproj_mul(Interval(0.1,0.2),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::mul_div_mono03_2() {
	Interval x(-2,-0.5);
	Interval y(0.5,2);
	iproj_mul(Interval(-0.2,-0.1),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::mul_div_mono04() {
	check_mul_div_mono(Interval(0.1,0.25),Interval::EMPTY_SET,Interval::EMPTY_SET,false,true);
}

void TestInnerArith::mul_div_mono04_2() {
	check_mul_div_mono(Interval(4.0,5.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::mul_div_mono04_3() {
	check_mul_div_mono(Interval(4.0,5.0),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false);
}

void TestInnerArith::mul_div_mono05_1() {
	check_mul_div_mono(Interval(0.1,2.0),Interval(1.8,1.9),Interval(1.0,1.05),false,true);
}

void TestInnerArith::mul_div_mono05_2() {
	check_mul_div_mono(Interval(0.5,4.0),Interval(0.6,0.7),Interval(0.85,0.90),true,false);
}

void TestInnerArith::mul_div_mono05_3() {
	check_mul_div_mono(Interval(0.5,2),Interval(1.9,1.95),Interval(0.55,0.6), true, true);
}

void TestInnerArith::mul_div_mono05_4() {
	check_mul_div_mono(Interval(0.5,2),Interval(0.55,0.6),Interval(1.9,1.95), true, true);
}

void TestInnerArith::mul_div_mono05_5() {
	check_mul_div_mono(Interval(0.5,2),Interval(1.8,1.9),Interval(1.0,1.05), true, true);
}

void TestInnerArith::mul_div_mono05_6() {
	check_mul_div_mono(Interval(0.5,2),Interval(0.6,0.7),Interval(0.85,0.90), true, true);
}

void TestInnerArith::mul_div_mono05_7() {
	check_mul_div_mono(Interval(0.5,2),1,1, true, true);
}

void TestInnerArith::mul_div_mono05_8() {
	check_mul_div_mono(Interval(0.5,2),2,1, true, true);
}

void TestInnerArith::mul_div_mono05_9() {
	check_mul_div_mono(Interval(0.5,2),0.5,1, true, true);
}

void TestInnerArith::mul_div_mono05_10() {
	check_mul_div_mono(Interval(0.5,2),1,2, true, true);
}

void TestInnerArith::mul_div_mono05_11() {
	check_mul_div_mono(Interval(0.5,2),1,0.5, true, true);
}

void TestInnerArith::mul_div_mono06_1() {
	check_mul_div_mono(Interval(0.1,1),2,0.5, false, true);
}


void TestInnerArith::mul_div_mono06_2() {
	check_mul_div_mono(Interval(1,4.0),2,0.5, true, false);
}

void TestInnerArith::mul_div_mono07() {
	Interval x(0.5,2);
	Interval y(0.5,2);
	iproj_mul(1,x,y);
	TEST_ASSERT(x.is_empty() || (x*y==1));
}

void TestInnerArith::mul08() {
	check_mul_div(Interval(NEG_INFINITY,1),Interval::EMPTY_SET,Interval::EMPTY_SET,true,false,false,true);
}

void TestInnerArith::mul09() {
	Interval x(-0.5,2);
	Interval y(-0.5,2);
	iproj_mul(Interval(1,2),x,y);
	TEST_ASSERT_DELTA(x.lb()*y.lb(),1.0,error);
	TEST_ASSERT_DELTA(x.ub()*y.ub(),2.0,error);
	TEST_ASSERT((x*y).is_subset(Interval(1,2)));
}

void TestInnerArith::mul10() {
	Interval x(-2,0.5);
	Interval y(-2,0.5);
	iproj_mul(Interval(1,2),x,y);
	TEST_ASSERT_DELTA(x.lb()*y.lb(),2.0,error);
	TEST_ASSERT_DELTA(x.ub()*y.ub(),1.0,error);
	TEST_ASSERT((x*y).is_subset(Interval(1,2)));
}

void TestInnerArith::mul11() {
	Interval x(-2,2);
	Interval y(-2,2);
	iproj_mul(Interval(1,2),x,y);
	TEST_ASSERT(fabs(x.lb()*y.lb()-1.0)<error || fabs(x.ub()*y.ub()-1.0)<error);
	TEST_ASSERT((x*y).is_subset(Interval(1,2)));
}

void TestInnerArith::mul12_1() {
	check_mul_div(Interval(-1,1), Interval::EMPTY_SET, Interval::EMPTY_SET, true,true,true,true);
}

void TestInnerArith::mul12_2() {
	check_mul_div(Interval(-4,1), Interval::EMPTY_SET, Interval::EMPTY_SET, true,true,true,true);
}

void TestInnerArith::mul12_3() {
	check_mul_div(Interval(-1,4), Interval::EMPTY_SET, Interval::EMPTY_SET, true,true,true,true);
}

void TestInnerArith::mul12_4() {
	check_mul_div(Interval(-4,4), Interval::EMPTY_SET, Interval::EMPTY_SET, true,true,true,true);
}

void TestInnerArith::mul13_1() {
	check_mul_div(Interval(-1,1), 2, 0.1, true,false,false,true);
}

void TestInnerArith::mul13_2() {
	check_mul_div(Interval(-1,1), -1, -1, true,false,false,true);
}

void TestInnerArith::mul13_3() {
	check_mul_div(Interval(-1,1), 0.1, -2, true,false,false,true);
}

void TestInnerArith::mul13_4() {
	check_mul_div(Interval(-1,1), -2, 0.1, true,false,false,true);
}

void TestInnerArith::mul14_1() {
	Interval x(-1,1);
	Interval y=Interval::ZERO;
	iproj_mul(Interval(-1,1),x,y);
	TEST_ASSERT(x==Interval(-1,1));
}

void TestInnerArith::mul14_2() {
	Interval x(-1,1);
	Interval y=Interval::ZERO;
	iproj_mul(Interval(1,1),x,y);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(y.is_empty());
}

void TestInnerArith::mul14_3() {
	Interval x(-1,1);
	Interval y=Interval::ZERO;
	iproj_mul(Interval::ZERO,x,y,Interval::ZERO,Interval::ZERO);
	TEST_ASSERT(x==Interval(-1,1));
}

void TestInnerArith::div08_1() {
	Interval x(-2,2);
	Interval y(0.5,1.0);
	iproj_div(Interval(NEG_INFINITY,1),x,y);
	TEST_ASSERT_DELTA(x.ub()/y.lb(),1.0,error);
	TEST_ASSERT(x.lb()==-2);
	TEST_ASSERT(y.ub()==1.0);
}

void TestInnerArith::div08_2() {
	Interval x(-2,2);
	Interval y(-1,-0.5);
	iproj_div(Interval(NEG_INFINITY,1),x,y);
	TEST_ASSERT_DELTA(x.lb()/y.ub(),1.0,error);
	TEST_ASSERT(x.ub()==2);
	TEST_ASSERT(y.lb()==-1.0);
}

void TestInnerArith::div09_1() {
	Interval x(-2,-1);
	Interval y(-2,2.0);
	iproj_div(Interval(NEG_INFINITY,-1),x,y);

	TEST_ASSERT_DELTA(x.ub()/y.ub(),-1.0,error);
	TEST_ASSERT(x.lb()==-2);
	TEST_ASSERT(y.lb()==next_float(0));
}

void TestInnerArith::div09_2() {
	Interval x(1,2);
	Interval y(-2,2.0);
	iproj_div(Interval(NEG_INFINITY,-1),x,y);

	TEST_ASSERT_DELTA(x.lb()/y.lb(),-1.0,error);
	TEST_ASSERT(x.ub()==2);
	TEST_ASSERT(y.ub()==previous_float(0));
}


void TestInnerArith::abs01() {
	Interval x;
	bool f=iproj_abs(Interval::EMPTY_SET,x);
	TEST_ASSERT(!f && x.is_empty());
}

void TestInnerArith::abs02() {
	Interval x(0,2);
	bool f=iproj_abs(Interval(1,3),x);
	TEST_ASSERT(f);
	check(x,Interval(1,2));
}

void TestInnerArith::abs03() {
	Interval x(-3,-1);
	bool f=iproj_abs(Interval(2,4),x);
	TEST_ASSERT(f);
	check(x,Interval(-3,-2));
}

void TestInnerArith::abs04() {
	Interval x(-2,2);
	bool f=iproj_abs(Interval(1,3),x,-2);
	TEST_ASSERT(f);
	check(x,Interval(-2,-1));
}

void TestInnerArith::abs05() {
	Interval x(-2,2);
	bool f=iproj_abs(Interval(1,3),x,2);
	TEST_ASSERT(f);
	check(x,Interval(1,2));
}

void TestInnerArith::sqrt01() {
	Interval x;
	bool f=iproj_sqrt(Interval::EMPTY_SET,x);
	TEST_ASSERT(!f && x.is_empty());
}

void TestInnerArith::sqrt02() {
	Interval x;
	bool f=iproj_sqrt(Interval(-1),x);
	TEST_ASSERT(!f && x.is_empty());
}

void TestInnerArith::sqrt03() {
	Interval x(0,6);
	Interval y(2,3);
	TEST_ASSERT(iproj_sqrt(y,x));
	check(x,Interval(4,6));
	//TEST_ASSERT(y.is_superset(sqrt(Interval(x.ub()))));
	//TEST_ASSERT(y.is_superset(sqrt(Interval(x.lb()))));
	TEST_ASSERT(x.lb()>=sqr(Interval(y.lb())).ub());
	TEST_ASSERT(x.ub()<=sqr(Interval(y.ub())).lb());
}

void TestInnerArith::sqrt04() {
	Interval x(0,1);
	Interval y(2,3);
	bool f=iproj_sqrt(y,x);
	TEST_ASSERT(!f && x.is_empty());
}

void TestInnerArith::sqrt05() {
	Interval x;
	Interval y(1,POS_INFINITY);
	bool f=iproj_sqrt(y,x);
	TEST_ASSERT(f);
	check(x,y);
}

void TestInnerArith::bugr894() {

	Variable x;
	Function f(x,sqr(x-Interval::ZERO));
	double _pt[][2]={{-1.03653, -1.03653}};
	IntervalVector pt(1,_pt);
	IntervalVector box(1);//,_box);
	f.iproj(Interval::NEG_REALS,box,pt);
	TEST_ASSERT(!f.eval(pt).is_empty());
}

void TestInnerArith::bugr899() {

	Interval x(ibex::previous_float(1.0),1);
	Interval px(ibex::previous_float(1.0),1);
	Interval y(Interval::ZERO);
	iproj_add(Interval(1),x,y,px,y);
	TEST_ASSERT(x==px);
}

} // end namespace
