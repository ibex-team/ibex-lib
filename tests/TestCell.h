/* ============================================================================
 * I B E X - CtcCell Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 10, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CELL_H__
#define __TEST_CELL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Bxp.h"
#include "utils.h"

namespace ibex {

class BxpTest:  public Bxp {
public:
	BxpTest(): Bxp(id), n(10) { };

	Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const {
		return new BxpTest(*this);
	};

	void update(const BoxEvent& event, const BoxProperties& prop) { }

	int n;
	static long id;

protected:
	/**
	 * \brief Constructor by copy.
	 */
	explicit BxpTest(const BxpTest& e) : Bxp(id), n(e.n) { };

};

class TestCell : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCell);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(test02);
	CPPUNIT_TEST_SUITE_END();

	void test01();
	void test02();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCell);

} // namespace ibex

#endif // __TEST_CELL_H__
