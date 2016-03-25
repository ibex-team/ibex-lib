/* ============================================================================
 * I B E X - Expression comparison tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 22, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_CMP_H__
#define __TEST_EXPR_CMP_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprCmp : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprCmp);
	
		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
		CPPUNIT_TEST(test03);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
	void test03();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprCmp);



} // namespace ibex
#endif // __TEST_EXPR_CMP_H__
