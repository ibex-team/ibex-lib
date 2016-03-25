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

using namespace std;

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
	const ExprSymbol& x=ExprSymbol::new_("x");
	Function f(x,x);
	CPPUNIT_ASSERT(x.f==&f);
	CPPUNIT_ASSERT(x.dim==Dim::scalar());
	CPPUNIT_ASSERT(x.height==0);
	CPPUNIT_ASSERT(x.key==0);
	CPPUNIT_ASSERT(strcmp(x.name,"x")==0);
	CPPUNIT_ASSERT(x.size==1);
	//CPPUNIT_ASSERT(x.deco.d->dim==x.dim);
	CPPUNIT_ASSERT(x.fathers.size()==0);
	CPPUNIT_ASSERT(!x.is_zero());
	CPPUNIT_ASSERT(x.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(checkExpr(x,"x"));
}

void TestExpr::addxy01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprNode& e=x+y;
	Function f(x,y,e);

	CPPUNIT_ASSERT(y.id==x.id+1);
	CPPUNIT_ASSERT(x.key==0);
	CPPUNIT_ASSERT(y.key==1);
	CPPUNIT_ASSERT(strcmp(y.name,"y")==0);

	CPPUNIT_ASSERT(e.f==&f);
	CPPUNIT_ASSERT(e.dim==Dim::scalar());
	CPPUNIT_ASSERT(e.height==1);
	CPPUNIT_ASSERT(e.id==y.id+1);
	CPPUNIT_ASSERT(e.size==3);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(x.fathers.size()==1);
	CPPUNIT_ASSERT(&x.fathers[0]==&e);
	CPPUNIT_ASSERT(y.fathers.size()==1);
	CPPUNIT_ASSERT(&y.fathers[0]==&e);
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::SCALAR);

	CPPUNIT_ASSERT(checkExpr(e,"(x+y)"));
}

void TestExpr::addxy02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e=x+y;

	CPPUNIT_ASSERT(e.dim==Dim::row_vec(3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::ROW_VECTOR);
}

void TestExpr::addxy03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode& e=x+y;

	CPPUNIT_ASSERT(e.dim==Dim::col_vec(3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::COL_VECTOR);
}

void TestExpr::addxy04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(2,3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::matrix(2,3));
	const ExprNode& e=x+y;
	CPPUNIT_ASSERT(e.dim==Dim::matrix(2,3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::MATRIX);
}

void TestExpr::addxx01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& e=x+x;
	Function f(x,e);
	CPPUNIT_ASSERT(e.f==&f);
	CPPUNIT_ASSERT(e.dim==Dim::scalar());
	CPPUNIT_ASSERT(e.height==1);
	CPPUNIT_ASSERT(e.size==2);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(x.fathers.size()==2);
	CPPUNIT_ASSERT(&x.fathers[0]==&e);
	CPPUNIT_ASSERT(&x.fathers[1]==&e);
	CPPUNIT_ASSERT(e.type()==Dim::SCALAR);

	CPPUNIT_ASSERT(checkExpr(e,"(x+x)"));
}

// scalar * scalar
void TestExpr::mulxy01() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::scalar());
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::scalar());
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::SCALAR);
}


// vector * vector (inner product)
void TestExpr::mulxy02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::scalar());
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::SCALAR);
}

// matrix * matrix
void TestExpr::mulxy03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(2,3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::matrix(3,4));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::matrix(2,4));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::MATRIX);
}

// matrix * vector
void TestExpr::mulxy04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix(2,3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::col_vec(2));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::COL_VECTOR);
}

// scalar * matrix
void TestExpr::mulxy05() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::matrix(2,3));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::matrix(2,3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::MATRIX);
}

// scalar * vector
void TestExpr::mulxy06() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::row_vec(3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::ROW_VECTOR);

	CPPUNIT_ASSERT(checkExpr(e,"(x*y)"));
}

// vector * vector (outer product)
void TestExpr::mulxy07() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(2));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::matrix(2,3));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::MATRIX);
}

// vector * matrix
void TestExpr::mulxy08() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::matrix(3,4));
	const ExprNode& e=x*y;
	CPPUNIT_ASSERT(e.dim==Dim::row_vec(4));
	CPPUNIT_ASSERT(!e.is_zero());
	CPPUNIT_ASSERT(e.type()==Dim::ROW_VECTOR);
}


// (x-sqr(x+y))*sqr(x+y) + (x-sqr(x+y))
void TestExpr::dag01() {

	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::scalar());
	const ExprNode& e0=x+y;
	const ExprNode& e1=pow(e0,2);
	const ExprNode& e2=x-e1;
	const ExprNode& e3=e2*e1;
	const ExprNode& e4=e3+e2;
	Function f(x,y,e4);

	CPPUNIT_ASSERT(e1.height==2);
	CPPUNIT_ASSERT(e2.height==3);
	CPPUNIT_ASSERT(e3.height==4);
	CPPUNIT_ASSERT(e4.height==5);

	int id=x.id;
	CPPUNIT_ASSERT(y.id==id+1);
	CPPUNIT_ASSERT(e1.id==id+3);
	CPPUNIT_ASSERT(e2.id==id+4);
	CPPUNIT_ASSERT(e3.id==id+5);
	CPPUNIT_ASSERT(e4.id==id+6);

	CPPUNIT_ASSERT(e1.size==4);
	CPPUNIT_ASSERT(e2.size==5);
	CPPUNIT_ASSERT(e3.size==6);
	CPPUNIT_ASSERT(e4.size==7);


	CPPUNIT_ASSERT(x.fathers.size()==2);
	CPPUNIT_ASSERT(&x.fathers[0]==&e0);
	CPPUNIT_ASSERT(&x.fathers[1]==&e2);
	CPPUNIT_ASSERT(y.fathers.size()==1);
	CPPUNIT_ASSERT(&y.fathers[0]==&e0);
	CPPUNIT_ASSERT(e1.fathers.size()==2);
	CPPUNIT_ASSERT(&e1.fathers[0]==&e2);
	CPPUNIT_ASSERT(&e1.fathers[1]==&e3);
	CPPUNIT_ASSERT(e2.fathers.size()==2);
	CPPUNIT_ASSERT(&e2.fathers[0]==&e3);
	CPPUNIT_ASSERT(&e2.fathers[1]==&e4);
	CPPUNIT_ASSERT(e3.fathers.size()==1);
	CPPUNIT_ASSERT(&e3.fathers[0]==&e4);
	CPPUNIT_ASSERT(e4.fathers.size()==0);

	CPPUNIT_ASSERT(checkExpr(e4,"(((x-(x+y)^2)*(x+y)^2)+(x-(x+y)^2))"));
}

void TestExpr::unaryOp() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	CPPUNIT_ASSERT(checkExpr(-x,"(-x)"));
	CPPUNIT_ASSERT(checkExpr(sign(x),"sign(x)"));
	CPPUNIT_ASSERT(checkExpr(abs(x),"abs(x)"));
	CPPUNIT_ASSERT(checkExpr(pow(x,4),"x^4"));
	CPPUNIT_ASSERT(checkExpr(sqr(x),"x^2"));
	CPPUNIT_ASSERT(checkExpr(sqrt(x),"sqrt(x)"));
	CPPUNIT_ASSERT(checkExpr(exp(x),"exp(x)"));
	CPPUNIT_ASSERT(checkExpr(log(x),"log(x)"));
	CPPUNIT_ASSERT(checkExpr(cos(x),"cos(x)"));
	CPPUNIT_ASSERT(checkExpr(sin(x),"sin(x)"));
	CPPUNIT_ASSERT(checkExpr(tan(x),"tan(x)"));
	CPPUNIT_ASSERT(checkExpr(cosh(x),"cosh(x)"));
	CPPUNIT_ASSERT(checkExpr(sinh(x),"sinh(x)"));
	CPPUNIT_ASSERT(checkExpr(tanh(x),"tanh(x)"));
	CPPUNIT_ASSERT(checkExpr(acos(x),"acos(x)"));
	CPPUNIT_ASSERT(checkExpr(asin(x),"asin(x)"));
	CPPUNIT_ASSERT(checkExpr(atan(x),"atan(x)"));
	CPPUNIT_ASSERT(checkExpr(acosh(x),"acosh(x)"));
	CPPUNIT_ASSERT(checkExpr(asinh(x),"asinh(x)"));
	CPPUNIT_ASSERT(checkExpr(atanh(x),"atanh(x)"));
}

void TestExpr::binaryOp() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::scalar());
	CPPUNIT_ASSERT(checkExpr(x+y,"(x+y)"));
	CPPUNIT_ASSERT(checkExpr(x-y,"(x-y)"));
	CPPUNIT_ASSERT(checkExpr(x*y,"(x*y)"));
	CPPUNIT_ASSERT(checkExpr(x/y,"(x/y)"));
	CPPUNIT_ASSERT(checkExpr(max(x,y),"max(x,y)"));
	CPPUNIT_ASSERT(checkExpr(min(x,y),"min(x,y)"));
	CPPUNIT_ASSERT(checkExpr(atan2(x,y),"atan2(x,y)"));
}

void TestExpr::cst01() {

	const ExprConstant& c=ExprConstant::new_scalar(5.0);
	CPPUNIT_ASSERT(c.f==NULL);
	CPPUNIT_ASSERT(c.dim==Dim::scalar());
	CPPUNIT_ASSERT(c.height==0);
	CPPUNIT_ASSERT(c.size==1);
	CPPUNIT_ASSERT(!c.is_zero());
	CPPUNIT_ASSERT(c.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(c.get_value()==5.0);

	const ExprConstant& z=ExprConstant::new_scalar(0);
	CPPUNIT_ASSERT(z.is_zero());
}

void TestExpr::cst02() {
	IntervalVector v(v1());
	const ExprConstant& c1=ExprConstant::new_vector(v,false);

	CPPUNIT_ASSERT(c1.dim==Dim::col_vec(3));
	CPPUNIT_ASSERT(!c1.is_zero());
	CPPUNIT_ASSERT(c1.type()==Dim::COL_VECTOR);
	CPPUNIT_ASSERT(c1.get_vector_value()==v);
}

void TestExpr::cst03() {
	IntervalMatrix M(M1());
	const ExprConstant& c2=ExprConstant::new_matrix(M);

	CPPUNIT_ASSERT(c2.dim==Dim::matrix(2,3));
	CPPUNIT_ASSERT(!c2.is_zero());
	CPPUNIT_ASSERT(c2.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(c2.get_matrix_value()==M);

}

void TestExpr::cst04() {
	IntervalVector v(v1());
	const ExprConstant& c1=ExprConstant::new_vector(v,true);

	CPPUNIT_ASSERT(c1.dim==Dim::row_vec(3));
	CPPUNIT_ASSERT(!c1.is_zero());
	CPPUNIT_ASSERT(c1.type()==Dim::ROW_VECTOR);
	CPPUNIT_ASSERT(c1.get_vector_value()==v);
}

void TestExpr::cst05() {
	IntervalVector v(3);
	IntervalMatrix M(2,3);

	for (int j=0; j<3; j++) {
		v[j]=0;
		for (int i=0; i<2; i++) M[i][j]=0;
	}

	const ExprConstant& z1=ExprConstant::new_vector(v,false);
	const ExprConstant& z2=ExprConstant::new_matrix(M);
	CPPUNIT_ASSERT(z1.is_zero());
	CPPUNIT_ASSERT(z2.is_zero());
}

void TestExpr::vector01() {

	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprNode& e1=x+y;
	const ExprNode& e2=x+e1;
	Function f(x,y,e2);

	Array<const ExprNode> c(x,y,e1,e2);

	const ExprVector& v=ExprVector::new_(c,false);
	CPPUNIT_ASSERT(v.f==NULL);
	CPPUNIT_ASSERT(v.dim==Dim::matrix(4,3));
	CPPUNIT_ASSERT(v.height==3);
	CPPUNIT_ASSERT(!v.row_vector());
	CPPUNIT_ASSERT(v.nb_args==4);
	CPPUNIT_ASSERT(v.length()==4);
	CPPUNIT_ASSERT(v.size==5);
	CPPUNIT_ASSERT(v.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(checkExpr(v,"(x;y;(x+y);(x+(x+y)))"));
}

void TestExpr::vector02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprNode& e1=x+y;
	const ExprNode& e2=x+e1;

	Array<const ExprNode> c(x,y,e1,e2);

	const ExprVector& v=ExprVector::new_(c,true);
	CPPUNIT_ASSERT(v.dim==Dim::matrix(3,4));
	CPPUNIT_ASSERT(v.row_vector());
	CPPUNIT_ASSERT(v.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(checkExpr(v,"(x,y,(x+y),(x+(x+y)))"));
}

void TestExpr::index01() {

	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix_array(2,3,4));
	const ExprIndex& e=x[1];
	Function f(x,e);
	CPPUNIT_ASSERT(e.f==&f);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(e.dim==Dim::matrix(3,4));
	CPPUNIT_ASSERT(e.height==1);
	CPPUNIT_ASSERT(e.size==2);
	CPPUNIT_ASSERT(e.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(checkExpr(e,"x[1]"));
	CPPUNIT_ASSERT(e.indexed_symbol());
	CPPUNIT_ASSERT(e.symbol_shift().first==&x);
	CPPUNIT_ASSERT(e.symbol_shift().second==12);
}

void TestExpr::index02() {

	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix_array(2,3,4));
	const ExprIndex& e=x[1][1];
	Function f(x,e);

	CPPUNIT_ASSERT(e.f==&f);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(e.dim==Dim::row_vec(4));
	CPPUNIT_ASSERT(e.height==2);
	CPPUNIT_ASSERT(e.size==3);
	CPPUNIT_ASSERT(e.type()==Dim::ROW_VECTOR);
	CPPUNIT_ASSERT(checkExpr(e,"x[1][1]"));
	CPPUNIT_ASSERT(e.indexed_symbol());
	CPPUNIT_ASSERT(e.symbol_shift().first==&x);
	CPPUNIT_ASSERT(e.symbol_shift().second==16);
}

void TestExpr::index03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix_array(2,3,4));
	const ExprIndex& e=x[1][1][1];
	Function f(x,e);

	CPPUNIT_ASSERT(e.f==&f);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(e.height==3);
	CPPUNIT_ASSERT(e.size==4);
	CPPUNIT_ASSERT(e.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(checkExpr(e,"x[1][1][1]"));
	CPPUNIT_ASSERT(e.indexed_symbol());
	CPPUNIT_ASSERT(e.symbol_shift().first==&x);
	CPPUNIT_ASSERT(e.symbol_shift().second==17);
}

void TestExpr::index04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::matrix_array(3,3,4));
	const ExprIndex& e=x[2][1][3];
	CPPUNIT_ASSERT(e.indexed_symbol());
	CPPUNIT_ASSERT(e.symbol_shift().first==&x);
	CPPUNIT_ASSERT(e.symbol_shift().second==31);
}

void TestExpr::apply01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z",Dim::matrix(3,4));
	Function f(x,y,z,y,"func");

	CPPUNIT_ASSERT(f.nb_arg()==3);

	const ExprSymbol& x2=ExprSymbol::new_("x2");
	const ExprSymbol& y2=ExprSymbol::new_("y2",Dim::col_vec(4));
	const ExprSymbol& A2=ExprSymbol::new_("A2",Dim::matrix(3,4));

	Array<const ExprNode> args(x2, A2*y2, A2);

	const ExprApply& e=ExprApply::new_(f,args);
	Function f2(x2,y2,A2,e);

	CPPUNIT_ASSERT(f2.nb_arg()==3);
	CPPUNIT_ASSERT(e.nb_args==3);
	CPPUNIT_ASSERT(&e.arg(0)==&x2);
	CPPUNIT_ASSERT(&e.arg(2)==&A2);
	CPPUNIT_ASSERT(e.f==&f2);
	//CPPUNIT_ASSERT(e.deco.d->dim==e.dim);
	CPPUNIT_ASSERT(e.height==2);
	CPPUNIT_ASSERT(e.size==5);
	CPPUNIT_ASSERT(e.type()==Dim::COL_VECTOR);
	CPPUNIT_ASSERT(checkExpr(e,"func(x2,(A2*y2),A2)"));
}

void TestExpr::apply03() {


	const ExprSymbol& x1 = ExprSymbol::new_("x1");
	const ExprSymbol& y1 = ExprSymbol::new_("y1");
	Function f1(x1,y1,x1+y1,"f1");

	const ExprSymbol& x2 = ExprSymbol::new_("x2");
	const ExprSymbol& y2 = ExprSymbol::new_("y2");
	Function f2(x2,y2,x2*y2,"f2");


	const ExprSymbol& x3 = ExprSymbol::new_("x3");
	Function f3(x3,f1(x3,x3)-f2(x3,x3),"f3");

	const ExprNode& e = f3.expr();

	CPPUNIT_ASSERT(e.height==2);
	CPPUNIT_ASSERT(e.size==4);
	CPPUNIT_ASSERT(checkExpr(e,"(f1(x3,x3)-f2(x3,x3))"));
}

void TestExpr::subnodes01() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& __z__=ExprSymbol::new_("z",Dim::scalar());
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::scalar());

	const ExprNode& e1=pow(x+y,2);
	const ExprNode& __e1__=e1*__z__;
	const ExprNode& e2=x-e1;
	const ExprNode& e3=e2*e1;
	const ExprNode& __e2__=e3-__e1__;
	const ExprNode& e4=e3+e2;
	const ExprNode& e5=e4+__e2__;

	Function f(x,__z__,y,e5);

	ExprSubNodes nodes(e4);

	CPPUNIT_ASSERT(&nodes[0]==&e4);
	CPPUNIT_ASSERT(&nodes[1]==&e3);
	CPPUNIT_ASSERT(&nodes[2]==&e2);
	CPPUNIT_ASSERT(&nodes[3]==&e1);
	CPPUNIT_ASSERT((&nodes[5]==&x && &nodes[6]==&y) ||
			    (&nodes[5]==&y && &nodes[6]==&x));
}

void TestExpr::subnodes02() {
	const ExprSymbol& x1=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_("y",Dim::scalar());

	Array<const ExprSymbol> vars(x1,x2);
	const ExprNode& y=x1+x2;

	ExprSubNodes nodes(vars, y);

	CPPUNIT_ASSERT(nodes.size()==3);
	CPPUNIT_ASSERT(&nodes[0]==&y);
	CPPUNIT_ASSERT(&nodes[1]==&x1);
	CPPUNIT_ASSERT(&nodes[2]==&x2);

	const ExprNode& e2=x2+x1;

	ExprSubNodes nodes2(vars, e2);

	CPPUNIT_ASSERT(nodes2.size()==3);
	CPPUNIT_ASSERT(&nodes2[0]==&e2);
	CPPUNIT_ASSERT(&nodes2[1]==&x1);
	CPPUNIT_ASSERT(&nodes2[2]==&x2);

}

void TestExpr::subnodes03() {
	const ExprSymbol& x1=ExprSymbol::new_();
	const ExprSymbol& x2=ExprSymbol::new_();
	const ExprSymbol& x3=ExprSymbol::new_();
	const ExprSymbol& x4=ExprSymbol::new_();
	const ExprSymbol& x5=ExprSymbol::new_();

	Array<const ExprSymbol> vars(x1,x2,x3,x4,x5);
	const ExprNode& y=x3*(x4-((x2+x5)/x1));

	ExprSubNodes nodes(vars, y);

	CPPUNIT_ASSERT(nodes.size()==9);
	CPPUNIT_ASSERT(&nodes[0]==&y);

	CPPUNIT_ASSERT(&nodes[4]==&x1);
	CPPUNIT_ASSERT(&nodes[5]==&x2);
	CPPUNIT_ASSERT(&nodes[6]==&x3);
	CPPUNIT_ASSERT(&nodes[7]==&x4);
	CPPUNIT_ASSERT(&nodes[8]==&x5);
}

void TestExpr::subnodes04() {
	const ExprSymbol& x1=ExprSymbol::new_("x",Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_("y",Dim::scalar());

	Array<const ExprSymbol> vars(x1,x2);

	const ExprConstant& one=ExprConstant::new_scalar(1.0);
	const ExprConstant& two=ExprConstant::new_scalar(2.0);

	const ExprNode& y=((one+x2)+x1)+two;

	ExprSubNodes nodes(vars, y);

	CPPUNIT_ASSERT(nodes.size()==7);

	CPPUNIT_ASSERT(&nodes[0]==&y);

	CPPUNIT_ASSERT(&nodes[3]==&one);
	CPPUNIT_ASSERT(&nodes[4]==&two);
	CPPUNIT_ASSERT(&nodes[5]==&x1);
	CPPUNIT_ASSERT(&nodes[6]==&x2);
}


void TestExpr::bug81() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::row_vec(3));
	IntervalVector y(3);
	const ExprNode& z=x+y;
	CPPUNIT_ASSERT(z.dim == x.dim);
}

} // end namespace
