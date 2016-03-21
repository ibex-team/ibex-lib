/* ============================================================================
 * I B E X - Test Split occurrences
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 18, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EXPR_SPLIT_OCC_H__
#define __TEST_EXPR_SPLIT_OCC_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestExprSplitOcc : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestExprSplitOcc);
	
		CPPUNIT_TEST(test01);
		CPPUNIT_TEST(test02);
		CPPUNIT_TEST(test03);
		CPPUNIT_TEST(test04);
		CPPUNIT_TEST(test05);
		CPPUNIT_TEST(test07);
	CPPUNIT_TEST_SUITE_END();

	// two occurrences of scalar var
	void test01();
	// several scalar vars
	void test02();
	// vector var: one occurrence for the whole vector, one with index
	void test03();
	// vector var: same index node with several fathers
	void test04();
	// vector var: several occurrences of an index (but different nodes)
	void test05();
	// vector var: only indices
	void test06();
	// vector var: more complex example
	void test07();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExprSplitOcc);


} // namespace ibex
#endif // __TEST_EXPR_SPLIT_OCC_H__
