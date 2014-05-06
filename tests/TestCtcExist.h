/* ============================================================================
 * I B E X - CtcExist Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 05, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_EXIST_H__
#define __TEST_CTC_EXIST_H__

#include "cpptest.h"
#include "ibex_CtcExist.h"
#include "utils.h"

namespace ibex {

class TestCtcExist : public TestIbex {

public:
	TestCtcExist() {

		TEST_ADD(TestCtcExist::test01);
		//TEST_ADD(TestCtcExist::test02);
		//TEST_ADD(TestCtcExist::test03);
		//TEST_ADD(TestCtcExist::test04);
	}

	void test01();
	//void test02();
	//void test03();
	//void test04();
};

} // namespace ibex
#endif // __TEST_CTC_EXIST_H__
