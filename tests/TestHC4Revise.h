/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 3, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_HC4_REVISE_H__
#define __TEST_HC4_REVISE_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestHC4Revise : public TestIbex {

public:
	TestHC4Revise() {
		TEST_ADD(TestHC4Revise::add01);
		TEST_ADD(TestHC4Revise::add02);
		TEST_ADD(TestHC4Revise::add03);
		TEST_ADD(TestHC4Revise::add04);
		TEST_ADD(TestHC4Revise::mul01);

		TEST_ADD(TestHC4Revise::dist01);
		TEST_ADD(TestHC4Revise::dist02);
	}

	void add01();
	void add02();
	void add03();
	void add04();

	void mul01();

	void dist01();
	void dist02();
};

} // end namespace

#endif /* __TEST_HC4Revise_H__ */
