/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EVAL_H__
#define __TEST_EVAL_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestEval : public TestIbex {

public:
	TestEval() {
		TEST_ADD(TestEval::add01);
	}

	void add01();
};

} // end namespace

#endif /* __TEST_EVAL_H__ */
