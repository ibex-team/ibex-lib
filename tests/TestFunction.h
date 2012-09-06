/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FUNCTION_H__
#define __TEST_FUNCTION_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestFunction : public TestIbex {

public:
	TestFunction() {
		TEST_ADD(TestFunction::build01);
		TEST_ADD(TestFunction::add_symbol);
		TEST_ADD(TestFunction::copy);
		TEST_ADD(TestFunction::separate);
		TEST_ADD(TestFunction::used);
		TEST_ADD(TestFunction::used02);
	}

	// an uninitialized function must be deletable
	// without problem (the case happens when
	// the parser returns prematurely)
	void build01();

	void add_symbol();
	void copy();
	void separate();
	void used();
	void used02();
};

} // end namespace

#endif /* __TEST_FUNCTION_H__ */
