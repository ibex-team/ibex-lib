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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestNumConstraint : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestNumConstraint);

	CPPUNIT_TEST(build_from_string01);
	CPPUNIT_TEST(build_from_string02);

	CPPUNIT_TEST(build_from_file01);
	CPPUNIT_TEST(build_from_file02);
//	CPPUNIT_TEST(is_thick_eq01);
//	CPPUNIT_TEST(is_thick_eq02);
	
	CPPUNIT_TEST_SUITE_END();

	void build_from_string01();
	void build_from_string02();
	void build_from_file01();

	// check that only the first constraint in the file
	// is loaded
	void build_from_file02();
//	void is_thick_eq01();
//	void is_thick_eq02();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNumConstraint);


} // end namespace

#endif /* __TEST_NUM_CONSTRAINT_H__ */

