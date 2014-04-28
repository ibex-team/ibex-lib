/* ============================================================================
 * I B E X - Fritz-John Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 07, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FRITZ_JOHN_H__
#define __TEST_FRITZ_JOHN_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestFritzJohn : public TestIbex {

public:
	TestFritzJohn() {
		TEST_ADD(TestFritzJohn::test01);
	}

	void test01();
};

} // end namespace

#endif /* __TEST_FRITZ_JOHN_H__ */
