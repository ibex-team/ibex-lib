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
#include <sstream>

namespace ibex {

static IntervalVector v1() {
	double vec0[][2] = { {0,3}, {0,4}, {0,5} };
	return IntervalVector(3,vec0);
}

static IntervalMatrix M1() {
	double m[][2]={{0,1},{0,2},{0,3},
			       {-1,0},{-2,0},{-3,0}};
	return IntervalMatrix(2,3,m);
}

static bool checkExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	return strcmp(s.str().c_str(),expr)==0;
}

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
	TEST_ASSERT(checkExpr(x,"x"));
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

	TEST_ASSERT(checkExpr(e,"(x+y)"));
}

void TestExpr::addxy02() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,3));
	const ExprNode& e=x+y;
	TEST_ASSERT(e.dim==Dim(0,0,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::ROW_VECTOR);
}

void TestExpr::addxy03() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,3,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	const ExprNode& e=x+y;
	TEST_ASSERT(e.dim==Dim(0,3,0));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::COL_VECTOR);
}

void TestExpr::addxy04() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,2,3));
	const ExprNode& e=x+y;
	TEST_ASSERT(e.dim==Dim(0,2,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::MATRIX);
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

	TEST_ASSERT(checkExpr(e,"(x+x)"));
}

// scalar * scalar
void TestExpr::mulxy01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,0));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,0,0));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::SCALAR);
}


// vector * vector
void TestExpr::mulxy02() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,0,0));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::SCALAR);
}

// matrix * matrix
void TestExpr::mulxy03() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,4));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,2,4));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::MATRIX);
}

// matrix * vector
void TestExpr::mulxy04() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,2,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,2,0));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::COL_VECTOR);
}

// scalar * matrix
void TestExpr::mulxy05() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,2,3));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,2,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::MATRIX);
}

// scalar * vector
void TestExpr::mulxy06() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,3));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,0,3));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::ROW_VECTOR);

	TEST_ASSERT(checkExpr(e,"(x*y)"));
}

// 1-rowed matrix * column vector
void TestExpr::mulxy07() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,1,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,1,0));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::COL_VECTOR);
}

// row vector * 1-columned matrix
void TestExpr::mulxy08() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,1));
	const ExprNode& e=x*y;
	TEST_ASSERT(e.dim==Dim(0,0,1));
	TEST_ASSERT(!e.is_zero());
	TEST_ASSERT(e.type()==Dim::ROW_VECTOR);
}


// (x-sqr(x+y))*sqr(x+y) + (x-sqr(x+y))
void TestExpr::dag01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,0));
	const ExprNode& e1=pow(x+y,2);
	const ExprNode& e2=x-e1;
	const ExprNode& e3=e2*e1;
	const ExprNode& e4=e3+e2;

	TEST_ASSERT(e1.height==2);
	TEST_ASSERT(e2.height==3);
	TEST_ASSERT(e3.height==4);
	TEST_ASSERT(e4.height==5);

	TEST_ASSERT(x.id==0);
	TEST_ASSERT(y.id==1);
	TEST_ASSERT(e1.id==3);
	TEST_ASSERT(e2.id==4);
	TEST_ASSERT(e3.id==5);
	TEST_ASSERT(e4.id==6);

	TEST_ASSERT(e1.size==4);
	TEST_ASSERT(e2.size==5);
	TEST_ASSERT(e3.size==6);
	TEST_ASSERT(e4.size==7);

	TEST_ASSERT(checkExpr(e4,"(((x-(x+y)^2)*(x+y)^2)+(x-(x+y)^2))"));

}

void TestExpr::unaryOp() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	TEST_ASSERT(checkExpr(-x,"(-x)"));
	TEST_ASSERT(checkExpr(sign(x),"sign(x)"));
	TEST_ASSERT(checkExpr(abs(x),"abs(x)"));
	TEST_ASSERT(checkExpr(pow(x,4),"x^4"));
	TEST_ASSERT(checkExpr(sqr(x),"x^2"));
	TEST_ASSERT(checkExpr(sqrt(x),"sqrt(x)"));
	TEST_ASSERT(checkExpr(exp(x),"exp(x)"));
	TEST_ASSERT(checkExpr(log(x),"log(x)"));
	TEST_ASSERT(checkExpr(cos(x),"cos(x)"));
	TEST_ASSERT(checkExpr(sin(x),"sin(x)"));
	TEST_ASSERT(checkExpr(tan(x),"tan(x)"));
	TEST_ASSERT(checkExpr(cosh(x),"cosh(x)"));
	TEST_ASSERT(checkExpr(sinh(x),"sinh(x)"));
	TEST_ASSERT(checkExpr(tanh(x),"tanh(x)"));
	TEST_ASSERT(checkExpr(acos(x),"acos(x)"));
	TEST_ASSERT(checkExpr(asin(x),"asin(x)"));
	TEST_ASSERT(checkExpr(atan(x),"atan(x)"));
	TEST_ASSERT(checkExpr(acosh(x),"acosh(x)"));
	TEST_ASSERT(checkExpr(asinh(x),"asinh(x)"));
	TEST_ASSERT(checkExpr(atanh(x),"atanh(x)"));
}

void TestExpr::binaryOp() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,0));
	TEST_ASSERT(checkExpr(x+y,"(x+y)"));
	TEST_ASSERT(checkExpr(x-y,"(x-y)"));
	TEST_ASSERT(checkExpr(x*y,"(x*y)"));
	TEST_ASSERT(checkExpr(x/y,"(x/y)"));
	TEST_ASSERT(checkExpr(max(x,y),"max(x,y)"));
	TEST_ASSERT(checkExpr(min(x,y),"min(x,y)"));
	TEST_ASSERT(checkExpr(atan2(x,y),"atan2(x,y)"));
}

void TestExpr::cst01() {
	Function f;
	const ExprConstant& c=ExprConstant::new_scalar(f,5.0);
	TEST_ASSERT(&c.context==&f);
	TEST_ASSERT(c.dim==Dim(0,0,0));
	TEST_ASSERT(c.height==0);
	TEST_ASSERT(c.id==0);
	TEST_ASSERT(c.size==1);
	TEST_ASSERT(c.deco==NULL);
	TEST_ASSERT(!c.is_zero());
	TEST_ASSERT(c.type()==Dim::SCALAR);
	TEST_ASSERT(c.get_value()==5.0);

	const ExprConstant& z=ExprConstant::new_scalar(f,0);
	TEST_ASSERT(z.is_zero());
}

void TestExpr::cst02() {
	Function f;
	IntervalVector v(v1());
	const ExprConstant& c1=ExprConstant::new_vector(f,v,false);

	TEST_ASSERT(c1.dim==Dim(0,3,0));
	TEST_ASSERT(!c1.is_zero());
	TEST_ASSERT(c1.type()==Dim::COL_VECTOR);
	TEST_ASSERT(c1.get_vector_value()==v);
}

void TestExpr::cst03() {
	Function f;
	IntervalMatrix M(M1());
	const ExprConstant& c2=ExprConstant::new_matrix(f,M);

	TEST_ASSERT(c2.dim==Dim(0,2,3));
	TEST_ASSERT(!c2.is_zero());
	TEST_ASSERT(c2.type()==Dim::MATRIX);
	TEST_ASSERT(c2.get_matrix_value()==M);

}

void TestExpr::cst04() {
	Function f;
	IntervalVector v(v1());
	const ExprConstant& c1=ExprConstant::new_vector(f,v,true);

	TEST_ASSERT(c1.dim==Dim(0,0,3));
	TEST_ASSERT(!c1.is_zero());
	TEST_ASSERT(c1.type()==Dim::ROW_VECTOR);
	TEST_ASSERT(c1.get_vector_value()==v);
}

void TestExpr::cst05() {
	Function f;
	IntervalVector v(3);
	IntervalMatrix M(2,3);

	for (int j=0; j<3; j++) {
		v[j]=0;
		for (int i=0; i<2; i++) M[i][j]=0;
	}

	const ExprConstant& z1=ExprConstant::new_vector(f,v,false);
	const ExprConstant& z2=ExprConstant::new_matrix(f,M);
	TEST_ASSERT(z1.is_zero());
	TEST_ASSERT(z2.is_zero());
}

void TestExpr::vector01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,0,3));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,0,3));
	const ExprNode& e1=x+y;
	const ExprNode& e2=x+e1;

	const ExprNode** c=new const ExprNode*[4];
	c[0]=&x;
	c[1]=&y;
	c[2]=&e1;
	c[3]=&e2;

	const ExprVector& v=ExprVector::new_(c,4,false);
	TEST_ASSERT(&v.context==&f);
	TEST_ASSERT(v.deco==NULL);
	TEST_ASSERT(v.dim==Dim(0,4,3));
	TEST_ASSERT(v.height==3);
	TEST_ASSERT(v.id==4);
	TEST_ASSERT(!v.row_vector());
	TEST_ASSERT(v.nb_args==4);
	TEST_ASSERT(v.length()==4);
	TEST_ASSERT(v.size==5);
	TEST_ASSERT(v.type()==Dim::MATRIX);
	TEST_ASSERT(checkExpr(v,"(x,y,(x+y),(x+(x+y)))"));
}

void TestExpr::vector02() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,3,0));
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	const ExprNode& e1=x+y;
	const ExprNode& e2=x+e1;

	const ExprNode** c=new const ExprNode*[4];
	c[0]=&x;
	c[1]=&y;
	c[2]=&e1;
	c[3]=&e2;

	const ExprVector& v=ExprVector::new_(c,4,true);
	TEST_ASSERT(v.dim==Dim(0,3,4));
	TEST_ASSERT(v.row_vector());
	TEST_ASSERT(v.type()==Dim::MATRIX);
	TEST_ASSERT(checkExpr(v,"(x,y,(x+y),(x+(x+y)))"));
}

void TestExpr::index01() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(2,3,4));
	const ExprNode& e=x[1];
	TEST_ASSERT(&e.context==&f);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(e.dim==Dim(0,3,4));
	TEST_ASSERT(e.height==1);
	TEST_ASSERT(e.id==1);
	TEST_ASSERT(e.size==2);
	TEST_ASSERT(e.type()==Dim::MATRIX);
	TEST_ASSERT(checkExpr(e,"x[1]"));
}

void TestExpr::index02() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(2,3,4));
	const ExprNode& e=x[1][1];
	TEST_ASSERT(&e.context==&f);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(e.dim==Dim(0,0,4));
	TEST_ASSERT(e.height==2);
	TEST_ASSERT(e.id==2);
	TEST_ASSERT(e.size==3);
	TEST_ASSERT(e.type()==Dim::ROW_VECTOR);
	TEST_ASSERT(checkExpr(e,"x[1][1]"));
}

void TestExpr::index03() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(2,3,4));
	const ExprNode& e=x[1][1][1];
	TEST_ASSERT(&e.context==&f);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(e.height==3);
	TEST_ASSERT(e.id==3);
	TEST_ASSERT(e.size==4);
	TEST_ASSERT(e.type()==Dim::SCALAR);
	TEST_ASSERT(checkExpr(e,"x[1][1][1]"));
}

void TestExpr::apply01() {
	Function f("func");
	//const ExprSymbol& x=
	f.add_symbol("x");
	const ExprSymbol& y=f.add_symbol("y",Dim(0,3,0));
	//const ExprSymbol& z=
	f.add_symbol("z",Dim(0,3,4));

	f.set_expr(y);
	TEST_ASSERT(f.nb_symbols()==3);

	Function f2;
	const ExprSymbol& x2=f2.add_symbol("x2");
	const ExprSymbol& y2=f2.add_symbol("y2",Dim(0,4,0));
	const ExprSymbol& A2=f2.add_symbol("A2",Dim(0,3,4));

	const ExprNode* args[3] = {&x2, &(A2*y2), &A2};

	const ExprApply& e=ExprApply::new_(f,args);
	TEST_ASSERT(e.nb_args==3);
	TEST_ASSERT(&e.arg(0)==&x2);
	TEST_ASSERT(&e.arg(2)==&A2);
	TEST_ASSERT(&e.context==&f2);
	TEST_ASSERT(e.deco==NULL);
	TEST_ASSERT(e.height==2);
	TEST_ASSERT(e.id==4);
	TEST_ASSERT(e.size==5);
	TEST_ASSERT(e.type()==Dim::COL_VECTOR);
	TEST_ASSERT(checkExpr(e,"func(x2,(A2*y2),A2)"));
}

void TestExpr::apply03() {

	Function f1("f1");
	const ExprSymbol& x1 = f1.add_symbol("x1");
	const ExprSymbol& y1 = f1.add_symbol("y1");

	Function f2("f2");
	const ExprSymbol& x2 = f2.add_symbol("x2");
	const ExprSymbol& y2 = f2.add_symbol("y2");

	Function f3("f3");
	const ExprSymbol& x3 = f3.add_symbol("x3");

	f1.set_expr(x1+y1);
	f2.set_expr(x2*y2);
	f3.set_expr(f1(x3,x3)-f2(x3,x3));
	const ExprNode& e = f3.expr();

	TEST_ASSERT(e.height==2);
	TEST_ASSERT(e.id==3);
	TEST_ASSERT(e.size==4);
	cout << e << endl;
	TEST_ASSERT(checkExpr(e,"(f1(x3,x3)-f2(x3,x3))"));
}

} // end namespace
