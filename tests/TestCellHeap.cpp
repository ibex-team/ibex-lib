/* ============================================================================
 * I B E X - CellHeap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Mar 2, 2014
 * ---------------------------------------------------------------------------- */

#include "TestCellHeap.h"
#include "ibex_CellHeapOptim.h"
#include "ibex_CellHeap_2.h"

using namespace std;

namespace ibex {

void TestCellHeap::test01() {


	CellHeapOptim h1(1,CellHeapOptim::LB);
	CellHeapVarLB h2(1);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	OptimCell *cell;
	for (int i=0; i<1.e7 ;i++) {
		cell = new OptimCell(box.random());
		h1.push(cell);
		h2.push(cell);
	}

	check(h1.top()->box,h2.top()->box);
}
/*
void TestCellHeap::test02() {
	BitSet mask(BitSet::empty(2));
	mask.add(0);
	mask.add(1);

	CellHeap c(2,mask);
	double _box[][2] = {{0.01,0.99},  {0.01,0.99}};
	IntervalVector box(2,_box);

	TEST_THROWS(c.contract(box), EmptyBoxException);
}
*/
} // end namespace
