/* ============================================================================
 * I B E X - Symbolic copy tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExprCopy.h"
#include "ibex_ExprCopy.h"

using namespace std;

namespace ibex {

void TestExprCopy::index_copy01() {
//	Variable x1,x2;
//
//	const Array<const ExprSymbol> ignore1(x1);
//	const Array<const ExprSymbol> ignore2(x2);
//
//	IntervalVector vec(2,Interval(0,1));
//
//	const ExprNode& y=ExprConstant::new_vector(vec,false);
//
//	const ExprNode& yi=ExprCopy().index_copy(ignore1,ignore2,y,0);
//
//	const ExprConstant* c = dynamic_cast<const ExprConstant*>(&yi);
//
//	CPPUNIT_ASSERT(c);
//	CPPUNIT_ASSERT(c->dim.is_scalar());
//	CPPUNIT_ASSERT(c->get_value()==Interval(0,1));
}

void TestExprCopy::var_component01() {
	const ExprSymbol& x=ExprSymbol::new_(Dim::matrix(3,3));
	const ExprSymbol& y=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& z=ExprSymbol::new_(Dim::row_vec(4));
	const ExprSymbol& w=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& k=ExprSymbol::new_(Dim::matrix(2,2));
	Array<const ExprSymbol> symbols(x,y,z,w,k);
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,0),x[0][0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,1),x[0][1]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,2),x[0][2]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,3),x[1][0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,4),x[1][1]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,5),x[1][2]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,6),x[2][0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,7),x[2][1]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,8),x[2][2]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,9),y));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,10),z[0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,11),z[1]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,12),z[2]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,13),z[3]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,14),w));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,15),k[0][0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,16),k[0][1]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,17),k[1][0]));
	CPPUNIT_ASSERT(sameExpr(var_component(symbols,18),k[1][1]));
}

void TestExprCopy::share() {
	ExprCopy c;
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprNode& e=x+1;
	Array<const ExprNode> a(3);
	a.set_ref(0,c.copy(x,Array<const ExprSymbol>(y),e+2,true));
	a.set_ref(1,c.copy(x,Array<const ExprSymbol>(y),e+3,true));
	a.set_ref(2,c.copy(x,Array<const ExprSymbol>(y),e+4,true));
	const ExprNode& e2=ExprVector::new_col(a);
	CPPUNIT_ASSERT(sameExpr(e2,"(((y+1)+2);((y+1)+3);((y+1)+4))"));
	CPPUNIT_ASSERT(e2.size==10);
}
} // end namespace
