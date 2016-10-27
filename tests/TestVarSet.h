/* ============================================================================
 * I B E X - VarSet Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 02, 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_VAR_SET_H__
#define __TEST_VAR_SET_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestVarSet : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestVarSet);

	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(test04);
	CPPUNIT_TEST(test05);
	CPPUNIT_TEST(test06);
	
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestVarSet);


} // namespace ibex
#endif // __TEST_VAR_SET_H__
