/* ============================================================================
 * I B E X - ExprSimplify Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 7, 2016
 * ---------------------------------------------------------------------------- */

#include "TestExprSimplify.h"
#include "ibex_Expr.h"
#include <sstream>

using namespace std;

namespace ibex {
void TestExprSimplify::add_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x+Interval::ZERO;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x"));
}

void TestExprSimplify::sub_zero1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x-Interval::ZERO;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x"));
}

void TestExprSimplify::sub_zero2() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=Interval::ZERO-x;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"-x"));
}

void TestExprSimplify::mul_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::ZERO;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"0"));
}

void TestExprSimplify::mul_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::ONE;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x"));
}

void TestExprSimplify::div_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x/Interval::ONE;
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x"));
}

void TestExprSimplify::index_var() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprNode& e=x[0];
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x(1)"));
}

void TestExprSimplify::index_add() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprNode& e=(x+y)[0];
	CPPUNIT_ASSERT(sameExpr(e.simplify(),"x(1)+y(1)"));
}

} // end namespace
