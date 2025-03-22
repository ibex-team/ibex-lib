//============================================================================
//                                  I B E X                                   
// File        : TestCtcPolytopeHull.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 22, 2014
//============================================================================

#ifndef __TEST_CTC_POLYTOPE_HULL_H__
#define __TEST_CTC_POLYTOPE_HULL_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_LPSolver.h"

namespace ibex {

class TestCtcPolytopeHull : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();
	

#ifndef __IBEX_NO_LP_SOLVER__

			lp01();
			fixbug01();

#endif //__IBEX_NO_LP_SOLVER__

		tearDown();
	}

	void lp01();

	void fixbug01();
};




} // end namespace ibex
#endif // __TEST_CTC_POLYTOPE_HULL_H__
