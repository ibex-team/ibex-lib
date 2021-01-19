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
#include "ibex_ExprSimplify.h"
#include "ibex_Expr.h"
#include <sstream>

using namespace std;

namespace ibex {
void TestExprSimplify::add_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x+Interval::zero();
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x"));
}

void TestExprSimplify::sub_zero1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x-Interval::zero();
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x"));
}

void TestExprSimplify::sub_zero2() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=Interval::zero()-x;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"(-x)"));
}

void TestExprSimplify::mul_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::zero();
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"0"));
}

void TestExprSimplify::mul_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::one();
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x"));
}

void TestExprSimplify::div_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x/Interval::one();
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x"));
}

void TestExprSimplify::index_var1() {
	Dim d=Dim::row_vec(3);
	const ExprSymbol& x=ExprSymbol::new_("x",d);
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[0]),"x(1)"));
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[0][0]),"x(1)"));
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[DoubleIndex::all(d)][0]),"x(1)"));
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[DoubleIndex::all(d)][0]),"x(1)"));
}

void TestExprSimplify::index_var2() {
	Dim d=Dim::matrix(4,3);
	const ExprSymbol& x=ExprSymbol::new_("x",d);
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[1][0]),"x(2,1)"));
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[DoubleIndex::one_col(d,1)][0]),"x(1,2)"));
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(x[DoubleIndex::rows(d,1,3)][DoubleIndex::one_row(Dim::matrix(3,3),0)]),"x(2,:)"));
}

void TestExprSimplify::index_var3() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(3,2));
	const ExprNode& e=x[0][0];
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(1,1)"));
}

void TestExprSimplify::index_add() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e=(x+y)[0];
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"(x(1)+y(1))"));
}

void TestExprSimplify::index_transpose() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprNode& e=transpose(x)[1];
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(2)"));
}

//void TestExprSimplify::issue366() {
//	const ExprSymbol& x=ExprSymbol::new_("x");
//	const ExprNode& e0=x+1-0;
//	const ExprBinaryOp& e=(const ExprBinaryOp&) e0.simplify();
//
//	CPPUNIT_ASSERT(e.fathers.size()==0);
//	CPPUNIT_ASSERT(e.left.fathers.size()==1);
//	CPPUNIT_ASSERT(e.right.fathers.size()==1);
//}

void TestExprSimplify::issue425_01() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=v*x;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(1)"));
}

void TestExprSimplify::issue425_02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(3,4));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=v*x;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(1,:)"));
}

void TestExprSimplify::issue425_03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprNode& e=Matrix::eye(3)*x;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e[0]),"x(1)"));
}

void TestExprSimplify::issue425_04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=x*v;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(1)"));
}

void TestExprSimplify::issue425_05() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(4,3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=x*v;
	CPPUNIT_ASSERT(sameExpr(ExprSimplify().simplify(e),"x(:,1)"));
}


} // end namespace
