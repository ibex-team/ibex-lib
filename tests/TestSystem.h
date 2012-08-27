/* ============================================================================
 * I B E X - System Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_SYSTEM_H__
#define __TEST_SYSTEM_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestSystem : public TestIbex {

public:
	TestSystem() {
		TEST_ADD(TestSystem::copy01);
//		TEST_ADD(TestSystem::copy02);
//		TEST_ADD(TestSystem::normalize01);
//		TEST_ADD(TestSystem::normalize02);
//		TEST_ADD(TestSystem::extend01);
//		TEST_ADD(TestSystem::extend02);
	}

	void copy01();
//	void copy02();
//	void normalize01();
//	void normalize02();
//	void extend01();
//	void extend02();

};

} // end namespace

#endif /* __TEST_SYSTEM_H__ */

