/* ============================================================================
 * I B E X - VarSet Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 02, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_VAR_SET_H__
#define __TEST_VAR_SET_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestVarSet : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		test01();
		test02();
		test03();
		test04();
		test05();
		test06();
	
		tearDown();
	}

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
};




} // namespace ibex
#endif // __TEST_VAR_SET_H__
