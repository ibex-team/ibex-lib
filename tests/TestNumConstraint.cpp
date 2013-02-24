/* ============================================================================
 * I B E X - NumConstraint Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestNumConstraint.h"
#include "ibex_NumConstraint.h"
#include "ibex_SyntaxError.h"

using namespace std;

namespace ibex {

void TestNumConstraint::is_thick_eq01() {
	Variable x,y;
	// a subtraction with no constant term is not a thick equality
	Function f1(x,y,x-y);
	NumConstraint c1(f1,EQ);
	pair<const ExprNode*,const Interval*> p=c1.is_thick_equality();

	TEST_ASSERT(p.first==NULL);

	// a "good" subtraction but with LEQ is not a thick equality
	Function f2(x,x-Interval::ZERO);
	NumConstraint c2(f2,LEQ);
	p=c2.is_thick_equality();
	TEST_ASSERT(p.first==NULL);
}

void TestNumConstraint::is_thick_eq02() {
	Variable x("x"),y("y");
	NumConstraint c1(x,y,x-y=Interval(0,1));
	pair<const ExprNode*,const Interval*> p=c1.is_thick_equality();
	TEST_ASSERT(p.first!=NULL);
	TEST_ASSERT(sameExpr(*p.first,"(x-y)"));
	TEST_ASSERT(*p.second==Interval(0,1));

	const ExprNode& cst=ExprConstant::new_scalar(Interval(0,1));
	NumConstraint c2(x,y,cst=x-y);
	p=c2.is_thick_equality();
	TEST_ASSERT(p.first!=NULL);
	TEST_ASSERT(sameExpr(*p.first,"(x-y)"));
	TEST_ASSERT(*p.second==Interval(0,1));
}


} // end namespace
