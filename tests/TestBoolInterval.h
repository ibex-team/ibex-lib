/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DOMAIN_H__
#define __TEST_DOMAIN_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_BoolInterval.h"
#include "utils.h"

namespace ibex {

class TestBoolInterval : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestBoolInterval);
	
		CPPUNIT_TEST(test_constructor1);
		CPPUNIT_TEST(test_constructor2);
		CPPUNIT_TEST(test_constructor3);
		CPPUNIT_TEST(test_assignement);
		CPPUNIT_TEST(test_inter1);
		CPPUNIT_TEST(test_inter2);
		CPPUNIT_TEST(test_union1);
		CPPUNIT_TEST(test_union2);
		CPPUNIT_TEST(test_and);
		CPPUNIT_TEST(test_or);	
	CPPUNIT_TEST_SUITE_END();

	void test_constructor1();
	void test_constructor2();
	void test_constructor3();
	void test_assignement();
	void test_inter1();
	void test_inter2();
	void test_union1();
	void test_union2();
	void test_and();
	void test_or();



};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBoolInterval);


} // namespace ibex
#endif // __TEST_DOMAIN_H__
