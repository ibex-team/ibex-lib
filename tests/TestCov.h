/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 28, 2018
 * Last Update : Nov 28, 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_COV_H__
#define __TEST_COV_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

using namespace ibex;

class TestCov : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCov);
	CPPUNIT_TEST(covfac);
	CPPUNIT_TEST(covfile);
	CPPUNIT_TEST_SUITE_END();
	
private:
	void covfac();
	void covfile();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCov);

#endif // __TEST_COV__
