/* ============================================================================
 * I B E X - CellHeap Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_CELLHEAP_H__
#define __TEST_CTC_CELLHEAP_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCellHeap : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCellHeap);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST(test05);
	CPPUNIT_TEST(test_D00);
	CPPUNIT_TEST(test_D01);
	CPPUNIT_TEST(test_D03);
	CPPUNIT_TEST(test_D04);
	CPPUNIT_TEST(test_D05);
	CPPUNIT_TEST_SUITE_END();

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

CPPUNIT_TEST_SUITE_REGISTRATION(TestCellHeap);

} // namespace ibex

#endif // __TEST_CTC_CELLHEAP_H__
