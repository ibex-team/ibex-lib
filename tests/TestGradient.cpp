//============================================================================
//                                  I B E X                                   
// File        : TestGradient.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================


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
	//e.f.cf.print<Domain>();
	//cout << "res=" << res << endl;
	TEST_ASSERT(g[0]==Interval(1,1));
	TEST_ASSERT(g[1]==Interval(1,1));
}

void TestGradient::add02() { }
void TestGradient::add03() { }
void TestGradient::add04() { }





} // end namespace

