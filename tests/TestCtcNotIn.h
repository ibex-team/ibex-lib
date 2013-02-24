/* ============================================================================
 * I B E X - CtcNotIn Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 24, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_NOT_IN_H__
#define __TEST_CTC_NOT_IN_H__

#include "cpptest.h"
#include "ibex_CtcNotIn.h"
#include "utils.h"

namespace ibex {

class TestCtcNotIn : public TestIbex {

public:
	TestCtcNotIn() {

		TEST_ADD(TestCtcNotIn::contract01);
		TEST_ADD(TestCtcNotIn::contract02);
		TEST_ADD(TestCtcNotIn::contract03);
		TEST_ADD(TestCtcNotIn::contract04);
	}

	void contract01();
	void contract02();
	void contract03();
	void contract04();

	void check_not_in(const Interval& x_input, const Interval& x_expected);
};

} // namespace ibex
#endif // __TEST_CTC_NOT_IN_H__
