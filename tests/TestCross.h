/* ============================================================================
 * I B E X - Operators Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Oct 7, 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CROSS_PRODUCT_H__
#define __TEST_CROSS_PRODUCT_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCrossProduct : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCrossProduct);
#ifndef _IBEX_WITH_NOLP_
	CPPUNIT_TEST(eval1);

#endif
	CPPUNIT_TEST_SUITE_END();

	void eval1();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCrossProduct);

} // namespace ibex

#endif // __TEST_CROSS_PRODUCT_H__
