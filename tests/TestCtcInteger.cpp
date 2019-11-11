/* ============================================================================
 * I B E X - CtcInteger Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestCtcInteger.h"
#include "ibex_CtcInteger.h"

using namespace std;

namespace ibex {

void TestCtcInteger::test01() {
	BitSet mask(BitSet::empty(4));
	mask.add(1);
	mask.add(3);

	CtcInteger c(4,mask);
	double _box[][2] = {{0.5,1.5},  {1.5,2.5},  {2.5,3.5},  {3.5,4.5}};
	IntervalVector box(4,_box);

	double _resbox[][2] = {{0.5,1.5},  {2,2},  {2.5,3.5},  {4,4}};
	IntervalVector resbox(4,_resbox);

	c.contract(box);

	check(box,resbox);
}

void TestCtcInteger::test02() {
	BitSet mask(BitSet::empty(2));
	mask.add(0);
	mask.add(1);

	CtcInteger c(2,mask);
	double _box[][2] = {{0.01,0.99},  {0.01,0.99}};
	IntervalVector box(2,_box);

	c.contract(box);

	CPPUNIT_ASSERT(box.is_empty());
}

void TestCtcInteger::test03() {
	IntervalVector x(1,Interval::empty_set());
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval::empty_set());
}

void TestCtcInteger::test04() {
	IntervalVector x(1,Interval(NEG_INFINITY,-0.000001));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval(NEG_INFINITY,-1));
}
void TestCtcInteger::test05() {
	IntervalVector x(1,Interval(0.00000001,POS_INFINITY));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval(1.0,POS_INFINITY));
}

void TestCtcInteger::test06() {
	IntervalVector x(1,Interval::all_reals());
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval::all_reals());
}

void TestCtcInteger::test07() {
	IntervalVector x(1,Interval(0.01,2.99));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval(1,2));
}

void TestCtcInteger::test08() {
	IntervalVector x(1,Interval(-0.01,2.99));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval(0,2));
}

void TestCtcInteger::test09() {
	IntervalVector x(1,Interval(1.000000001,1.9999999999));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x,Interval::empty_set());
}

void TestCtcInteger::test10() {
	IntervalVector x(1,Interval(1e8, DBL_MAX));
	CtcInteger c(1,BitSet::all(1));
	c.contract(x);
	check(x, Interval(1e8,DBL_MAX));
}
} // end namespace
