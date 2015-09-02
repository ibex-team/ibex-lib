/* ============================================================================
 * I B E X - VarSet Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 02, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_VAR_SET_H__
#define __TEST_VAR_SET_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestVarSet : public TestIbex {

public:
	TestVarSet() {

		TEST_ADD(TestVarSet::test01);
		TEST_ADD(TestVarSet::test02);
		TEST_ADD(TestVarSet::test03);
		TEST_ADD(TestVarSet::test04);
		TEST_ADD(TestVarSet::test05);
	}

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
};

} // namespace ibex
#endif // __TEST_VAR_SET_H__
