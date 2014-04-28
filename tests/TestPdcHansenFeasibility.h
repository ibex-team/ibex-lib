/* ============================================================================
 * I B E X - Hansen feasibility Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov, 28 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_HANSEN_FEASIBILITY_H__
#define __TEST_HANSEN_FEASIBILITY_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestPdcHansenFeasibility : public TestIbex {

public:
	TestPdcHansenFeasibility() {
		TEST_ADD(TestPdcHansenFeasibility::test01);
	}

	void test01();
};

} // end namespace

#endif // __TEST_HANSEN_FEASIBILITY_H__
