/* ============================================================================
 * I B E X - Symbol Map Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_SYMBOL_MAP_H__
#define __TEST_SYMBOL_MAP_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_SymbolMap.h"
#include "utils.h"

namespace ibex {

class TestSymbolMap : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestSymbolMap);
	

		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSymbolMap);


} // namespace ibex
#endif // __TEST_SYMBOL_MAP__
