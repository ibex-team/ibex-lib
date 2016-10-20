/* ============================================================================
 * I B E X - Continuation Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_PARAMETRIC_PROOF_H__
#define __TEST_PARAMETRIC_PROOF_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestParametricProof : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestParametricProof);
	CPPUNIT_TEST(get_vars01);
	CPPUNIT_TEST(find_sol);
	CPPUNIT_TEST(half_ball_1D);
	CPPUNIT_TEST(half_ball_2D);
	CPPUNIT_TEST_SUITE_END();

	void get_vars01();
	void find_sol();
	void half_ball_1D();
	void half_ball_2D();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestParametricProof);


} // end namespace

#endif // __TEST_PARAMETRIC_PROOF_H__
