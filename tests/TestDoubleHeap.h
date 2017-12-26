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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_DoubleHeap.h"
#include "utils.h"


namespace ibex {

class TestDoubleHeap : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestDoubleHeap);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST(test05);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDoubleHeap);


} // namespace ibex


#endif // __TEST_CTC_DOUBLEHEAP_H__
