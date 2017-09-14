/* ============================================================================
 * I B E X - System Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_AMPL_H__
#define __TEST_AMPL_H__

#ifndef SRCDIR_TESTS
  #define SRCDIR_TESTS "."
#endif

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestAmpl : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestAmpl);
	
		CPPUNIT_TEST(factory01);
		CPPUNIT_TEST(factory02);
		CPPUNIT_TEST(normalize);
		CPPUNIT_TEST(extend);
		CPPUNIT_TEST(merge);
		CPPUNIT_TEST(variable1);
		CPPUNIT_TEST(variable2);
		CPPUNIT_TEST(variable3);
	CPPUNIT_TEST_SUITE_END();

	void factory01();
	void factory02();
	void normalize();
	void extend();
	void merge();
	void variable1();
	void variable2();
	void variable3();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAmpl);


} // end namespace

#endif /* __TEST_SYSTEM_H__ */

