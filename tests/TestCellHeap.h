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
#include "ibex_CellSharedHeap.h"
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
		TEST_ADD(TestCellHeap::test_D00);
		TEST_ADD(TestCellHeap::test_D01);
		TEST_ADD(TestCellHeap::test_D03);
		TEST_ADD(TestCellHeap::test_D04);
		TEST_ADD(TestCellHeap::test_D05);
	}


	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test_D00();
	void test_D01();
	void test_D03();
	void test_D04();
	void test_D05();
};

} // namespace ibex
#endif // __TEST_CTC_CELLHEAP_H__
