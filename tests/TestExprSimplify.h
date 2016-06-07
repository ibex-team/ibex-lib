/* ============================================================================
 * I B E X - ExprSimplify Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 7, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_SIMPLIFY_H__
#define __TEST_EXPR_SIMPLIFY_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprSimplify : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprSimplify);
	
	CPPUNIT_TEST(add_zero);
	CPPUNIT_TEST(sub_zero1);
	CPPUNIT_TEST(sub_zero2);
	CPPUNIT_TEST(mul_zero);
	CPPUNIT_TEST(mul_one);
	CPPUNIT_TEST(div_one);
	CPPUNIT_TEST(index_var);
	CPPUNIT_TEST(index_add);

	CPPUNIT_TEST_SUITE_END();

	void add_zero();
	void sub_zero1();
	void sub_zero2();
	void mul_zero();
	void mul_one();
	void div_one();
	void index_var();
	void index_add();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprSimplify);


} // end namespace

#endif /* __TEST_EXPR_SIMPLIFY_H__ */

