/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_EVAL_H__
#define __TEST_EVAL_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestEval : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestEval);
	
	CPPUNIT_TEST(deco01);
	CPPUNIT_TEST(deco02);
	CPPUNIT_TEST(dist01);
	CPPUNIT_TEST(apply01);
	CPPUNIT_TEST(apply02);
	CPPUNIT_TEST(apply03);
	CPPUNIT_TEST(apply04);
	CPPUNIT_TEST(issue242);
	CPPUNIT_TEST(eval_components01);
	CPPUNIT_TEST(eval_components02);

	CPPUNIT_TEST_SUITE_END();

	void deco01();
	void deco02();

	void dist01();

	void apply01();
	void apply02();
	void apply03();
	void apply04();

	void issue242();
	void eval_components01();
	void eval_components02();

private:
	void check_deco(Function& f, const ExprNode& e);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestEval);


} // end namespace

#endif /* __TEST_EVAL_H__ */
