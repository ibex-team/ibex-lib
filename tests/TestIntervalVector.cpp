/* ============================================================================
 * I B E X - Interval Vector Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestIntervalVector.h"
#include "ibex_Interval.h"

using namespace std;

namespace ibex {

#define ERROR 1e-10

void TestIntervalVector::check(double y_actual, double y_expected) {
	TEST_ASSERT_DELTA("==",y_actual,y_expected,ERROR);
}

void TestIntervalVector::check(const Interval& y_actual, const Interval& y_expected) {
	TEST_ASSERT_DELTA("lb", y_actual.lb(),y_expected.lb(),ERROR);
	TEST_ASSERT_DELTA("ub", y_actual.ub(),y_expected.ub(),ERROR);
}

void TestIntervalVector::check(const IntervalVector& y_actual, const IntervalVector& y_expected) {
	TEST_ASSERT(y_actual.size()==y_actual.size());
	for (int i=0; i<y_actual.size(); i++)
		check(y_actual[i],y_expected[i]);
}

void TestIntervalVector::cons01() {
	IntervalVector x(2);
	x[0]=Interval::ALL_REALS;
	x[1]=Interval::ALL_REALS;
	check(x,IntervalVector(2));
	check(x,IntervalVector(x));
}

void TestIntervalVector::cons02() {
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(0,1);
	check(x,IntervalVector(2,Interval(0,1)));
	check(x,IntervalVector(x));
}

void TestIntervalVector::cons03() {
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(2,3);
	check(x,IntervalVector(x));
}

void TestIntervalVector::cons04() {
	double bounds[][2] = {{0,1},{2,3}};
	IntervalVector x(2);
	x[0]=Interval(0,1);
	x[1]=Interval(2,3);
	check(x,IntervalVector(2,bounds));
}

void TestIntervalVector::cons05() {
	IntervalVector x(2);
	x[0].set_empty();
	x[1].set_empty();
	check(x,IntervalVector::empty(2));
}

void TestIntervalVector::set_empty01() {
	IntervalVector x(2);
	TEST_ASSERT(!x.is_empty());
	x.set_empty();
	TEST_ASSERT(x.is_empty());
}


void TestIntervalVector::is_empty01() {
	TEST_ASSERT(IntervalVector::empty(2).is_empty());
}

void TestIntervalVector::is_empty02() {
	TEST_ASSERT(!IntervalVector(2).is_empty());
}

void TestIntervalVector::resize01() {
	IntervalVector x(1);
	x[0]=Interval(1,2);
	x.resize(3);
	TEST_ASSERT(x.size()==3);
	check(x[0],Interval(1,2));
	check(x[1],Interval::ALL_REALS);
	check(x[2],Interval::ALL_REALS);
}

void TestIntervalVector::resize02() {
	IntervalVector x(1);
	x[0]=Interval(1,2);
	x.resize(1);
	TEST_ASSERT(x.size()==1);
	check(x[0],Interval(1,2));
}

void TestIntervalVector::resize03() {
	IntervalVector x(2);
	x[0]=Interval(1,2);
	x.set_empty();
	x.resize(3);
	TEST_ASSERT(x.size()==3);
	TEST_ASSERT(x.is_empty());
	TEST_ASSERT(x[2].is_empty());
}

void TestIntervalVector::resize04() {
	IntervalVector x(5);
	x[0]=Interval(1,2);
	x[1]=Interval(3,4);
	x.resize(2);
	TEST_ASSERT(x.size()==2);
	check(x[0],Interval(1,2));
	check(x[1],Interval(3,4));
}

static double _x[][2]={{0,1},{2,3},{4,5}};

void TestIntervalVector::subvector01() {
	double _x01[][2]={{0,1},{2,3}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(0,1)==IntervalVector(2,_x01));
}

void TestIntervalVector::subvector02() {
	double _x12[][2]={{2,3},{4,5}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(1,2)==IntervalVector(2,_x12));
}

void TestIntervalVector::subvector03() {
	double _x11[][2]={{2,3}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(1,1)==IntervalVector(1,_x11));
}

void TestIntervalVector::subvector04() {
	double _x22[][2]={{4,5}};
	TEST_ASSERT(IntervalVector(3,_x).subvector(2,2)==IntervalVector(1,_x22));
}

void TestIntervalVector::subvector05() {
	TEST_ASSERT(IntervalVector(3,_x).subvector(0,2)==IntervalVector(3,_x));
}



} // namespace ibex
