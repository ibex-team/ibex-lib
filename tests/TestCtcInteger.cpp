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

} // end namespace
