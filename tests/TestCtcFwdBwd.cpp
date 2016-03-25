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

using namespace std;

namespace ibex {

void TestCtcFwdBwd::sqrt_issue28() {
	Variable x;
	Function f(x,sqrt(x));
	NumConstraint c(f);

	CtcFwdBwd ctc(c);

	IntervalVector box(1,Interval(-2,-1));

	ctc.contract(box);

	CPPUNIT_ASSERT(box.is_empty());
}

void TestCtcFwdBwd::atan2_issue134() {
	Variable x, y, z;
	Function f(y, x, z, z - atan2(y, x));
	NumConstraint c(f);
	CtcFwdBwd ctc(c);

	IntervalVector box(3);
	box[0] = Interval(1,1);
	box[1] = Interval(0,0);
	box[2] = Interval(-100,100);
	ctc.contract(box);
	check(box[2],Interval::HALF_PI);
}


} // namespace ibex
