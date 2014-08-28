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

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExprDiff : public TestIbex {

public:
	TestExprDiff() {
		TEST_ADD(TestExprDiff::linear01);
		TEST_ADD(TestExprDiff::poly01);
		TEST_ADD(TestExprDiff::vec01);
		TEST_ADD(TestExprDiff::vec02);
		TEST_ADD(TestExprDiff::vec03);
		TEST_ADD(TestExprDiff::one_var_one_func);
		TEST_ADD(TestExprDiff::mat01);
		TEST_ADD(TestExprDiff::mat02);
		TEST_ADD(TestExprDiff::apply01);
		TEST_ADD(TestExprDiff::apply02)
		TEST_ADD(TestExprDiff::apply03);
		TEST_ADD(TestExprDiff::cst_grad);
		TEST_ADD(TestExprDiff::vecimg01);
		TEST_ADD(TestExprDiff::vecimg02);
	}

	void linear01();

	void poly01();

	// x (vec) -> x[0]+x[3]
	void vec01();
	// x (vec) -> x[0]*x[3]
	void vec02();
	// x (vec) -> (x[0];x[1])[1]
	void vec03();

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
};


} // namespace ibex
#endif // __TEST_EXPR_DIFF_H__
