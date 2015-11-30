/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DOMAIN_H__
#define __TEST_DOMAIN_H__

#include "cpptest.h"
#include "ibex_BoolInterval.h"
#include "utils.h"

namespace ibex {

class TestBoolInterval : public TestIbex {

public:
	TestBoolInterval() {
		TEST_ADD(TestBoolInterval::test_constructor1);
		TEST_ADD(TestBoolInterval::test_constructor2);
		TEST_ADD(TestBoolInterval::test_constructor3);
		TEST_ADD(TestBoolInterval::test_assignement);
		TEST_ADD(TestBoolInterval::test_inter1);
		TEST_ADD(TestBoolInterval::test_inter2);
		TEST_ADD(TestBoolInterval::test_union1);
		TEST_ADD(TestBoolInterval::test_union2);
		TEST_ADD(TestBoolInterval::test_and);
		TEST_ADD(TestBoolInterval::test_or);	
	}

	void test_constructor1();
	void test_constructor2();
	void test_constructor3();
	void test_assignement();
	void test_inter1();
	void test_inter2();
	void test_union1();
	void test_union2();
	void test_and();
	void test_or();



};

} // namespace ibex
#endif // __TEST_DOMAIN_H__
