/* ============================================================================
 * I B E X - DoubleHeap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_DOUBLEHEAP_H__
#define __TEST_CTC_DOUBLEHEAP_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_DoubleHeap.h"
#include "utils.h"


namespace ibex {

class TestDoubleHeap : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		test01();
		test02();
		test03();
		test04();
		test05();
		tearDown();
	}

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
};




} // namespace ibex


#endif // __TEST_CTC_DOUBLEHEAP_H__
