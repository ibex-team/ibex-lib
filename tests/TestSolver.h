/* ============================================================================
 * I B E X - Solver Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 2, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_SOLVER_H__
#define __TEST_SOLVER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Solver.h"
#include "utils.h"

namespace ibex {

class TestSolver : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestSolver);
	CPPUNIT_TEST(circle1);
	CPPUNIT_TEST(circle2);
	CPPUNIT_TEST(circle3);
	CPPUNIT_TEST(circle4);
	CPPUNIT_TEST_SUITE_END();

	void circle1();
	void circle2();
	void circle3();
	void circle4();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSolver);

} // namespace ibex

#endif // __TEST_SOLVER_H__
