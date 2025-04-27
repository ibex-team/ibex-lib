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

#ifndef __TEST_TRACE_H__
#define __TEST_TRACE_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestTrace : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		eval1();
		tearDown();
	}

	void eval1();
};



} // namespace ibex

#endif // __TEST_TRACE_H__
