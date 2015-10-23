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

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestFunction : public TestIbex {

public:
	TestFunction() {
		TEST_ADD(TestFunction::build01);
		TEST_ADD(TestFunction::add_symbol);
		TEST_ADD(TestFunction::copy);
		TEST_ADD(TestFunction::generate_comp01);
		TEST_ADD(TestFunction::generate_comp02);
		TEST_ADD(TestFunction::used);
		TEST_ADD(TestFunction::used02);
		TEST_ADD(TestFunction::numctr01);
		TEST_ADD(TestFunction::apply01);
		TEST_ADD(TestFunction::from_string01);
		TEST_ADD(TestFunction::from_string02);
		TEST_ADD(TestFunction::from_string03);
		TEST_ADD(TestFunction::from_string04);
		TEST_ADD(TestFunction::issue43);
		TEST_ADD(TestFunction::issue43_bis);
		// TEST_ADD(TestFunction::minibex01);
		// TEST_ADD(TestFunction::minibex02);
		// TEST_ADD(TestFunction::minibex03);
	}

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
	// void minibex01();
	// void minibex02();
	// void minibex03();
};

} // end namespace

#endif /* __TEST_FUNCTION_H__ */
