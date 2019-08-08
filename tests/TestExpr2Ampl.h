/* ============================================================================
 * I B E X -  Expression/Function serialization tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan NININ
 * Created     : Jul 05, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_2_AMPL_H__
#define __TEST_EXPR_2_AMPL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExpr2Ampl : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExpr2Ampl);
	
		CPPUNIT_TEST(expr01);
		CPPUNIT_TEST(expr02);
		CPPUNIT_TEST(expr03);
	CPPUNIT_TEST_SUITE_END();

	void expr01();
	void expr02();
	void expr03();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpr2Ampl);



} // namespace ibex
#endif // __TEST_EXPR_2_AMPL_H
