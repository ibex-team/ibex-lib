/* ============================================================================
 * I B E X - Fritz-John Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 07, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FRITZ_JOHN_H__
#define __TEST_FRITZ_JOHN_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestFritzJohn : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestFritzJohn);
	
		CPPUNIT_TEST(test01);
	CPPUNIT_TEST_SUITE_END();

	void test01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFritzJohn);


} // end namespace

#endif /* __TEST_FRITZ_JOHN_H__ */
