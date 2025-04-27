/* ============================================================================
 * I B E X -  Expression/Function serialization tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Oct 05, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_2_MINIBEX_H__
#define __TEST_EXPR_2_MINIBEX_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExpr2Minibex : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
			expr01();
			expr02();
			expr03();
			expr04();
		tearDown();
	}

	void expr01();
	void expr02();
	void expr03();
	void expr04();

};





} // namespace ibex
#endif // __TEST_EXPR_2_MINIBEX_H
