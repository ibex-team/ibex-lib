/* ============================================================================
 * I B E X - CtcSubBox Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jul 20, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_SUB_BOX_H__
#define __TEST_CTC_SUB_BOX_H__

#include "cpptest.h"
#include "ibex_CtcSubBox.h"
#include "utils.h"

namespace ibex {

class TestCtcSubBox : public TestIbex {

public:
	TestCtcSubBox() {

		TEST_ADD(TestCtcSubBox::test01);
		TEST_ADD(TestCtcSubBox::test02);
		//TEST_ADD(TestCtcSubBox::test03);
		//TEST_ADD(TestCtcSubBox::test04);
	}

	void test01();
	void test02();
	//void test03();
	//void test04();
};

} // namespace ibex
#endif // __TEST_CTC_SUB_BOX_H__
