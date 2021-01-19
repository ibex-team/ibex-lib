/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * Last update : Jan 28, 2020
 * ---------------------------------------------------------------------------- */

#include "TestHC4Revise.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_NumConstraint.h"
#include "ibex_HC4Revise.h"

using namespace std;

namespace ibex {

void TestHC4Revise::id01() {
	Variable _x;
	Function f(_x,_x);
	Domain y(Dim::scalar());
	y.i()=Interval(1,POS_INFINITY);
	IntervalVector x(1,Interval(0,2));
	f.backward(y,x);
	CPPUNIT_ASSERT(x[0]==Interval(1,2));
}

void TestHC4Revise::add01() {

	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	Function f(x,y,x+y);

	double init_xy[][2]= { {1,3}, {-4,-2} };
	IntervalVector box(2,init_xy);

	Domain zero(Dim::scalar());
	zero.i()=Interval(0,0);
	f.backward(zero,box);
	//cout << box << endl;

	double res_xy[][2]= { {2,3}, {-3,-2} };
	IntervalVector box1(2,res_xy);

	//cout << hc4r.eval.f << endl;
	check(box,box1);
	CPPUNIT_ASSERT(box.is_superset(box1));
}

void TestHC4Revise::add02() {

}

void TestHC4Revise::add03() {

}

void TestHC4Revise::add04() {

}

void TestHC4Revise::min01() {
	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	Function f(x,y,min(x,y));

	double init_xy[][2]= { {0,3}, {2,4} };
	IntervalVector box(2,init_xy);

	Domain zero(Dim::scalar());
	zero.i()=Interval(1,1);
	f.backward(zero,box);
	//cout << box << endl;

	double res_xy[][2]= { {1,1}, {2,4} };
	IntervalVector box1(2,res_xy);

	CPPUNIT_ASSERT(box==box1);
}

void TestHC4Revise::mul01() {

}

void TestHC4Revise::dist01() {

	const ExprSymbol& xa = ExprSymbol::new_("xa");
	const ExprSymbol& ya = ExprSymbol::new_("ya");
	const ExprSymbol& xb = ExprSymbol::new_("xb");
	const ExprSymbol& yb = ExprSymbol::new_("yb");

	const ExprSymbol* args[4]={&xa, &ya, &xb, &yb};
	Function f(Array<const ExprSymbol>(args,4),sqrt(sqr(xa-xb)+sqr(ya-yb))-5.0);

	double init_xy[][2] = { {0,10}, {-10,10},
						{1,1}, {2,2} };
	IntervalVector box(4,init_xy);

	Domain zero(Dim::scalar());
	zero.i()=Interval(0,0);
	f.backward(zero,box);

	double res_xy[][2] = { {0,6}, {-3,7},
						{1,1}, {2,2} };
	IntervalVector boxR(4,res_xy);
	check(box, boxR);
}

void TestHC4Revise::dist02() {

	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(2));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::col_vec(2));

	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1]))-5.0);

	double init_xy[][2] = { {0,10}, {-10,10},
						{1,1}, {2,2} };
	IntervalVector box(4,init_xy);

	Domain zero(Dim::scalar());
	zero.i()=Interval(0,0);
	f.backward(zero,box);

	double res_xy[][2] = { {0,6}, {-3,7},
						{1,1}, {2,2} };
	IntervalVector boxR(4,res_xy);
	check(box, boxR);
}

void TestHC4Revise::vec01() {
	const ExprSymbol& _x = ExprSymbol::new_("x",Dim::col_vec(2));
	const ExprSymbol& _y = ExprSymbol::new_("y",Dim::col_vec(2));
	Function f(_x,_y,ExprVector::new_col(_x,_y));
	IntervalVector x(4);
	IntervalVector y{{0,1},{1,2},{2,3},{3,4}};
	f.backward(y,x);
	CPPUNIT_ASSERT(x==y);
}

void TestHC4Revise::vec02() {
	const ExprSymbol& _x = ExprSymbol::new_("x",Dim::row_vec(2));
	const ExprSymbol& _y = ExprSymbol::new_("y",Dim::row_vec(3));
	const ExprSymbol& _z = ExprSymbol::new_("z");
	const ExprSymbol& _w = ExprSymbol::new_("z",Dim::row_vec(2));
	Function f(_x,_y,_z,_w,ExprVector::new_row(Array<const ExprNode>(_x,_z,_w,_y)));
	IntervalVector x(8);
	IntervalVector y {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8}};
	IntervalVector x2{{0,1},{1,2},{5,6},{6,7},{7,8},{2,3},{3,4},{4,5}};
	f.backward(y,x);
	CPPUNIT_ASSERT(x==x2);
}

void TestHC4Revise::vec03() {
	const ExprSymbol& _x = ExprSymbol::new_("x",Dim::matrix(2,2));
	const ExprSymbol& _y = ExprSymbol::new_("y",Dim::matrix(3,2));
	const ExprSymbol& _z = ExprSymbol::new_("z",Dim::row_vec(2));
	Function f(_x,_y,_z,ExprVector::new_col(_x,_z,_y)); // y and z permuted
	IntervalVector x(12);
	IntervalMatrix y{{{0,1},{1,2}},
		{{2,3},{3,4}},
		{{4,5},{5,6}},
		{{6,7},{7,8}},
		{{8,9},{9,10}},
		{{10,11},{11,12}}};

	IntervalVector x2{{0,1},{1,2},
		{2,3},{3,4},
		{6,7},{7,8},
		{8,9},{9,10},
		{10,11},{11,12},
		{4,5},{5,6}};
	f.backward(y,x);

	f.backward(y,x);
	CPPUNIT_ASSERT(x==x2);
}

void TestHC4Revise::issue431() {
	Function f("x","(sqrt(x))");
	IntervalVector x(1,Interval(-5,0));
	f.backward(Interval::zero(), x);
	CPPUNIT_ASSERT(x[0]==Interval::zero());
}

} // end namespace

