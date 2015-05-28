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
	check_not_in(1, IntervalVector(1,x_input), IntervalVector(1,x_expected));
}

void TestCtcNotIn::check_not_in(int dim, const IntervalVector& x_input, const IntervalVector& x_expected) {

	Variable _x(dim);
	Function f(_x,_x);

	CtcNotIn* c;
	if (dim==1) c=new CtcNotIn(f,Interval(-1,1));
	else c=new CtcNotIn(f,IntervalVector(dim,Interval(-1,1)));

	IntervalVector x(x_input);

	c->contract(x);

	check(x,x_expected);

	delete c;
}

void TestCtcNotIn::contract01() {
	check_not_in(Interval(0,2),Interval(1,2));
}

void TestCtcNotIn::contract02() {
	check_not_in(Interval(-2,2),Interval(-2,2));
}

void TestCtcNotIn::contract03() {
	check_not_in(Interval(0.1,0.9),Interval::EMPTY_SET);
}

void TestCtcNotIn::contract04() {
	check_not_in(Interval(-2,0),Interval(-2,-1));
}

void TestCtcNotIn::vector01() {
	double vec[2][2]={{-0.99,0.99},{-0.99,0.99}};
	check_not_in(2,IntervalVector(2,vec),IntervalVector::empty(2));
}

void TestCtcNotIn::vector02() {
	double input[2][2]={{-0.99,0},{-2,0}};
	double expected[2][2]={{-0.99,0},{-2,-1}};
	check_not_in(2,IntervalVector(2,input),IntervalVector(2,expected));
}

void TestCtcNotIn::vector03() {
	double vec[2][2]={{-2,0},{-2,0}};
	check_not_in(2,IntervalVector(2,vec),IntervalVector(2,vec));
}

} // end namespace



