/* ============================================================================
 * I B E X - Symbolic diff tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_DIFF_H__
#define __TEST_EXPR_DIFF_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "utils.h"

namespace ibex {

class TestExprDiff : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprDiff);
	CPPUNIT_TEST(linear01);
	CPPUNIT_TEST(poly01);
	CPPUNIT_TEST(vec01);
	CPPUNIT_TEST(vec02);
	CPPUNIT_TEST(vec03);
	CPPUNIT_TEST(row_vec);
	CPPUNIT_TEST(one_var_one_func);
	CPPUNIT_TEST(mat01);
	CPPUNIT_TEST(mat02);
	CPPUNIT_TEST(apply01);
	CPPUNIT_TEST(apply02);
	CPPUNIT_TEST(apply03);
	CPPUNIT_TEST(cst_grad);
	CPPUNIT_TEST(vecimg01);
	CPPUNIT_TEST(vecimg02);
	CPPUNIT_TEST(mul01);
	CPPUNIT_TEST(mul02);
	CPPUNIT_TEST(mul03);
	CPPUNIT_TEST(mul04);
	CPPUNIT_TEST(apply_mul01);
	CPPUNIT_TEST(issue247);

	CPPUNIT_TEST_SUITE_END();

	void linear01();

	void poly01();

	// x (vec) -> x[0]+x[3]
	void vec01();
	// x (vec) -> x[0]*x[3]
	void vec02();
	// x (vec) -> (x[0];x[1])[1]
	void vec03();

	// x (row vec) -> x
	void row_vec();

	// x (mat) -> x[1][0]
	void mat01();
	// x (mat) -> (x[0];x[1])[1][0]
	void mat02();

	// x -> f(3*x) with f(y)=y²
	void apply01();
	// x -> 3*f(x) with f(y)=y²
	void apply02();
	// (x,y) -> f(2*x,3*y) with f(z,k)=z*k
	void apply03();

	void one_var_one_func();

	// function with constant gradient
	void cst_grad();

	// x -> (x,x,x)
	void vecimg01();

	// (x,y) -> (x,y,x)
	void vecimg02();

	// V*V
	void mul01();

	// V'*V
	void mul02();

	// M*V
	void mul03();

	// V*M
	void mul04();

	// void (x,y) -> f(x,y)*f(y,x) with f(x,y)=x
	void apply_mul01();

	// void (x,y) -> f(x,y)*f(y,x) with f(x,y)=g(x,y) with g(x,y)=x
	void apply_mul02();

	void issue247();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprDiff);

} // namespace ibex

#endif // __TEST_EXPR_DIFF_H__
