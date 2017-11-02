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

#ifndef __TEST_SYSTEM_H__
#define __TEST_SYSTEM_H__

#ifndef SRCDIR_TESTS
  #define SRCDIR_TESTS "."
#endif

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestSystem : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestSystem);
	
		CPPUNIT_TEST(factory01);
		CPPUNIT_TEST(factory02);
		CPPUNIT_TEST(copy01);
		CPPUNIT_TEST(copy02);
		CPPUNIT_TEST(ineq_only01);
		CPPUNIT_TEST(eq_only01);
//		CPPUNIT_TEST(normalize01);
//		CPPUNIT_TEST(normalize02);
		CPPUNIT_TEST(extend01);
		CPPUNIT_TEST(extend02);
//		CPPUNIT_TEST(extend02);
		CPPUNIT_TEST(merge01);
		CPPUNIT_TEST(merge02);
		CPPUNIT_TEST(merge03);
		CPPUNIT_TEST(merge04);
	CPPUNIT_TEST_SUITE_END();

	void factory01();
	void factory02();
	void copy01();
	void copy02();
	void ineq_only01();
	void eq_only01();
	void normalize01();
	void normalize02();
	void extend01();
	void extend02();
//	void extend02();
	void merge01();
	void merge02();
	void merge03();
	void merge04();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSystem);


} // end namespace

#endif /* __TEST_SYSTEM_H__ */

