//============================================================================
//                                  I B E X                                   
// File        : TestKernel.h
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 5, 2018
// Last Update : Mar 5, 2018
//============================================================================

#ifndef __TEST_KERNEL_H__
#define __TEST_KERNEL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestKernel : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestKernel);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST_SUITE_END();

	void test01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestKernel);


} // end namespace ibex
#endif // __TEST_KERNEL_H__
