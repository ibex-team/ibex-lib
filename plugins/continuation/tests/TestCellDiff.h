/* ============================================================================
 * I B E X - CellDiffinuation Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CELL_DIFF_H__
#define __TEST_CELL_DIFF_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCellDiff : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCellDiff);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST_SUITE_END();

void test01();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCellDiff);


} // end namespace

#endif // __TEST_CELL_DIFF_H__
