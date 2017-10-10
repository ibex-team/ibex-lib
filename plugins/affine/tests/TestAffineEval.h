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
#include "ibex_AffineEval.h"

namespace ibex {

template <class T>
class TestAffineEval : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestAffineEval<T>);
	

	CPPUNIT_TEST(dist01);
	CPPUNIT_TEST(apply01);
	CPPUNIT_TEST(apply02);
	CPPUNIT_TEST(apply03);
	CPPUNIT_TEST(apply04);
	CPPUNIT_TEST(issue242);
	CPPUNIT_TEST(eval_components01);
	CPPUNIT_TEST(eval_components02);

	CPPUNIT_TEST_SUITE_END();


	void dist01();

	void apply01();
	void apply02();
	void apply03();
	void apply04();

	void issue242();
	void eval_components01();
	void eval_components02();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_Default>);
//CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_Other>);



#ifdef _IBEX_WITH_AFFINE_EXTENDED_

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_sAF>);

#endif


} // end namespace

#endif /* __TEST_EVAL_H__ */
