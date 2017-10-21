/* ============================================================================
 * I B E X - CtcCell Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 10, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CELL_H__
#define __TEST_CELL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "utils.h"

namespace ibex {

class TestCell : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCell);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCell);

} // namespace ibex

#endif // __TEST_CELL_H__
