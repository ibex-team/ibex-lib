/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * Last update : Jul 10, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EVAL_H__
#define __TEST_EVAL_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestEval : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
		deco01();
		deco02();
		dist01();
		apply01();
		apply02();
		apply03();
		apply04();
		issue242();
		eval_components01();
		eval_components02();
		matrix_components();

		tearDown();
	}

	void deco01();
	void deco02();

	void dist01();

	void apply01();
	void apply02();
	void apply03();
	void apply04();

	void issue242();
	void eval_components01();
	void eval_components02();

	// check in particular that the components that are not selected are not computed uselessly
	void matrix_components();

private:
	void check_deco(Function& f, const ExprNode& e);
};




} // end namespace

#endif /* __TEST_EVAL_H__ */
