//============================================================================
//                                  I B E X                                   
// File        : ibex_TestNewton.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 10, 2012
// Last Update : Jun 10, 2012
//============================================================================

#ifndef __TEST_NEWTON_H__
#define __TEST_NEWTON_H__

#include "cpptest.h"
#include "ibex_Newton.h"
#include "utils.h"

namespace ibex {

class TestNewton : public TestIbex {

public:
	TestNewton() {
		TEST_ADD(TestNewton::newton01);
	}

	void newton01();
};

} // end namespace ibex
#endif // __TEST_NEWTON_H__
