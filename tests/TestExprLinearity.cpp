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

namespace ibex {

void TestExprLinearity::test01() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprNode& e=x;
	ExprLinearity lin(args,e);

	double _x[][2]={{1,1},{0,0},{0,0}};
	CPPUNIT_ASSERT(lin.coeffs(x)==IntervalVector(3,_x));
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
	CPPUNIT_ASSERT(lin.coeffs(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeffs(y)==IntervalVector(3,_y));
	CPPUNIT_ASSERT(lin.coeffs(e)==IntervalVector(3,_e));
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
	CPPUNIT_ASSERT(lin.coeffs(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeffs(y)==IntervalVector(3,_y));
	CPPUNIT_ASSERT(lin.coeffs(e)==IntervalVector(3,_e));
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

	CPPUNIT_ASSERT(lin.coeffs(x)==IntervalVector(3,_x));
	CPPUNIT_ASSERT(lin.coeffs(e)==IntervalVector(3,_e));
	cleanup(e,true);
}

void TestExprLinearity::test05() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	Array<const ExprSymbol> args(x,y);
	const ExprConstant& c=ExprConstant::new_scalar(0);
	const ExprNode& e1=2*x;
	const ExprNode& e=sin(x)*e1;
	ExprLinearity lin(args,e);

	double _e1[][2]={{2,2},{0,0},{0,0}};

	CPPUNIT_ASSERT(lin.coeffs(e1)==IntervalVector(3,_e1));
	CPPUNIT_ASSERT(lin.coeffs(e)==IntervalVector::empty(3));
	cleanup(e,true);
}

void TestExprLinearity::test06() {

}

void TestExprLinearity::test07;
void TestExprLinearity::test08;
void TestExprLinearity::test09;
void TestExprLinearity::test10;


} // end namespace

