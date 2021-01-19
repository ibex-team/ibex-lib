/* ============================================================================
 * I B E X - ExprSimplify Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 17, 2020
 * ---------------------------------------------------------------------------- */

#include "TestExprSimplify2.h"
#include "ibex_ExprSimplify2.h"
#include <sstream>

using namespace std;

namespace ibex {

void TestExprSimplify2::add_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x+Interval::zero();
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x"));
	cleanup(e2,true);
}

void TestExprSimplify2::sub_zero1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x-Interval::zero();
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x"));
	cleanup(e2,true);
}

void TestExprSimplify2::sub_zero2() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=Interval::zero()-x;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(-x)"));
	cleanup(e2,true);
}

void TestExprSimplify2::mul_zero() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::zero();
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"0"));
	cleanup(e2,true);
	delete &x;
}

void TestExprSimplify2::mul_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x*Interval::one();
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x"));
	cleanup(e2,true);
}

void TestExprSimplify2::div_one() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x/Interval::one();
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x"));
	cleanup(e2,true);
}

void TestExprSimplify2::index_var1() {
	Dim d=Dim::row_vec(3);
	const ExprSymbol& x=ExprSymbol::new_("x",d);
	const ExprNode& e2=ExprSimplify2().simplify(x[0]);
	CPPUNIT_ASSERT(sameExpr(e2,"x(1)"));
	const ExprNode& e3=ExprSimplify2().simplify(x[0][0]);
	CPPUNIT_ASSERT(sameExpr(e3,"x(1)"));
	const ExprNode& e4=ExprSimplify2().simplify(x[DoubleIndex::all(d)][0]);
	CPPUNIT_ASSERT(sameExpr(e4,"x(1)"));
	const ExprNode& e5=ExprSimplify2().simplify(x[DoubleIndex::all(d)][0]);
	CPPUNIT_ASSERT(sameExpr(e5,"x(1)"));
	cleanup(e2,false);
	cleanup(e3,false);
	cleanup(e4,false);
	cleanup(e5,true);
}

void TestExprSimplify2::index_var2() {
	Dim d=Dim::matrix(4,3);
	const ExprSymbol& x=ExprSymbol::new_("x",d);
	const ExprNode& e2=ExprSimplify2().simplify(x[1][0]);
	CPPUNIT_ASSERT(sameExpr(e2,"x(2,1)"));
	const ExprNode& e3=ExprSimplify2().simplify(x[DoubleIndex::one_col(d,1)][0]);
	CPPUNIT_ASSERT(sameExpr(e3,"x(1,2)"));
	const ExprNode& e4=ExprSimplify2().simplify(x[DoubleIndex::rows(d,1,3)][DoubleIndex::one_row(Dim::matrix(3,3),0)]);
	CPPUNIT_ASSERT(sameExpr(e4,"x(2,:)"));
	cleanup(e2,false);
	cleanup(e3,false);
	cleanup(e4,true);
}

void TestExprSimplify2::index_var3() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(3,2));
	const ExprNode& e=x[0][0];
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x(1,1)"));
	cleanup(e2,true);
}

void TestExprSimplify2::index_add() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e=(x+y)[0];
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(x(1)+y(1))"));
	cleanup(e2,true);
}

void TestExprSimplify2::index_transpose() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprNode& e=transpose(x)[1];
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x(2)"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprNode& e=x*y + y*x + x*x + y*y*y;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"((x^2+(2*(x*y)))+y^3)"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_02() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprNode& e=-(x*y)*(y)*(-x[0]);
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(x^2*y^2)"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_03() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=sin(x)-sin(x)+1;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"1"));
	cleanup(e2,true);
	delete &x;
}

void TestExprSimplify2::poly_04() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=sin(x+x)+sin(2*x)+1;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(1+(2*sin((2*x))))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_05() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& R=Return(
			Return(cos(x), -sin(x), ExprVector::ROW),
			Return(sin(x), cos(x), ExprVector::ROW),
			ExprVector::COL);

	const ExprNode& e=ExprConstant::new_vector(Vector::ones(2),true)
	* R * ExprConstant::new_vector(Vector::ones(2),false);

	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(2*cos(x))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_06() {
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(3,3));
	const ExprSymbol& B=ExprSymbol::new_("B",Dim::matrix(3,3));

	const ExprNode& e=A+B+B;

	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(A+(2*B))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_07() {
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::col_vec(3));
	const ExprSymbol& B=ExprSymbol::new_("B",Dim::col_vec(3));

	const ExprNode& e=(A+ExprConstant::new_vector(Vector::ones(3),false))+B;

	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(((1+A(1))+B(1));((1+A(2))+B(2));((1+A(3))+B(3)))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_08() {
	const ExprSymbol& u=ExprSymbol::new_("u",Dim::col_vec(3));
	const ExprSymbol& v=ExprSymbol::new_("v",Dim::col_vec(3));

	const ExprNode& e=transpose(u)*v + transpose(v)*u;

	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(2*((u)'*v))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_09() {
	const ExprSymbol& u1=ExprSymbol::new_("u1",Dim::col_vec(3));
	const ExprSymbol& u2=ExprSymbol::new_("u2",Dim::col_vec(3));
	const ExprSymbol& v1=ExprSymbol::new_("v1",Dim::col_vec(3));
	const ExprSymbol& v2=ExprSymbol::new_("v2",Dim::col_vec(3));

	const ExprNode& e=transpose(u1+u2)*(v1+v2) + transpose(v1+v2)*(u1+u2);

	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(2*(((u1+u2))'*(v1+v2)))"));
	cleanup(e2,true);
}

void TestExprSimplify2::poly_10() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprVector& e=Return(x,cos(x)+2*cos(x),-x+2*x,ExprVector::COL);
	cout << e << endl;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(x;(3*cos(x));x)"));
	cleanup(e2,true);
}

//void TestExprSimplify2::issue366() {
//	const ExprSymbol& x=ExprSymbol::new_("x");
//	const ExprNode& e0=x+1-0;
//	const ExprBinaryOp& e=(const ExprBinaryOp&) e0.simplify();
//
//	CPPUNIT_ASSERT(e.fathers.size()==0);
//	CPPUNIT_ASSERT(e.left.fathers.size()==1);
//	CPPUNIT_ASSERT(e.right.fathers.size()==1);
//}

void TestExprSimplify2::issue425_01() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=v*x;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x(1)"));
	cleanup(e2,true);
}

void TestExprSimplify2::issue425_02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(3,4));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=v*x;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(x(1,1),x(1,2),x(1,3),x(1,4))"));
	cleanup(e2,true);
}

void TestExprSimplify2::issue425_03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprNode& e=Matrix::eye(3)*x;
	const ExprNode& e2=ExprSimplify2().simplify(e[0]);
	CPPUNIT_ASSERT(sameExpr(e2,"x(1)"));
	cleanup(e2,true);
}

void TestExprSimplify2::issue425_04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=x*v;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"x(1)"));
	cleanup(e2,true);
}

void TestExprSimplify2::issue425_05() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(4,3));
	Vector v(3);
	v[0]=1; v[1]=0; v[2]=0;
	const ExprNode& e=x*v;
	const ExprNode& e2=ExprSimplify2().simplify(e);
	CPPUNIT_ASSERT(sameExpr(e2,"(x(1,1);x(2,1);x(3,1);x(4,1))"));
	cleanup(e2,true);
}

} // end namespace
