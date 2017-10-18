/* ============================================================================
 * I B E X - Expression Linearity Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : July 04th, 2017
 * ---------------------------------------------------------------------------- */


#include "TestExprLinearity.h"
#include "ibex_ExprLinearity.h"

using namespace std;

namespace ibex {

void TestExprLinearity::test01() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprNode& e=x;
	ExprLinearity lin(args,e);

	double _x[][2]={{1,1},{0,0},{0,0}};
	CPPUNIT_ASSERT(lin.coeff_vector(x)==IntervalVector(3,_x));
	cleanup(e,true);
}

void TestExprLinearity::test02() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprNode& e=x+2*y+1;
	ExprLinearity lin(args,e);

	double _x[][2]={{1,1},{0,0},{0,0}};
	double _y[][2]={{0,0},{1,1},{0,0}};
	double _e[][2]={{1,1},{2,2},{1,1}};
	CPPUNIT_ASSERT(lin.coeff_vector(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeff_vector(y)==IntervalVector(3,_y));
	CPPUNIT_ASSERT(lin.coeff_vector(e)==IntervalVector(3,_e));
	cleanup(e,true);
}

void TestExprLinearity::test03() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprNode& e=3*x+2*y-4*x+5*y+3;
	ExprLinearity lin(args,e);

	double _x[][2]={{1,1},{0,0},{0,0}};
	double _y[][2]={{0,0},{1,1},{0,0}};
	double _e[][2]={{-1,-1},{7,7},{3,3}};
	CPPUNIT_ASSERT(lin.coeff_vector(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeff_vector(y)==IntervalVector(3,_y));
	CPPUNIT_ASSERT(lin.coeff_vector(e)==IntervalVector(3,_e));
	cleanup(e,true);
}

void TestExprLinearity::test04() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprConstant& c=ExprConstant::new_scalar(1);
	const ExprNode& e=x+sqr(c);
	ExprLinearity lin(args,e);

	double _x[][2]={{1,1},{0,0},{0,0}};
	double _e[][2]={{1,1},{0,0},{1,1}};

	CPPUNIT_ASSERT(lin.coeff_vector(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeff_vector(e)==IntervalVector(3,_e));
	cleanup(e,true);
}

void TestExprLinearity::test05() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprConstant& c=ExprConstant::new_scalar(0);
	const ExprNode& e1=2*x;
	const ExprNode& e=sin(x)*e1+y;
	ExprLinearity lin(args,e);

	double _e1[][2]={{2,2},{0,0},{0,0}};
	double _e[][2]={{NEG_INFINITY,POS_INFINITY},{1,1},{0,0}};

	CPPUNIT_ASSERT(lin.coeff_vector(e1)==IntervalVector(3,_e1));
	CPPUNIT_ASSERT(lin.coeff_vector(e)==IntervalVector(3,_e));
	cleanup(e,true);
}

void TestExprLinearity::test06() {
	Matrix A=Matrix::rand(2,3);
	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprNode& e=A*x;
	ExprLinearity lin(args,e);
	IntervalMatrix dx=lin.coeff_matrix(x);
	CPPUNIT_ASSERT(dx.nb_rows()==3 && dx.nb_cols()==5);
	CPPUNIT_ASSERT(dx.submatrix(0,2,0,2)==Matrix::eye(3));
	CPPUNIT_ASSERT(dx.submatrix(0,2,3,4)==Matrix::zeros(3,2));
	IntervalMatrix de=lin.coeff_matrix(e);
	CPPUNIT_ASSERT(de.nb_rows()==2 && de.nb_cols()==5);
	CPPUNIT_ASSERT(de.submatrix(0,1,0,2)==A);
	CPPUNIT_ASSERT(de.submatrix(0,1,3,4)==Matrix::zeros(2,2));
	cleanup(e,true);
}

void TestExprLinearity::test07() {
	Matrix A=Matrix::rand(2,3);
	Vector b=A[1];

	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	Array<const ExprNode> array(A*x,y,b);
	const ExprVector& e=ExprVector::new_(array,ExprVector::COL);
	ExprLinearity lin(args,e);
	IntervalMatrix de=lin.coeff_matrix(e);
	CPPUNIT_ASSERT(de.nb_rows()==6 && de.nb_cols()==5);
	CPPUNIT_ASSERT(de.submatrix(0,1,0,2)==A);
	CPPUNIT_ASSERT(de.submatrix(2,2,0,2)==Matrix::zeros(1,3));
	CPPUNIT_ASSERT(de[2][3]==Interval::ONE);
	CPPUNIT_ASSERT(de[2][4]==Interval::ZERO);
	CPPUNIT_ASSERT(de.submatrix(3,5,0,3)==Matrix::zeros(3,4));
	CPPUNIT_ASSERT(de.submatrix(3,5,4,4).col(0)==b);
	cleanup(e,true);
}

void TestExprLinearity::test08() {
	Matrix A=Matrix::rand(2,3);
	Matrix A2=Matrix::rand(2,3);
	Vector b=A.col(1);
	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_(Dim::col_vec(3));
	Array<const ExprSymbol> args(x,y);

	const ExprNode& e=A*x+A2*y+b;
	ExprLinearity lin(args,e);
	IntervalMatrix de=lin.coeff_matrix(e);
	CPPUNIT_ASSERT(de.nb_rows()==2 && de.nb_cols()==7);
	CPPUNIT_ASSERT(de.submatrix(0,1,0,2)==A);
	CPPUNIT_ASSERT(de.submatrix(0,1,3,5)==A2);
	CPPUNIT_ASSERT(de.submatrix(0,1,6,6).col(0)==b);
	cleanup(e,true);
}

void TestExprLinearity::test09() {
	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));
	Array<const ExprNode> array(3*x[0]+4*x[1]-2*x[2]+1,-x[0]+2*x[1]-x[2]-2);
	const ExprVector& e=ExprVector::new_(array,ExprVector::COL);
	ExprLinearity lin(x,e);
	IntervalMatrix de=lin.coeff_matrix(e);

	double _de[8]={3,4,-2,1,-1,2,-1,-2};
	CPPUNIT_ASSERT(de.nb_rows()==2 && de.nb_cols()==4);
	CPPUNIT_ASSERT(de==Matrix(2,4,_de));
	cleanup(e,true);
}

void TestExprLinearity::test10() {
	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));
	Array<const ExprNode> array(3*x[0]+4*x[1]-2*x[2]+1,-x[0]+2*x[1]-x[2]-2);
	Matrix A=Matrix::rand(2,3);
	Vector b=A.col(1);
	const ExprNode& e=A*x + ExprVector::new_(array,ExprVector::COL) + b;
	ExprLinearity lin(x,e);
	IntervalMatrix de=lin.coeff_matrix(e);

	double _de1[6]={3,4,-2,-1,2,-1};
	double _de2[2]={1,-2};
	CPPUNIT_ASSERT(de.nb_rows()==2 && de.nb_cols()==4);
	CPPUNIT_ASSERT(almost_eq(de.submatrix(0,1,0,2),(A+Matrix(2,3,_de1)),1e-10));
	CPPUNIT_ASSERT(almost_eq(de.submatrix(0,1,3,3).col(0),b+Vector(2,_de2),1e-10));
	cleanup(e,true);
}

void TestExprLinearity::test11() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	Array<const ExprNode> vec1(x,ExprConstant::new_scalar(0),2*y);
	Array<const ExprNode> vec2(x,ExprConstant::new_scalar(0),ExprConstant::new_scalar(3));
	const ExprNode& e=ExprVector::new_row(vec1)*ExprVector::new_col(vec2);
	ExprLinearity lin(args,e);
	IntervalVector de=lin.coeff_vector(e);

	CPPUNIT_ASSERT(de[0]==Interval::ALL_REALS);
	CPPUNIT_ASSERT(de[1]==6);
	cleanup(e,true);

}


} // end namespace

