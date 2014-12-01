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

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestCtcPolytopeHull : public TestIbex {
public:
	TestCtcPolytopeHull() {
		TEST_ADD(TestCtcPolytopeHull::lp01);
		TEST_ADD(TestCtcPolytopeHull::fixbug01);
	}

	void lp01();

	void fixbug01();
};

} // end namespace ibex
#endif // __TEST_CTC_POLYTOPE_HULL_H__
