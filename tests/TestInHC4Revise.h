/* ============================================================================
 * I B E X - InHC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 3, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_IN_HC4_REVISE_H__
#define __TEST_IN_HC4_REVISE_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestInHC4Revise : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestInHC4Revise);
	
//		CPPUNIT_TEST(add01);
//		CPPUNIT_TEST(add02);
//		CPPUNIT_TEST(dist01);
//		CPPUNIT_TEST(dist02);
//		CPPUNIT_TEST(apply01);
//		CPPUNIT_TEST(apply02);
//		CPPUNIT_TEST(add_mult01);
//		CPPUNIT_TEST(bugr900);
//		CPPUNIT_TEST(issue69);
		CPPUNIT_TEST(issue70);
	CPPUNIT_TEST_SUITE_END();

	void add01();
	void add02();
	void dist01();
	void dist02();
	void apply01();
	void apply02();
	// z=x+0*y
	void add_mult01();
	void bugr900();
	void issue70();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestInHC4Revise);


} // end namespace

#endif /* __TEST_IN_HC4_REVISE_H__ */
