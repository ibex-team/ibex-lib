//============================================================================
//                                  I B E X                                   
// File        : TestLinear.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 03, 2013
// Last Update : Dec 28, 2017
//============================================================================

#ifndef __TEST_LINEAR_H__
#define __TEST_LINEAR_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Linear.h"
#include "utils.h"

namespace ibex {

class TestLinear : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestLinear);
	CPPUNIT_TEST(lu_partial_underctr);
	CPPUNIT_TEST(gauss_seidel01);
	CPPUNIT_TEST(gauss_seidel02);
	CPPUNIT_TEST(gauss_seidel03);
	CPPUNIT_TEST(inflating_gauss_seidel01);
	CPPUNIT_TEST(inflating_gauss_seidel02);
	CPPUNIT_TEST(inflating_gauss_seidel03);
	CPPUNIT_TEST(det01);
	CPPUNIT_TEST(det02);
	CPPUNIT_TEST(is_posdef_sylvester01);
	CPPUNIT_TEST(is_posdef_rohn01);
	CPPUNIT_TEST(is_diagonal_dominant01);
	CPPUNIT_TEST_SUITE_END();

	void lu_partial_underctr();
	void lu_complete_underctr();
	void lu_complete_overctr();

	// m==n (square case)
	void gauss_seidel01();
	// m<n (under-constrained)
	void gauss_seidel02();
	// m>n (over-constrained)
	void gauss_seidel03();
	// convergence, start with degenerated vector
	void inflating_gauss_seidel01();
	// convergence, start with thick vector
	void inflating_gauss_seidel02();
	// divergence, start with thick vector
	void inflating_gauss_seidel03();

	void det01();
	void det02();

	void is_posdef_sylvester01();
	void is_posdef_rohn01();
	void is_diagonal_dominant01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLinear);


} // end namespace ibex
#endif // __TEST_LINEAR_H__
