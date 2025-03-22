/* ============================================================================
 * I B E X - Parser Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * Last update : May 22, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_PARSER_H__
#define __TEST_PARSER_H__

#ifndef SRCDIR_TESTS
  #define SRCDIR_TESTS "."
#endif

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestParser : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		var01();

		const_var_idx();

		ponts();
		choco01();
		func01();
		func02();
		func03();
		loop01();
		const08();
		issue245_1();
		issue245_2();
		issue245_3();
		nary_max();
		sum01();
		sum02();
		sum03();
		sum04();
		temp_in_loop();
		diff_lock();
	// requires DAGification:
	//	issue365();
		mutable_cst_1();
		mutable_cst_2();

	//			error01();
		tearDown();
	}

	void var01();

	void const_var_idx();
	// test hexadecimal constant
	void const08();

	void func01();
	void func02();
	void func03();
	void ponts();
	void choco01();
	void error01();
	void loop01();
	void issue245_1();
	void issue245_2();
	void issue245_3();
	void nary_max();

	void sum01();
	void sum02();
	void sum03();
	void sum04();

	void mutable_cst_1();
	void mutable_cst_2();

	void temp_in_loop(); // issue #380
	void diff_lock();    // see issue #365
	void issue365();
	void issue474();
};




} // end namespace

#endif // __TEST_PARSER_H__
