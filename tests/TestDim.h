/* ============================================================================
 * I B E X - Dim Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DIM_H__
#define __TEST_DIM_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Dim.h"
#include "utils.h"

namespace ibex {

class TestDim : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestDim);
	CPPUNIT_TEST(scalar);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST(test03);
	CPPUNIT_TEST(add);
	CPPUNIT_TEST(mul);
	CPPUNIT_TEST(vec);
	CPPUNIT_TEST_SUITE_END();

	void scalar();
	void test02();
	void test03();
	void add();
	void mul();
	void vec();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDim);

} // namespace ibex

#endif // __TEST_DIM_H__
