/* ============================================================================
 * I B E X - CtcExist Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 05, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_EXIST_H__
#define __TEST_CTC_EXIST_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_CtcExist.h"
#include "utils.h"

namespace ibex {

class TestCtcExist : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	

			test01();
		//	test02();
		//	test03();
		//	test04();
		tearDown();
	}

	void test01();
	//void test02();
	//void test03();
	//void test04();
};




} // namespace ibex
#endif // __TEST_CTC_EXIST_H__
