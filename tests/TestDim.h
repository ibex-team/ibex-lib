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

#ifndef __TEST_DIM_H__
#define __TEST_DIM_H__

#include "cpptest.h"
#include "ibex_Dim.h"
#include "utils.h"

namespace ibex {

class TestDim : public TestIbex {

public:
	TestDim() {

		TEST_ADD(TestDim::test01);
		TEST_ADD(TestDim::test02);
		TEST_ADD(TestDim::test03);
		TEST_ADD(TestDim::test04);
	}

	void test01();
	void test02();
	void test03();
	void test04();
};

} // namespace ibex
#endif // __TEST_DIM_H__
