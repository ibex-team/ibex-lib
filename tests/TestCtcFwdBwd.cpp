//============================================================================
//                                  I B E X                                   
// File        : TestCtcFwdBwd.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 21, 2014
//============================================================================

#include "TestCtcFwdBwd.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

void TestCtcFwdBwd::sqrt_issue28() {
	Variable x;
	Function f(x,sqrt(x));
	NumConstraint c(f);

	CtcFwdBwd ctc(c);

	IntervalVector box(1,Interval(-2,-1));
	TEST_THROWS_ANYTHING(ctc.contract(box)); // should raise EmptyBoxException
}

} // namespace ibex
