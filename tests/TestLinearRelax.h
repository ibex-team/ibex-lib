/* ============================================================================
 * I B E X - Linear Relaxation Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : July, 20 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_LINEAR_RELAX_H__
#define __TEST_LINEAR_RELAX_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestLinearRelax : public TestIbex {

public:
	TestLinearRelax() {

		TEST_ADD(TestLinearRelax::test1);
	}
	void test1_AF2(IntervalVector& box) ;

	void test1();


};

} // namespace ibex
#endif // __TEST_LINEAR_RELAX_H__
