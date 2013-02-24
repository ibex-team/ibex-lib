/* ============================================================================
 * I B E X - NumConstraint Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_NUM_CONSTRAINT_H__
#define __TEST_NUM_CONSTRAINT_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestNumConstraint : public TestIbex {

public:
	TestNumConstraint() {
		TEST_ADD(TestNumConstraint::is_thick_eq01);
		TEST_ADD(TestNumConstraint::is_thick_eq02);
	}

	void is_thick_eq01();
	void is_thick_eq02();

};

} // end namespace

#endif /* __TEST_NUM_CONSTRAINT_H__ */

