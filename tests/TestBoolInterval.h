/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_BOOL_INTERVAL_H__
#define __TEST_BOOL_INTERVAL_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_BoolInterval.h"
#include "utils.h"

namespace ibex {

class TestBoolInterval : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
			test_constructor1();
			test_constructor2();
			test_constructor3();
			test_assignement();
			test_inter1();
			test_inter2();
			test_union1();
			test_union2();
			test_and();
			test_or();	
		tearDown();
	}

	void test_constructor1();
	void test_constructor2();
	void test_constructor3();
	void test_assignement();
	void test_inter1();
	void test_inter2();
	void test_union1();
	void test_union2();
	void test_and();
	void test_or();



};




} // namespace ibex
#endif // __TEST_BOOL_INTERVAL_H__
