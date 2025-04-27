/* ============================================================================
 * I B E X - Symbolic copy tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_COPY_H__
#define __TEST_EXPR_COPY_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprCopy : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		index_copy01();
		var_component01();
		share();
		tearDown();
	}

	// case where the vector is a constant vector
	void index_copy01();
	void var_component01();
	void share();

};




} // namespace ibex
#endif // __TEST_EXPR_COPY_H__
