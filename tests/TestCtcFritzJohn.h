/* ============================================================================
 * I B E X - Fritz-John contractor Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 07, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_FRITZ_JOHN_H__
#define __TEST_CTC_FRITZ_JOHN_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestCtcFritzJohn : public TestIbex {

public:
	TestCtcFritzJohn() {
		TEST_ADD(TestCtcFritzJohn::test01);
	}

	void test01();
};

} // end namespace

#endif /* __TEST_CTC_FRITZ_JOHN_H__ */
