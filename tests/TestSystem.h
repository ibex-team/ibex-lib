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
		TEST_ADD(TestSystem::factory01);
		TEST_ADD(TestSystem::factory02);
		TEST_ADD(TestSystem::copy01);
		TEST_ADD(TestSystem::copy02);
		TEST_ADD(TestSystem::normalize01);
//		TEST_ADD(TestSystem::normalize02);
		TEST_ADD(TestSystem::extend01);
		TEST_ADD(TestSystem::extend02);
//		TEST_ADD(TestSystem::extend02);
		TEST_ADD(TestSystem::merge01);
		TEST_ADD(TestSystem::merge02);
		TEST_ADD(TestSystem::merge03);
		TEST_ADD(TestSystem::merge04);
	}

	void factory01();
	void factory02();
	void copy01();
	void copy02();
	void normalize01();
//	void normalize02();
	void extend01();
	void extend02();
//	void extend02();
	void merge01();
	void merge02();
	void merge03();
	void merge04();
};

} // end namespace

#endif /* __TEST_SYSTEM_H__ */

