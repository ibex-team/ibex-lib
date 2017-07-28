/* ============================================================================
 * I B E X - DoubleHeap Tests
 * ============================================================================
 * Copyright   : ENSTA-Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_LINEARSOLVER_H__
#define __TEST_LINEARSOLVER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_LinearSolver.h"

namespace ibex {

class TestLinearSolver : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestLinearSolver);
	CPPUNIT_TEST(kleemin3);
	//CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void kleemin3();
	//void test02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLinearSolver);

} // namespace ibex

#endif // __TEST_CTC_DOUBLEHEAP_H__
