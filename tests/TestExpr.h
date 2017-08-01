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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExpr : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExpr);

	CPPUNIT_TEST(symbol);
	CPPUNIT_TEST(addxy01);
	CPPUNIT_TEST(addxy02);
	CPPUNIT_TEST(addxy03);
	CPPUNIT_TEST(addxy04);

	CPPUNIT_TEST(addxx01);

	CPPUNIT_TEST(mulxy01);
	CPPUNIT_TEST(mulxy02);
	CPPUNIT_TEST(mulxy03);
	CPPUNIT_TEST(mulxy04);
	CPPUNIT_TEST(mulxy05);
	CPPUNIT_TEST(mulxy06);
	CPPUNIT_TEST(mulxy07);
	CPPUNIT_TEST(mulxy08);

	CPPUNIT_TEST(dag01);

	CPPUNIT_TEST(unaryOp);
	CPPUNIT_TEST(binaryOp);

	CPPUNIT_TEST(cst01);
	CPPUNIT_TEST(cst02);
	CPPUNIT_TEST(cst03);
	CPPUNIT_TEST(cst04);
	CPPUNIT_TEST(cst05);

	CPPUNIT_TEST(vector01);
	CPPUNIT_TEST(vector02);

	CPPUNIT_TEST(index01);
	CPPUNIT_TEST(index02);
	CPPUNIT_TEST(index03);

	// deprecated, and don't work since r2.5
	// because of ExprLinearity that does not
	// manage ExprApply
//	CPPUNIT_TEST(apply01);
//	CPPUNIT_TEST(apply03);

	CPPUNIT_TEST(subnodes01);
	CPPUNIT_TEST(subnodes02);
	CPPUNIT_TEST(subnodes03);
	CPPUNIT_TEST(subnodes04);

	CPPUNIT_TEST(bug81);
	CPPUNIT_TEST_SUITE_END();

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
	void mulxy07();
	void mulxy08();

	void dag01();

	void unaryOp();
	void binaryOp();

	void cst01();
	void cst02();
	void cst03();
	void cst04();
	void cst05();

	void vector01();
	void vector02();

	void index01();
	void index02();
	void index03();
	void index04();

	void apply01();
	void apply03();

	void subnodes01();

	// variant with 2 arguments: check that variables appear in the declaration order
	// test with x+y and y+x
	void subnodes02();

	// same thing with 5 variables
	void subnodes03();

	// with constants
	void subnodes04();

	// row vector variable + (column) vector constant
	void bug81();

private:
	bool same_mask(int, int, bool*, bool**);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpr);


} // end namespace

#endif /* __TEST_EXPR_H__ */

