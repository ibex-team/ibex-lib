/* ============================================================================
 * I B E X - System Cache Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 30th, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_BXP_SYSTEM_CACHE_H__
#define __TEST_BXP_SYSTEM_CACHE_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>

#include "utils.h"

namespace ibex {

class TestBxpSystemCache : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		goal_eval01();
		goal_gradient01();
		ctrs_eval();
		ctrs_jacobian();
		active_ctrs();
		is_inner();
		active_ctrs_eval();
		active_ctrs_jacobian();
		tearDown();
	}

	void goal_eval01();
	void goal_gradient01();
	void ctrs_eval();
	void ctrs_jacobian();
	void active_ctrs();
	void is_inner();
	void active_ctrs_eval();
	void active_ctrs_jacobian();

};




} // end namespace

#endif // __TEST_BXP_SYSTEM_CACHE_H__
