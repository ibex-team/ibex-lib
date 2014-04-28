//============================================================================
//                                  I B E X                                   
// File        : ibex_TestLinear.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 03, 2013
// Last Update : Oct 03, 2013
//============================================================================

#ifndef __TEST_LINEAR_H__
#define __TEST_LINEAR_H__

#include "cpptest.h"
#include "ibex_Linear.h"
#include "utils.h"

namespace ibex {

class TestLinear : public TestIbex {

public:

	TestLinear() {
		TEST_ADD(TestLinear::lu_partial_underctr);
		TEST_ADD(TestLinear::inflating_gauss_seidel01);
		TEST_ADD(TestLinear::inflating_gauss_seidel02);
		TEST_ADD(TestLinear::inflating_gauss_seidel03);
	}

	void lu_partial_underctr();
	void lu_complete_underctr();
	void lu_complete_overctr();

	// convergence, start with degenerated vector
	void inflating_gauss_seidel01();
	// convergence, start with thick vector
	void inflating_gauss_seidel02();
	// divergence, start with thick vector
	void inflating_gauss_seidel03();
};

} // end namespace ibex
#endif // __TEST_LINEAR_H__
