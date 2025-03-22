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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprSimplify2 : public CppUnit::TestFixture {

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
		poly_01();
		poly_02();
		poly_03();
		poly_04();
		poly_05();
		poly_06();
		poly_07();
		poly_08();
		poly_09();
		poly_10();
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




} // end namespace

#endif /* __TEST_EXPR_SIMPLIFY_2_H__ */

