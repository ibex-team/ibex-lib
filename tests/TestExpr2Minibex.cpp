/* ============================================================================
 * I B E X - Expression/Function serialization tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Oct 05, 2015
 * ---------------------------------------------------------------------------- */

#include "TestExpr2Minibex.h"
#include "ibex_Expr2Minibex.h"

#include <sstream>

using namespace std;

namespace ibex {

void TestExpr2Minibex::expr01() {

	Variable x("x"),y("y");
	const ExprNode& e=((x+1)-1)+(y*x);

	stringstream stream;

	Expr2Minibex().print(stream,e);
	CPPUNIT_ASSERT(strcmp(stream.str().c_str(),"  return (((x+1)-1)+(y*x));")==0);
}

void TestExpr2Minibex::expr02() {

	Variable x("x"),y("y");
	const ExprNode& e1=x+1;
	const ExprNode& e2=e1+y;

	const ExprNode& e=(e1*e1)-(e2+e2);

	stringstream stream;

	Expr2Minibex().print(stream,e);

	CPPUNIT_ASSERT(strcmp(stream.str().c_str(),"  _tmp_0_ = (x+1);\n  _tmp_1_ = (_tmp_0_+y);\n  return ((_tmp_0_*_tmp_0_)-(_tmp_1_+_tmp_1_));")==0);

}

void TestExpr2Minibex::expr03() {

	Variable x(Dim::row_vec(3),"x");
	IntervalVector v(3,Interval(1,2));
	const ExprConstant& c1=ExprConstant::new_vector(v,true);
	const ExprConstant& c2=ExprConstant::new_scalar(3.0);
	const ExprNode& e=(c2*(x+c1))-c2*c1;

	stringstream stream;

	Expr2Minibex().print(stream,e);

	CPPUNIT_ASSERT(strcmp(stream.str().c_str(),"  _tmp_0_ = ([1, 2] , [1, 2] , [1, 2]);\n  return ((3*(x+_tmp_0_))-(3*_tmp_0_));")==0);
}

void TestExpr2Minibex::expr04() {

	double x=0.1;
	const ExprConstant& c=ExprConstant::new_scalar(x);

	stringstream stream;

	Expr2Minibex().print(stream,c,false);

	CPPUNIT_ASSERT(strcmp(stream.str().c_str(),"  return #3fb999999999999a;")==0);

}

} // end namespace

