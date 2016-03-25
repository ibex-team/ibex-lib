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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestHC4Revise : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestHC4Revise);
	
		CPPUNIT_TEST(id01);
		CPPUNIT_TEST(add01);
		CPPUNIT_TEST(add02);
		CPPUNIT_TEST(add03);
		CPPUNIT_TEST(add04);
		CPPUNIT_TEST(mul01);
		CPPUNIT_TEST(min01);
		CPPUNIT_TEST(dist01);
		CPPUNIT_TEST(dist02);
	CPPUNIT_TEST_SUITE_END();
	void id01();
	void add01();
	void add02();
	void add03();
	void add04();

	void mul01();
	void min01();

	void dist01();
	void dist02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestHC4Revise);


} // end namespace

#endif /* __TEST_HC4Revise_H__ */
