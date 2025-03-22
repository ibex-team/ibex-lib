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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Newton.h"
#include "utils.h"

namespace ibex {

class TestNewton : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		newton01();
		inflating_newton01();
		inflating_newton02();
		ctc_parameter01();

		tearDown();
	}

	void newton01();
	void inflating_newton01();
	void inflating_newton02();
	void ctc_parameter01();
};




} // end namespace ibex
#endif // __TEST_NEWTON_H__
