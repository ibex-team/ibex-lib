/* ============================================================================
 * I B E X - Expression Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestExpr.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"

namespace ibex {

void TestExpr::symbol() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x");
	TEST_ASSERT(&x.context==&f);
	TEST_ASSERT(x.dim==Dim(0,0,0));
	TEST_ASSERT(x.height==0);
	TEST_ASSERT(x.id==0);
	TEST_ASSERT(x.key==0);
	TEST_ASSERT(strcmp(x.name,"x")==0);
	TEST_ASSERT(x.size==1);
	TEST_ASSERT(x.deco==NULL);
	TEST_ASSERT(!x.is_zero());
	TEST_ASSERT(x.type()==Dim::SCALAR);
}

void TestExpr::addxy01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x");
	const ExprSymbol& y=f.add_symbol("y");
	TEST_ASSERT(x.id==0);
	TEST_ASSERT(y.id==1);
	TEST_ASSERT(x.key==0);
	TEST_ASSERT(y.key==1);
	TEST_ASSERT(strcmp(y.name,"y")==0);

	const ExprNode& e=x+y;
	TEST_ASSERT(&e.context==&f);
	TEST_ASSERT(e.dim==Dim(0,0,0));
	TEST_ASSERT(e.height==1);
	TEST_ASSERT(e.id==2);
	TEST_ASSERT(e.size==3);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::SCALAR);
}

void TestExpr::addxy02() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,3));
	const ExprNode& e=x+y;
	TEST_ASSERT(e.dim==Dim(0,0,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(!e.type()==Dim::SCALAR);
}

void TestExpr::addxy03() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,2,3));
	const ExprNode& e=x+y;
	TEST_ASSERT(e.dim==Dim(0,2,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(!e.type()==Dim::SCALAR);
}

void TestExpr::addxx01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x");
	const ExprNode& e=x+x;
	TEST_ASSERT(&e.context==&f);
	TEST_ASSERT(e.dim==Dim(0,0,0));
	TEST_ASSERT(e.height==1);
	TEST_ASSERT(e.id==1);
	TEST_ASSERT(e.size==2);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::SCALAR);
}

} // end namespace
