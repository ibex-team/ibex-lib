/* ============================================================================
 * I B E X - Optimizer Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_OPTIMIZER_H__
#define __TEST_OPTIMIZER_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestOptimizer : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestOptimizer);
#ifndef _IBEX_WITH_NOLP_
	CPPUNIT_TEST(vec_problem01);
	CPPUNIT_TEST(vec_problem02);
	CPPUNIT_TEST(issue50_1);
	CPPUNIT_TEST(issue50_2);
	CPPUNIT_TEST(issue50_3);
	CPPUNIT_TEST(issue50_4);
	CPPUNIT_TEST(unconstrained);
#endif
	CPPUNIT_TEST_SUITE_END();

	// problem with vector variables
	void vec_problem01();
	// vector variables and vector constraints
	void vec_problem02();

	// upperbounding with goal_prec=10% will remove everything (initial loup > true minimum) --> NO_FEASIBLE_FOUND
	void issue50_1();
	// upperbounding with goal_prec=0 will make the optimizer succeed --> SUCCESS
	void issue50_2();
	// upperbounding with goal_prec=10% will make the optimizer fail (initial loup < true minimum) --> NO_FEASIBLE_FOUND
	void issue50_3();
	// upperbounding with goal_prec=0 will make the optimizer fail (initial loup < true minimum) --> INFEASIBLE
	void issue50_4();

	void unconstrained(); // issue 333 and 335
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestOptimizer);

} // namespace ibex

#endif // __TEST_OPTIMIZER_H__
