/* ============================================================================
 * I B E X - CellHeap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_CELLHEAP_H__
#define __TEST_CTC_CELLHEAP_H__

#include "cpptest.h"
#include "ibex_CellHeapOptim.h"
#include "ibex_CellHeap_2.h"
#include "utils.h"

namespace ibex {

class TestCellHeap : public TestIbex {

public:
	TestCellHeap() {

		TEST_ADD(TestCellHeap::test01);
		TEST_ADD(TestCellHeap::test02);
		TEST_ADD(TestCellHeap::test03);
		TEST_ADD(TestCellHeap::test04);
		TEST_ADD(TestCellHeap::test05);
	}

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
};

} // namespace ibex
#endif // __TEST_CTC_CELLHEAP_H__
