/* ============================================================================
 * I B E X - CtcNotIn Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 24, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTC_NOT_IN_H__
#define __TEST_CTC_NOT_IN_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ibex_CtcNotIn.h"
#include "utils.h"

namespace ibex {

class TestCtcNotIn : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCtcNotIn);
	CPPUNIT_TEST(contract01);
	CPPUNIT_TEST(contract02);
	CPPUNIT_TEST(contract03);
	CPPUNIT_TEST(contract04);
	CPPUNIT_TEST(vector01);
	CPPUNIT_TEST(vector02);
	CPPUNIT_TEST(vector03);
	CPPUNIT_TEST_SUITE_END();

	void contract01();
	void contract02();
	void contract03();
	void contract04();
	void vector01();
	void vector02();
	void vector03();

	void check_not_in(const Interval& x_input, const Interval& x_expected);

	void check_not_in(int dim, const IntervalVector& x_input, const IntervalVector& x_expected);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcNotIn);

} // namespace ibex

#endif // __TEST_CTC_NOT_IN_H__
