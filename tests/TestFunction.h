/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FUNCTION_H__
#define __TEST_FUNCTION_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestFunction : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestFunction);
	
		CPPUNIT_TEST(build01);
		CPPUNIT_TEST(add_symbol);
		CPPUNIT_TEST(copy);
		CPPUNIT_TEST(generate_comp01);
		CPPUNIT_TEST(generate_comp02);
		CPPUNIT_TEST(used);
		CPPUNIT_TEST(used02);
		CPPUNIT_TEST(numctr01);
		CPPUNIT_TEST(apply01);
		CPPUNIT_TEST(from_string01);
		CPPUNIT_TEST(from_string02);
		CPPUNIT_TEST(from_string03);
		CPPUNIT_TEST(from_string04);
		CPPUNIT_TEST(issue43);
		CPPUNIT_TEST(issue43_bis);
		CPPUNIT_TEST(minibex01);
		CPPUNIT_TEST(minibex02);
		CPPUNIT_TEST(minibex03);
	CPPUNIT_TEST_SUITE_END();

	// an uninitialized function must be deletable
	// without problem (the case happens when
	// the parser returns prematurely)
	void build01();

	void add_symbol();

	void copy();

	void generate_comp01();
	void generate_comp02();

	void used();
	void used02();

	void numctr01();

	void apply01();

	void from_string01();
	void from_string02();
	void from_string03();
	void from_string04();

	void issue43();
	void issue43_bis();
	void minibex01();
	void minibex02();
	void minibex03();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFunction);


} // end namespace

#endif /* __TEST_FUNCTION_H__ */
