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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExpr : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		symbol();
		addxy01();
		addxy02();
		addxy03();
		addxy04();

		addxx01();

		mulxy01();
		mulxy02();
		mulxy03();
		mulxy04();
		mulxy05();
		mulxy06();
		mulxy07();
		mulxy08();

		dag01();

		unaryOp();
		binaryOp();

		cst01();
		cst02();
		cst03();
		cst04();
		cst05();

		vector01();
		vector02();

		index01();
		index02();
		index03();

	// deprecated, and don't work since r2.5
	// because of ExprLinearity that does not
	// manage ExprApply
//		apply01();
//		apply03();

		subnodes01();
		subnodes02();
		subnodes03();
		subnodes04();

		bug81();
		tearDown();
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




} // end namespace

#endif /* __TEST_EXPR_H__ */

