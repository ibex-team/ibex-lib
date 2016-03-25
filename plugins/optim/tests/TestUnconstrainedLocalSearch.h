//============================================================================
//                                  I B E X                                   
// File        : TestUnconstrainedLocalSearch.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 21, 2014
// Last Update : Mar 21, 2014
//============================================================================

#ifndef __TEST_UNCONSTRAINED_LOCAL_SEARCH_H__
#define __TEST_UNCONSTRAINED_LOCAL_SEARCH_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestUnconstrainedLocalSearch : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestUnconstrainedLocalSearch);
	
		CPPUNIT_TEST(almost_diag);
	CPPUNIT_TEST_SUITE_END();

	void simple01();
	void almost_diag();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestUnconstrainedLocalSearch);


} // end namespace ibex
#endif // __TEST_UNCONSTRAINED_LOCAL_SEARCH_H__
