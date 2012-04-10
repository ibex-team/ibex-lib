//============================================================================
//                                  I B E X                                   
// File        : TestHC4.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#ifndef __TEST_HC4_H__
#define __TEST_HC4_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestHC4 : public TestIbex {
public:
	TestHC4() {
		TEST_ADD(TestHC4::ponts30);
	}

	void ponts30();
};

} // end namespace ibex
#endif // __TEST_HC4_H__
