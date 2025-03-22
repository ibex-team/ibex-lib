/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 3, 2012
 * Last update : Jan 28, 2020
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_HC4_REVISE_H__
#define __TEST_HC4_REVISE_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestHC4Revise : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
		id01();
		add01();
		add02();
		add03();
		add04();
		mul01();
		min01();
		dist01();
		dist02();
		dist02();
		vec01();
		vec02();
		vec03();
		issue431();
		tearDown();
	}

	void id01();
	void add01();
	void add02();
	void add03();
	void add04();

	void mul01();
	void min01();

	void dist01();
	void dist02();
	void vec01();
	void vec02();
	void vec03();

	// case where f(x) is a subset of y
	// but x is not fully inside the definition
	// domain of f.
	void issue431();

};




} // end namespace

#endif /* __TEST_HC4Revise_H__ */
