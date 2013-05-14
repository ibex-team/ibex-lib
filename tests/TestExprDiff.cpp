/* ============================================================================
 * I B E X - Symbolic diff tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExprDiff.h"
#include "ibex_ExprDiff.h"

using namespace std;

namespace ibex {

void TestExprDiff::linear01() {

	Variable x,y;
	Function f(x,y,2*x+3*y);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	TEST_ASSERT(c);
	TEST_ASSERT(c->dim.type()==Dim::ROW_VECTOR);
	double _grad[][2] = {{2,2},{3,3}};
	IntervalVector grad(2,_grad);
	TEST_ASSERT(c->get_vector_value()==grad);
}

void TestExprDiff::poly01() {

	Variable x("x"),y("y");
	Function f(x,y,((sqr(x)+2*x*y)+pow(y,3))+1);
	Function df(f,Function::DIFF);
	const ExprVector* v=dynamic_cast<const ExprVector*>(&df.expr());
	TEST_ASSERT(v);
	TEST_ASSERT(v->dim.type()==Dim::ROW_VECTOR);
	TEST_ASSERT(sameExpr(v->arg(0),"((2*x)+(2*y))")
			|| sameExpr(v->arg(0),"((2*y)+(2*x))"));
	TEST_ASSERT(sameExpr(v->arg(1),"((2*x)+(3*y^2))"));
}

void TestExprDiff::vec01() {

	Variable x(4,"x");
	Function f(x,x[0]+x[3]);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	TEST_ASSERT(c);
	TEST_ASSERT(c->dim.type()==Dim::ROW_VECTOR);
	double _grad[][2] = {{1,1},{0,0},{0,0},{1,1}};
	IntervalVector grad(4,_grad);

	TEST_ASSERT(c->get_vector_value()==grad);
}

void TestExprDiff::vec02() {
	Variable x(4,"x");
	Function f(x,x[0]*x[3]);
	Function df(f,Function::DIFF);
	const ExprVector* v=dynamic_cast<const ExprVector*>(&df.expr());
	TEST_ASSERT(v);
	TEST_ASSERT(v->dim.type()==Dim::ROW_VECTOR);
	TEST_ASSERT(sameExpr(v->arg(0),x[3]));
	TEST_ASSERT(sameExpr(v->arg(3),x[0]));
}

} // end namespace
