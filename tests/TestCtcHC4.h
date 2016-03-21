//============================================================================
//                                  I B E X                                   
// File        : TestCtcHC4.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#ifndef __TEST_CTC_HC4_H__
#define __TEST_CTC_HC4_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCtcHC4 : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCtcHC4);
	
		CPPUNIT_TEST(ponts30);
	CPPUNIT_TEST_SUITE_END();

	void ponts30();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcHC4);


} // end namespace ibex
#endif // __TEST_HC4_H__
