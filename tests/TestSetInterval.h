//============================================================================
//                                  I B E X                                   
// File        : TestSetInterval.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#ifndef __TEST_SET_INTERVAL_H__
#define __TEST_SET_INTERVAL_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestSetInterval : public TestIbex {
public:
	TestSetInterval() {
		TEST_ADD(TestSetInterval::diff01);
		TEST_ADD(TestSetInterval::diff02);
	}

	void diff01();
	void diff02();
};

} // end namespace ibex
#endif // __TEST_SET_INTERVAL_H__
