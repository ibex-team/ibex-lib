/* ============================================================================
 * I B E X - CtcInteger Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jul 20, 2012
 * Last update : oct 29, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_INTEGER_H__
#define __TEST_CTC_INTEGER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_CtcInteger.h"
#include "utils.h"

namespace ibex {

class TestCtcInteger : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCtcInteger);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST(test05);
	CPPUNIT_TEST(test06);
	CPPUNIT_TEST(test07);
	CPPUNIT_TEST(test08);
	CPPUNIT_TEST(test09);
	CPPUNIT_TEST(test10);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
	void test07();
	void test08();
	void test09();
	void test10();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcInteger);


} // namespace ibex
#endif // __TEST_CTC_INTEGER_H__
