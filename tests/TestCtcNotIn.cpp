//============================================================================
//                                  I B E X                                   
// File        : TestCtcNotIn.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 24, 2013
// Last Update : Feb 24, 2013
//============================================================================

#include "TestCtcNotIn.h"

using namespace std;

namespace ibex {

void TestCtcNotIn::check_not_in(const Interval& x_input, const Interval& x_expected) {

	Variable _x;
	Function f(_x,_x);

	CtcNotIn c(f,Interval(-1,1));
	IntervalVector x(1,x_input);

	try {
		c.contract(x);
	} catch(EmptyBoxException&) { }

 	TEST_ASSERT(almost_eq(x[0],x_expected,ERROR));
}

void TestCtcNotIn::contract01() {
	check_not_in(Interval(0,2),Interval(1,2));
}

void TestCtcNotIn::contract02() {
	check_not_in(Interval(-2,2),Interval(-2,2));
}

void TestCtcNotIn::contract03() {
	check_not_in(Interval(0.1,1.9),Interval::EMPTY_SET);
}

void TestCtcNotIn::contract04() {
	check_not_in(Interval(-2,0),Interval(-2,-1));
}

} // end namespace



