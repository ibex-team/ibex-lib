/* ============================================================================
 * I B E X - CtcInteger Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jul 20, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_INTEGER_H__
#define __TEST_CTC_INTEGER_H__

#include "cpptest.h"
#include "ibex_CtcInteger.h"
#include "utils.h"

namespace ibex {

class TestCtcInteger : public TestIbex {

public:
	TestCtcInteger() {

		TEST_ADD(TestCtcInteger::test01);
		TEST_ADD(TestCtcInteger::test02);
		//TEST_ADD(TestCtcInteger::test03);
		//TEST_ADD(TestCtcInteger::test04);
	}

	void test01();
	void test02();
	//void test03();
	//void test04();
};

} // namespace ibex
#endif // __TEST_CTC_INTEGER_H__
