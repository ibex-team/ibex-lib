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

void TestIntervalVector::is_empty01() {
	TEST_ASSERT(IntervalVector::empty(2).is_empty());
}

void TestIntervalVector::is_empty02() {
	TEST_ASSERT(!IntervalVector(2).is_empty());
}



} // namespace ibex
