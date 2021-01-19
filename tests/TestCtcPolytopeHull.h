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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_LPSolver.h"

namespace ibex {

class TestCtcPolytopeHull : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCtcPolytopeHull);
	

#ifndef __IBEX_NO_LP_SOLVER__

		CPPUNIT_TEST(lp01);
		CPPUNIT_TEST(fixbug01);

#endif //__IBEX_NO_LP_SOLVER__

	CPPUNIT_TEST_SUITE_END();

	void lp01();

	void fixbug01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcPolytopeHull);


} // end namespace ibex
#endif // __TEST_CTC_POLYTOPE_HULL_H__
