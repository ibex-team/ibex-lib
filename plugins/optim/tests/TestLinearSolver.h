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
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(kleemin3);
	CPPUNIT_TEST(kleemin6);
	CPPUNIT_TEST(kleemin8);
	CPPUNIT_TEST(kleemin30);
	CPPUNIT_TEST_SUITE_END();

	void test01();


	void kleemin(int n);

	void kleemin3() {kleemin(3);};
	void kleemin6() {kleemin(6);};
	void kleemin8() {kleemin(8);};
	void kleemin30();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLinearSolver);

} // namespace ibex

#endif // __TEST_CTC_DOUBLEHEAP_H__
