/* ============================================================================
 * I B E X - Dim Tests
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
	BoolMask mask(4);
	mask[0]=false;
	mask[1]=true;
	mask[2]=false;
	mask[3]=true;

	CtcInteger c(4,mask);
	double _box[][2] = {{0.5,1.5},  {1.5,2.5},  {2.5,3.5},  {3.5,4.5}};
	IntervalVector box(4,_box);

	double _resbox[][2] = {{0.5,1.5},  {2,2},  {2.5,3.5},  {4,4}};
	IntervalVector resbox(4,_resbox);

	c.contract(box);

	check(box,resbox);
}

void TestCtcInteger::test02() {
	BoolMask mask(2);
	mask[0]=true;
	mask[1]=true;

	CtcInteger c(2,mask);
	double _box[][2] = {{0.01,0.99},  {0.01,0.99}};
	IntervalVector box(2,_box);

	TEST_THROWS(c.contract(box), EmptyBoxException);
}

} // end namespace
