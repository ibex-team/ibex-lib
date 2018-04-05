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
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::ROW_VECTOR);
	double _grad[][2] = {{2,2},{3,3}};
	IntervalVector grad(2,_grad);
	check(c->get_vector_value(),grad);
}

void TestExprDiff::poly01() {

	Variable x("x"),y("y");
	Function f(x,y,((sqr(x)+2*(x*y))+pow(y,3))+1);
	Function df(f,Function::DIFF);
	const ExprVector* v=dynamic_cast<const ExprVector*>(&df.expr());
	CPPUNIT_ASSERT(v);
	CPPUNIT_ASSERT(v->dim.type()==Dim::ROW_VECTOR);

	CPPUNIT_ASSERT(sameExpr(v->arg(0),"((2*x)+(2*y))")
			|| sameExpr(v->arg(0),"((2*y)+(2*x))")
			||sameExpr(v->arg(0),"(([2,2]*x)+(2*y))")
			|| sameExpr(v->arg(0),"((2*y)+([2,2]*x))")
			|| sameExpr(v->arg(0),"(([2,2]*y)+([2,2]*x))")
			||sameExpr(v->arg(0),"(([2,2]*y)+(2*x))")
			|| sameExpr(v->arg(0),"((2*x)+([2,2]*y))")
			|| sameExpr(v->arg(0),"(([2,2]*x)+([2,2]*y))"));
	CPPUNIT_ASSERT(sameExpr(v->arg(1),"((2*x)+(3*y^2))")
			|| sameExpr(v->arg(1),"(([2,2]*x)+([3,3]*y^2))")
			|| sameExpr(v->arg(1),"((2*x)+([3,3]*y^2))")
			|| sameExpr(v->arg(1),"(([2,2]*x)+(3*y^2))")
	        || sameExpr(v->arg(1),"((x*2)+(3*y^2))"));
}

void TestExprDiff::one_var_one_func() {
	Variable x("x");
	Function f(x,sqr(x)+1);
	Function df(f,Function::DIFF);
	CPPUNIT_ASSERT(df.expr().dim.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(sameExpr(df.expr(),"(2*x)")
			||sameExpr(df.expr(),"([2,2]*x)"));
}

void TestExprDiff::vec01() {

	Variable x(4,"x");
	Function f(x,x[0]+x[3]);
	Function df(f,Function::DIFF);

	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::ROW_VECTOR);
	double _grad[][2] = {{1,1},{0,0},{0,0},{1,1}};
	IntervalVector grad(4,_grad);

	CPPUNIT_ASSERT(c->get_vector_value()==grad);
}

void TestExprDiff::vec02() {
	Variable x(4,"x");
	Function f(x,x[0]*x[3]);
	Function df(f,Function::DIFF);
	const ExprVector* v=dynamic_cast<const ExprVector*>(&df.expr());
	CPPUNIT_ASSERT(v);
	CPPUNIT_ASSERT(v->dim.type()==Dim::ROW_VECTOR);
	CPPUNIT_ASSERT(sameExpr(v->arg(0),x[3]));
	CPPUNIT_ASSERT(sameExpr(v->arg(3),x[0]));
}

void TestExprDiff::vec03() {
	Variable x(2,"x");
	Array<const ExprNode> _vec1(x[0],x[1]);
	const ExprNode& vec1=ExprVector::new_col(_vec1);
	Function f(x,vec1[1]);
	Function df(f,Function::DIFF);

	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::ROW_VECTOR);
	double _v[][2]= {{0,0},{1,1}};
	CPPUNIT_ASSERT(c->get_vector_value()==IntervalVector(2,_v));
}

void TestExprDiff::row_vec() {
	Variable x(1,3,"x");
	Function f(x,x);
	Function df(f,Function::DIFF);

	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(c->get_matrix_value()==Matrix::eye(3));
}

void TestExprDiff::mat01() {
	Variable x(2,2,"x");
	Function f(x,x[1][0]);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c!=NULL);
	//CPPUNIT_ASSERT(c->dim.type()==Dim::MATRIX);
	double _M[][2]= {{0,0},{0,0},{1,1},{0,0}};
	//CPPUNIT_ASSERT(c->get_matrix_value()==IntervalMatrix(2,2,_M));
	CPPUNIT_ASSERT(c->get_vector_value()==IntervalVector(4,_M));
}

void TestExprDiff::mat02() {
	Variable x(2,2,"x");
	Array<const ExprNode> vec1(x[0],x[1]);
	Function f(x,vec1[1][0]);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c!=NULL);
	//CPPUNIT_ASSERT(c->dim.type()==Dim::MATRIX);
	double _M[][2]= {{0,0},{0,0},{1,1},{0,0}};
	//CPPUNIT_ASSERT(c->get_matrix_value()==IntervalMatrix(2,2,_M));
	CPPUNIT_ASSERT(c->get_vector_value()==IntervalVector(4,_M));
}

void TestExprDiff::apply01() {
	Variable x("x");
	Function f(x,sqr(x),"f");
	Function g(x,f(3*x));
	Function dg(g,Function::DIFF);
//	CPPUNIT_ASSERT(sameExpr(f.diff().expr(),"(2*x)")
//			||sameExpr(f.diff().expr(),"([2,2]*x)"));
//	CPPUNIT_ASSERT(sameExpr(dg.expr(),"(3*df((3*x)))")
//			||sameExpr(dg.expr(),"([3,3]*df((3*x)))")
//			||sameExpr(dg.expr(),"(df((3*x))*[3,3])")
//			||sameExpr(dg.expr(),"(df((3*x))*3)"));
	CPPUNIT_ASSERT(sameExpr(dg.expr(),"(18*x)"));
}

void TestExprDiff::apply02() {
	Variable x("x");
	Function f(x,sqr(x),"f");
	Function g(x,3*f(x));
	Function dg(g,Function::DIFF);
//	CPPUNIT_ASSERT(sameExpr(dg.expr(),"(df(x)*3)")
//			||sameExpr(dg.expr(),"(df(x)*[3,3])")
//			||sameExpr(dg.expr(),"(3*df(x))")
//			||sameExpr(dg.expr(),"([3,3]*df(x))"));
	CPPUNIT_ASSERT(sameExpr(dg.expr(),"(6*x)"));
}

void TestExprDiff::apply03() {
	Variable x("x"),y("y");
	Function f(x,y,x*y,"f");
	Function g(x,y,f(2*x,3*y));
	Function dg(g,Function::DIFF);

	// TODO: there are actually many different equivalent expressions of
	// the differential.
	// What should we exactly test? Probably requires expression equivalence
	// operator but this is known to be a difficult task...
	//	CPPUNIT_ASSERT(sameExpr(dg.expr(),"((2*df((2*x),(3*y))[0]),(3*df((2*x),(3*y))[1]))"));

	//CPPUNIT_ASSERT(sameExpr(dg.expr(),"((df((2*x),(3*y))[0]*2),(df((2*x),(3*y))[1]*3))"));
	CPPUNIT_ASSERT(sameExpr(dg.expr(),"((6*y),(6*x))"));

	double _box[][2]={{1,1},{2,2}};
	double _dg_box[][2]={{12,12},{6,6}};
	IntervalVector dg_box(dg.eval_vector(IntervalVector(2,_box)));
	//CPPUNIT_ASSERT(dg_box==IntervalVector(2,_dg_box));
	check(dg_box,IntervalVector(2,_dg_box));
	CPPUNIT_ASSERT(dg_box.is_superset(IntervalVector(2,_dg_box)));
}

void TestExprDiff::cst_grad() {
	Function f("x","y","z","10*(x-y)");
	Function df(f,Function::DIFF);
	const ExprNode& y=df.expr();
	const ExprConstant* c = dynamic_cast<const ExprConstant*>(&y);

	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.is_vector());
	IntervalVector v(3);
	v[0]=Interval(10,10);
	v[1]=Interval(-10,-10);
	v[2]=Interval(0,0);
	check(c->get_vector_value(),v);
	CPPUNIT_ASSERT((c->get_vector_value()).is_superset(v));
}


void TestExprDiff::vecimg01() {
	Function f("x","(x;x;x)");
	Function df(f,Function::DIFF);
	const ExprNode& y=df.expr();
	const ExprConstant* c = dynamic_cast<const ExprConstant*>(&y);
	CPPUNIT_ASSERT(c!=NULL);
	CPPUNIT_ASSERT(c->dim.is_vector());
	IntervalVector v(3,Interval(1.0));
	check(c->get_vector_value(),v);
	CPPUNIT_ASSERT((c->get_vector_value()).is_superset(v));
}

void TestExprDiff::vecimg02() {
	Function f("x","y","(x;y;-x)");
	Function df(f,Function::DIFF);
	const ExprNode& y=df.expr();

	const ExprConstant* c = dynamic_cast<const ExprConstant*>(&y);
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.is_matrix());
	double _mat[3*2] = {1,0,0,1,-1,0};
	Matrix mat(3,2,_mat);

	CPPUNIT_ASSERT(c->get_matrix_value()==mat);
}

void TestExprDiff::mul01() {
	Variable x("x");
	const ExprVector& v1=ExprVector::new_row(x,x);
	const ExprVector& v2=ExprVector::new_col(x,Interval(2,2));
	Function f(x,v1*v2);
	Function df(f,Function::DIFF);
	//CPPUNIT_ASSERT(sameExpr(df.expr(),"((x+2)+x)"));
	CPPUNIT_ASSERT(sameExpr(df.expr(),"((x+x)+2)"));
}

void TestExprDiff::mul02() {
	Variable x("x");
	const ExprVector& v1=ExprVector::new_col(x,x);
	const ExprVector& v2=ExprVector::new_col(x,Interval(2,2));
	Function f(x,transpose(v1)*v2);
	Function df(f,Function::DIFF);
	//CPPUNIT_ASSERT(sameExpr(df.expr(),"((x+2)+x)"));
	CPPUNIT_ASSERT(sameExpr(df.expr(),"((x+x)+2)"));
}

void TestExprDiff::mul03() {
	Variable x("x");
	Variable y("y");
	const ExprVector& v=ExprVector::new_col(x,y);
	const ExprConstant& M=ExprConstant::new_matrix(Matrix::eye(2));
	Function f(x,y,M*v);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(c->get_matrix_value()==Matrix::eye(2));
}

void TestExprDiff::mul04() {
	Variable x("x");
	Variable y("y");
	const ExprVector& v=ExprVector::new_row(x,y);
	const ExprConstant& M=ExprConstant::new_matrix(Matrix::eye(2));
	Function f(x,y,v*M);
	Function df(f,Function::DIFF);
	const ExprConstant* c=dynamic_cast<const ExprConstant*>(&df.expr());
	CPPUNIT_ASSERT(c);
	CPPUNIT_ASSERT(c->dim.type()==Dim::MATRIX);
	CPPUNIT_ASSERT(c->get_matrix_value()==Matrix::eye(2));
}

void TestExprDiff::apply_mul01() {
	Variable x,y;
	Function f(x,y,x,"f");

	Variable x3,y3;
	Function g(x3,y3,y3,"g");

	Variable x2("x"),y2("y");
	Function h(x2,y2,f(x2,y2)*g(x2,y2));
	Function dh(h,Function::DIFF);

	CPPUNIT_ASSERT(sameExpr(dh.expr(),"(y,x)"));
//	CPPUNIT_ASSERT(sameExpr(dh.expr(),"(((df(x,y)[0]*g(x,y))+(dg(x,y)[0]*f(x,y))),((df(x,y)[1]*g(x,y))+(dg(x,y)[1]*f(x,y))))"));
}

void TestExprDiff::apply_mul02() {
//	Variable x,y;
//	Function f(x,y,x,"f");
//
//	Variable x3,y3;
//	Function g(x3,y3,f(x3,y3),"g");
//
//	Variable x2("x"),y2("y");
//	Function h(x2,y2,g(x2,y2)g(x2,y2));
//
//	Function dg(g,Function::DIFF);
//
//	cout << "DG=" << dg << endl;
//	CPPUNIT_ASSERT(sameExpr(dh.expr(),"(((df(x,y)[0]*g(x,y))+(dg(x,y)[0]*f(x,y))),((df(x,y)[1]*g(x,y))+(dg(x,y)[1]*f(x,y))))"));
}

void TestExprDiff::issue247() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	const ExprNode& e=x+y;
	Array<const ExprNode> a(e,e,e);
	const ExprVector& v1=ExprVector::new_col(a);
	DoubleIndex idx(v1.dim,0,1,0,0);
	const ExprVector& v2=ExprVector::new_col(v1[idx],v1[idx]);
	Function f(x,y,v2[0]);
	IntervalVector g=f.gradient(IntervalVector(2));
	CPPUNIT_ASSERT(g==Vector::ones(2));
}

} // end namespace

