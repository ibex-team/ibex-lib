/* ============================================================================
 * I B E X - OptimMinMAx Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 2, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_OPTIMMINMAX_H__
#define __TEST_OPTIMMINMAX_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestOptimMinMax : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestOptimMinMax);


	CPPUNIT_TEST(ex_messine);
	CPPUNIT_TEST(ex_segundo1);
	CPPUNIT_TEST(ex_segundo2);
	CPPUNIT_TEST(ex_segundo3);
	CPPUNIT_TEST(ex_segundo4);
	CPPUNIT_TEST_SUITE_END();


	void ex_messine();
	void ex_segundo1();
	void ex_segundo2();
	void ex_segundo3();
	void ex_segundo4();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestOptimMinMax);


} // namespace ibex
#endif // __TEST_OPTIMMINMAX_H__
