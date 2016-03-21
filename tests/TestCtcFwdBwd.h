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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCtcFwdBwd : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCtcFwdBwd);
	CPPUNIT_TEST(sqrt_issue28);
	CPPUNIT_TEST(atan2_issue134);
	CPPUNIT_TEST_SUITE_END();

	void sqrt_issue28();
	void atan2_issue134();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcFwdBwd);


} // namespace ibex

#endif // __TEST_CTC_FWD_BWD_H__
