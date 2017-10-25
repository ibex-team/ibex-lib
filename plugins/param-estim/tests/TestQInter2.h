//============================================================================
//                                  I B E X                                   
// File        : TestQInter.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
// Last Update : Jan 02, 2015
//============================================================================

#ifndef __TEST_Q_INTER__
#define __TEST_Q_INTER__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "utils.h"

namespace ibex {

class TestQInter : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE( TestQInter );
	CPPUNIT_TEST( test_projF_1 );
	CPPUNIT_TEST( test_projF_2 );
	CPPUNIT_TEST( test_projF_3 );
	CPPUNIT_TEST_SUITE_END();

    void test_projF_1();
    void test_projF_2();
    void test_projF_3();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestQInter );

} // end namespace ibex
#endif // __TEST_Q_INTER__
