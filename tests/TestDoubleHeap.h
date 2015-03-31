/* ============================================================================
 * I B E X - DoubleHeap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_DOUBLEHEAP_H__
#define __TEST_CTC_DOUBLEHEAP_H__

#include "cpptest.h"
#include "utils.h"
#include "ibex_DoubleHeap.h"

namespace ibex {


class TestDoubleHeap : public TestIbex {
public:
	TestDoubleHeap() {

		TEST_ADD(TestDoubleHeap::test01);
		TEST_ADD(TestDoubleHeap::test02);
	}


	void test01();
	void test02();
};




} // namespace ibex
#endif // __TEST_CTC_DOUBLEHEAP_H__
