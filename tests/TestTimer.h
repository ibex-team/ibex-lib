/* ============================================================================
 * I B E X - Timer Tests
 * ============================================================================
 * Copyright   : ENSTA-Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan NININ
 * Created     : Mar 2, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_TIMER_H__
#define __TEST_TIMER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Timer.h"
#include "utils.h"

namespace ibex {

class TestTimer : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestTimer);
	

		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
		CPPUNIT_TEST(test03);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
	void test03();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTimer);


} // namespace ibex
#endif // __TEST_STRING_H__
