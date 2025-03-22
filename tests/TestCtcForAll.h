/* ============================================================================
 * I B E X - CtcForall Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 21, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_FOR_ALL_H__
#define __TEST_CTC_FOR_ALL_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCtcForAll : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	

			test01();
		tearDown();
	}

	void test01();
};




} // namespace ibex
#endif // __TEST_CTC_FOR_ALL_H__
