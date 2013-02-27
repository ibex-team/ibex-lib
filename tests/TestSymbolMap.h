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

#include "cpptest.h"
#include "ibex_SymbolMap.h"
#include "utils.h"

namespace ibex {

class TestSymbolMap : public TestIbex {

public:
	TestSymbolMap() {

		TEST_ADD(TestSymbolMap::test01);
		TEST_ADD(TestSymbolMap::test02);
	}

	void test01();
	void test02();
};

} // namespace ibex
#endif // __TEST_SYMBOL_MAP__
