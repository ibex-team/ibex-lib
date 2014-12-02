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

#ifndef _IBEX_WITH_NOLP_

		TEST_ADD(TestCtcPolytopeHull::lp01);
		TEST_ADD(TestCtcPolytopeHull::fixbug01);

#endif //_IBEX_WITH_NOLP_

	}

	void lp01();

	void fixbug01();
};

} // end namespace ibex
#endif // __TEST_CTC_POLYTOPE_HULL_H__
