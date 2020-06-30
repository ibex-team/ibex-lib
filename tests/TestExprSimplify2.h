/* ============================================================================
 * I B E X - ExprSimplify Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 17, 2020
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_SIMPLIFY_2_H__
#define __TEST_EXPR_SIMPLIFY_2_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprSimplify2 : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprSimplify2);
	
	CPPUNIT_TEST(add_zero);
	CPPUNIT_TEST(sub_zero1);
	CPPUNIT_TEST(sub_zero2);
	CPPUNIT_TEST(mul_zero);
	CPPUNIT_TEST(mul_one);
	CPPUNIT_TEST(div_one);
	CPPUNIT_TEST(index_var1);
	CPPUNIT_TEST(index_var2);
	CPPUNIT_TEST(index_var3);
	CPPUNIT_TEST(index_add);
	CPPUNIT_TEST(index_transpose);
	CPPUNIT_TEST(poly_01);
	CPPUNIT_TEST(poly_02);
	CPPUNIT_TEST(poly_03);
	CPPUNIT_TEST(poly_04);
	CPPUNIT_TEST(poly_05);
	CPPUNIT_TEST(poly_06);
	CPPUNIT_TEST(poly_07);
	CPPUNIT_TEST(poly_08);
	CPPUNIT_TEST(poly_09);
	CPPUNIT_TEST(poly_10);
	//CPPUNIT_TEST(issue366);
	CPPUNIT_TEST(issue425_01);
	CPPUNIT_TEST(issue425_02);
	CPPUNIT_TEST(issue425_03);
	CPPUNIT_TEST(issue425_04);
	CPPUNIT_TEST(issue425_05);

	CPPUNIT_TEST_SUITE_END();

	void add_zero();
	void sub_zero1();
	void sub_zero2();
	void mul_zero();
	void mul_one();
	void div_one();
	void index_var1();
	void index_var2();
	void index_var3();
	void index_add();
	void index_transpose();
	void poly_01();
	void poly_02();
	void poly_03();
	void poly_04();
	void poly_05();
	void poly_06();
	void poly_07();
	void poly_08();
	void poly_09();
	void poly_10();
	//void issue366();
	void issue425_01();
	void issue425_02();
	void issue425_03();
	void issue425_04();
	void issue425_05();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprSimplify2);


} // end namespace

#endif /* __TEST_EXPR_SIMPLIFY_2_H__ */

