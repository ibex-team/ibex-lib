/* ============================================================================
 * I B E X - Expression comparison tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExprCmp.h"
#include "ibex_ExprCmp.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestExprCmp::test01() {
	const ExprSymbol& x1=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_(Dim::scalar());

	CPPUNIT_ASSERT(x1==x1);
	CPPUNIT_ASSERT((x1+1)==(x1+1));
	CPPUNIT_ASSERT((x1+x2)==(x1+x2));
	CPPUNIT_ASSERT((exp(x1+x2)-1)==(exp(x1+x2)-1));
	CPPUNIT_ASSERT((x1+x2*x1)==(x1+x2*x1));

}

void TestExprCmp::test02() {
	const ExprSymbol& x1=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_(Dim::scalar());

	CPPUNIT_ASSERT(x1!=x2);
	CPPUNIT_ASSERT(x1!=ExprConstant::new_scalar(Interval::ZERO));
	CPPUNIT_ASSERT((x1+1)!=(x1-1));
	CPPUNIT_ASSERT((x1+x2)!=(x1-x2));
	CPPUNIT_ASSERT((x1+x2*x1)!=(x1+x1*x2));

}

void TestExprCmp::test03() {
	Variable x;
	const ExprSymbol& x1=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_(Dim::scalar());

	Function f(x,x+1);
	CPPUNIT_ASSERT(f(x1)==f(x1));
	CPPUNIT_ASSERT(f(x2)!=f(x1));
}


} // end namespace
