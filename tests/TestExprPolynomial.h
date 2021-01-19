/* ============================================================================
 * I B E X - Expression polynomials Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2020
 * Last update : Apr 29, 2020
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_POLYNOMIAL_H__
#define __TEST_EXPR_POLYNOMIAL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprPolynomial : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprPolynomial);
	CPPUNIT_TEST(test00);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST_SUITE_END();

	void test00();
	void test01();
	void test02();
	void test03();
	void test04();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprPolynomial);

} // namespace ibex

#endif // __TEST_EXPR_POLYNOMIAL_H__
