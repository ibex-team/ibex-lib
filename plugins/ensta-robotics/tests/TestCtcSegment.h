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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_CtcSegment.h"
#include "utils.h"

namespace ibex {

class TestCtcSegment : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCtcSegment);
	

		CPPUNIT_TEST(test_contract_segment);
        CPPUNIT_TEST(test_contract_border);
        CPPUNIT_TEST(test_contract_empty);
        CPPUNIT_TEST(test_contract_degenerate);
        CPPUNIT_TEST(test_call_with_empty_box);
        CPPUNIT_TEST(test_call_with_all_real);
	CPPUNIT_TEST_SUITE_END();

	void test_contract_segment();
	void test_contract_border();
    void test_contract_degenerate();
    void test_contract_empty();
	void test_call_with_empty_box();
	void test_call_with_all_real();
	
	//void test03();
	//void test04();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcSegment);


} // namespace ibex
#endif // __TEST_CTC_INTEGER_H__
