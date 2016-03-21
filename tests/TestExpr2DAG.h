/* ============================================================================
 * I B E X - Expression DAGification tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 22, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_2_DAG_H__
#define __TEST_EXPR_2_DAG_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExpr2DAG : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExpr2DAG);
	
		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpr2DAG);



} // namespace ibex
#endif // __TEST_EXPR_2_DAG_H__
