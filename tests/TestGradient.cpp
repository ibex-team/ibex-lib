//============================================================================
//                                  I B E X                                   
// File        : TestGradient.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#include "TestGradient.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_Eval.h"
#include "ibex_BasicDecorator.h"

using namespace std;

namespace ibex {

void TestGradient::deco01() { }

void TestGradient::deco02() { }

void TestGradient::add01() {
	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	Function f(x,y,x+y);

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	IntervalVector g(2);
	f.gradient(box,g);
	TEST_ASSERT(g[0]==Interval(1,1));
	TEST_ASSERT(g[1]==Interval(1,1));
}

void TestGradient::add02() {
	Variable x,y;
	Function f(x,y,ExprVector::new_(sqr(x)-y,x*y,false));

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	IntervalMatrix J(2,2);
	f.jacobian(box,J);
	TEST_ASSERT(J[0][0]==Interval(2,4));
	TEST_ASSERT(J[0][1]==Interval(-1,-1));
	TEST_ASSERT(J[1][0]==Interval(3,4));
	TEST_ASSERT(J[1][1]==Interval(1,2));

}

void TestGradient::add03() { }
void TestGradient::add04() { }

void TestGradient::dist() {
	Variable xA,xB,yA,yB;
	Function dist(xA,xB,yA,yB,sqr(xA-xB)+sqr(yA-yB));

	IntervalVector box(4);
	box[0]=Interval(1,1);
	box[1]=Interval(2,2);
	box[2]=Interval(2,2);
	box[3]=Interval(4,4);

	IntervalVector g(4);
	dist.gradient(box,g);

	TEST_ASSERT(g[0]==Interval(-2,-2));
	TEST_ASSERT(g[1]==Interval(2,2));
	TEST_ASSERT(g[2]==Interval(-4,-4));
	TEST_ASSERT(g[3]==Interval(4,4));
}

} // end namespace

