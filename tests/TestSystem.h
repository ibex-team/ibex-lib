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

#ifndef SRCDIR_TESTS
  #define SRCDIR_TESTS "."
#endif

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestSystem : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		empty();
		factory01();
		factory02();
		copy01();
		copy02();
		ineq_only01();
		eq_only01();
		normalize01();
		normalize02();
		extend01();
		extend02();
		merge01();
		merge02();
		merge03();
		merge04();
		mutable_cst();
		tearDown();
	}

	void empty();
	void factory01();
	void factory02();
	void copy01();
	void copy02();
	void ineq_only01();
	void eq_only01();
	void normalize01();
	void normalize02();
	void extend01();
	void extend02();
//	void extend02();
	void merge01();
	void merge02();
	void merge03();
	void merge04();
	void mutable_cst();
};




} // end namespace

#endif /* __TEST_SYSTEM_H__ */

