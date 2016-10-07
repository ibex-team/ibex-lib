/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_STRING_H__
#define __TEST_STRING_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_String.h"
#include "utils.h"

namespace ibex {

class TestString : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestString);
	

		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestString);


} // namespace ibex
#endif // __TEST_STRING_H__
