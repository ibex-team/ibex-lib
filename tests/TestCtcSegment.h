/* ============================================================================
 * I B E X - CtcSegment Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Benoit DESROCHERS
 * Created     : Nov 10, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_SEGMENT_H__
#define __TEST_CTC_SEGMENT_H__

#include "cpptest.h"
#include "ibex_CtcSegment.h"
#include "utils.h"

namespace ibex {

class TestCtcSegment : public TestIbex {

public:
	TestCtcSegment() {

		TEST_ADD(TestCtcSegment::test_contract_segment);
        TEST_ADD(TestCtcSegment::test_contract_border);
        TEST_ADD(TestCtcSegment::test_contract_empty);
        TEST_ADD(TestCtcSegment::test_contract_degenerate);
        TEST_ADD(TestCtcSegment::test_call_with_empty_box);
        TEST_ADD(TestCtcSegment::test_call_with_all_real);
	}

	void test_contract_segment();
	void test_contract_border();
    void test_contract_degenerate();
    void test_contract_empty();
	void test_call_with_empty_box();
	void test_call_with_all_real();
	
	//void test03();
	//void test04();
};

} // namespace ibex
#endif // __TEST_CTC_INTEGER_H__
