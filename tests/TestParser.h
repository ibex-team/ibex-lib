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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestParser : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestParser);

	CPPUNIT_TEST(var01);

	CPPUNIT_TEST(const_var_idx);

	CPPUNIT_TEST(ponts);
	CPPUNIT_TEST(choco01);
	CPPUNIT_TEST(func01);
	CPPUNIT_TEST(func02);
	CPPUNIT_TEST(func03);
	CPPUNIT_TEST(loop01);
	CPPUNIT_TEST(const08);
	CPPUNIT_TEST(issue245_1);
	CPPUNIT_TEST(issue245_2);
	CPPUNIT_TEST(issue245_3);
	CPPUNIT_TEST(nary_max);
	CPPUNIT_TEST(sum01);
	CPPUNIT_TEST(sum02);
	CPPUNIT_TEST(sum03);
	CPPUNIT_TEST(sum04);
	CPPUNIT_TEST(temp_in_loop);
	CPPUNIT_TEST(diff_lock);
	// requires DAGification:
	//CPPUNIT_TEST(issue365);
	CPPUNIT_TEST(mutable_cst_1);
	CPPUNIT_TEST(mutable_cst_2);

	//		CPPUNIT_TEST(error01);
	CPPUNIT_TEST_SUITE_END();

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

CPPUNIT_TEST_SUITE_REGISTRATION(TestParser);


} // end namespace

#endif // __TEST_PARSER_H__
