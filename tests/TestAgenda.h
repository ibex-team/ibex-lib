/* ============================================================================
 * I B E X - TestAgenda
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_AGENDA_H__
#define __TEST_AGENDA_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestAgenda : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestAgenda);
	CPPUNIT_TEST(test01);
	CPPUNIT_TEST(copy);
	CPPUNIT_TEST(swap);
	CPPUNIT_TEST(push01);
	CPPUNIT_TEST(pop01);
	CPPUNIT_TEST_SUITE_END();
private:

	void test01();
	void copy();
	void swap();
	void push01();
	void pop01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAgenda);

#endif // __TEST_AGENDA__
