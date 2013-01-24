/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_STRING_H__
#define __TEST_STRING_H__

#include "cpptest.h"
#include "ibex_String.h"
#include "utils.h"

namespace ibex {

class TestString : public TestIbex {

public:
	TestString() {

		TEST_ADD(TestString::test01);
		TEST_ADD(TestString::test02);
	}

	void test01();
	void test02();
};

} // namespace ibex
#endif // __TEST_STRING_H__
