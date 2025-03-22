//============================================================================
//                                  I B E X                                   
// File        : TestCtcFwdBwd.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 21, 2014
//============================================================================

#ifndef __TEST_CTC_FWD_BWD_H__
#define __TEST_CTC_FWD_BWD_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCtcFwdBwd : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		sqrt_issue28();
		atan2_issue134();
		tearDown();
	}

	void sqrt_issue28();
	void atan2_issue134();

};




} // namespace ibex

#endif // __TEST_CTC_FWD_BWD_H__
