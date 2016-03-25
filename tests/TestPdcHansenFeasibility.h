/* ============================================================================
 * I B E X - Hansen feasibility Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov, 28 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_HANSEN_FEASIBILITY_H__
#define __TEST_HANSEN_FEASIBILITY_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestPdcHansenFeasibility : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestPdcHansenFeasibility);
	
		CPPUNIT_TEST(test01);
	CPPUNIT_TEST_SUITE_END();

	void test01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPdcHansenFeasibility);


} // end namespace

#endif // __TEST_HANSEN_FEASIBILITY_H__
