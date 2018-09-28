/* ============================================================================
 * I B E X - Generic operators
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 28, 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_GENERIC_OP_H__
#define __TEST_GENERIC_OP_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestGenericOp : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestGenericOp);
	CPPUNIT_TEST(build1);
	CPPUNIT_TEST(simplify1);
	CPPUNIT_TEST_SUITE_END();

	void build1();
	void simplify1();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestGenericOp);

} // namespace ibex

#endif // __TEST_GENERIC_OP_H__
