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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprSimplify : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
		add_zero();
		sub_zero1();
		sub_zero2();
		mul_zero();
		mul_one();
		div_one();
		index_var1();
		index_var2();
		index_var3();
		index_add();
		index_transpose();
	//	issue366();
		issue425_01();
		issue425_02();
		issue425_03();
		issue425_04();
		issue425_05();

		tearDown();
	}

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
	//void issue366();
	void issue425_01();
	void issue425_02();
	void issue425_03();
	void issue425_04();
	void issue425_05();
};




} // end namespace

#endif /* __TEST_EXPR_SIMPLIFY_H__ */

