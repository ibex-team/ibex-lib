/* ============================================================================
 * I B E X - Expression monomials Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2020
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_MONOMIAL_H__
#define __TEST_EXPR_MONOMIAL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprMonomial : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprMonomial);
	CPPUNIT_TEST(cmp_mul_00);
	CPPUNIT_TEST(cmp_mul_01);
	CPPUNIT_TEST(cmp_mul_02);
	CPPUNIT_TEST(cmp_mul_03);
	CPPUNIT_TEST(cmp_mul_04);
	CPPUNIT_TEST(cmp_mul_05);
	CPPUNIT_TEST(cmp_mul_06);
	CPPUNIT_TEST(add_sub_01);
	CPPUNIT_TEST(add_sub_02);
	CPPUNIT_TEST(add_sub_03);
	CPPUNIT_TEST_SUITE_END();

	void cmp_mul_00();
	void cmp_mul_01();
	void cmp_mul_02();
	void cmp_mul_03();
	void cmp_mul_04();
	void cmp_mul_05();
	void cmp_mul_06();
	void add_sub_01();
	void add_sub_02();
	void add_sub_03();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprMonomial);

} // namespace ibex

#endif // __TEST_EXPR_MONOMIAL_H__
