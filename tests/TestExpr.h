/* ============================================================================
 * I B E X - Expression Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_H__
#define __TEST_EXPR_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestExpr : public TestIbex {

public:
	TestExpr() {
		TEST_ADD(TestExpr::symbol);
		TEST_ADD(TestExpr::addxy01);
		TEST_ADD(TestExpr::addxy02);
		TEST_ADD(TestExpr::addxy03);
		TEST_ADD(TestExpr::addxy04);

		TEST_ADD(TestExpr::addxx01);

		TEST_ADD(TestExpr::mulxy01);
		TEST_ADD(TestExpr::mulxy02);
		TEST_ADD(TestExpr::mulxy03);
		TEST_ADD(TestExpr::mulxy04);
		TEST_ADD(TestExpr::mulxy05);
		TEST_ADD(TestExpr::mulxy06);

		TEST_ADD(TestExpr::dag01);

		TEST_ADD(TestExpr::unaryOp);
		TEST_ADD(TestExpr::binaryOp);

		TEST_ADD(TestExpr::cst01);
		TEST_ADD(TestExpr::cst02);
		TEST_ADD(TestExpr::cst03);
		TEST_ADD(TestExpr::cst04);
		TEST_ADD(TestExpr::cst05);

		TEST_ADD(TestExpr::vector);
	}

	void symbol();
	void addxy01();
	void addxy02();
	void addxy03();
	void addxy04();

	void addxx01();

	void mulxy01();
	void mulxy02();
	void mulxy03();
	void mulxy04();
	void mulxy05();
	void mulxy06();

	void dag01();

	void unaryOp();
	void binaryOp();

	void cst01();
	void cst02();
	void cst03();
	void cst04();
	void cst05();

	void vector();
};

} // end namespace

#endif /* __TEST_FUNCTION_H__ */

