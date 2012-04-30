/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#include "TestHC4Revise.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_NumConstraint.h"
#include "ibex_HC4ReviseAlgo.h"

using namespace std;

namespace ibex {

void TestHC4Revise::add01() {
	Function f;
	const ExprSymbol& x = f.add_symbol("x");
	const ExprSymbol& y = f.add_symbol("y");
	f.set_expr(x+y);

	NumConstraint c(f,true);

	HC4Revise hc4r(c);

	double init_xy[][2]= { {1,3}, {-4,-2} };
	IntervalVector box(2,init_xy);

	hc4r.contract(box);
	//cout << box << endl;

	double res_xy[][2]= { {2,3}, {-3,-2} };
	IntervalVector box1(2,res_xy);

	//cout << hc4r.eval.f << endl;
	TEST_ASSERT(box==box1);
}

void TestHC4Revise::add02() {

}

void TestHC4Revise::add03() {

}

void TestHC4Revise::add04() {

}

void TestHC4Revise::mul01() {

}

void TestHC4Revise::dist01() {
	Function f;
	const ExprSymbol& xa = f.add_symbol("xa");
	const ExprSymbol& ya = f.add_symbol("ya");
	const ExprSymbol& xb = f.add_symbol("xb");
	const ExprSymbol& yb = f.add_symbol("yb");

	f.set_expr(sqrt(sqr(xa-xb)+sqr(ya-yb))-5.0);


	NumConstraint c(f,true);

	HC4Revise hc4r(c);

	double init_xy[][2] = { {0,10}, {-10,10},
						{1,1}, {2,2} };
	IntervalVector box(4,init_xy);

	hc4r.contract(box);

	double res_xy[][2] = { {0,6}, {-3,7},
						{1,1}, {2,2} };
	IntervalVector boxR(4,res_xy);
	check(box, boxR);
}

void TestHC4Revise::dist02() {
	Function f;
	const ExprSymbol& x = f.add_symbol("x",Dim(0,2,0));
	const ExprSymbol& y = f.add_symbol("y",Dim(0,2,0));

	f.set_expr(sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1]))-5.0);

	NumConstraint c(f,true);

	HC4Revise hc4r(c);

	double init_xy[][2] = { {0,10}, {-10,10},
						{1,1}, {2,2} };
	IntervalVector box(4,init_xy);

	hc4r.contract(box);

	double res_xy[][2] = { {0,6}, {-3,7},
						{1,1}, {2,2} };
	IntervalVector boxR(4,res_xy);
	check(box, boxR);
}

} // end namespace

