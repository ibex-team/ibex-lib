//============================================================================
//                                  I B E X
// File        : TestRoundRobin.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 25, 2017
//============================================================================

#ifndef __TEST_ROUND_ROBIN_H__
#define __TEST_ROUND_ROBIN_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_RoundRobin.h"
#include "utils.h"

namespace ibex {

class TestRoundRobin : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		test01();
		test02();
		tearDown();
	}

	void test01();
	void test02();

};



} // namespace ibex

#endif // __TEST_ROUND_ROBIN_H__
