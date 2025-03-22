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

#ifndef SRCDIR_TESTS
  #define SRCDIR_TESTS "."
#endif

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Solver.h"
#include "utils.h"

namespace ibex {

class TestSolver : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		empty();
		circle1();
		circle2();
		circle3();
		circle4();
		tearDown();
	}

	void empty();
	void circle1();
	void circle2();
	void circle3();
	void circle4();
};



} // namespace ibex

#endif // __TEST_SOLVER_H__
