/* ============================================================================
 * I B E X - Expression DAGification tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExpr2DAG.h"
#include "ibex_Expr2DAG.h"
#include "ibex_ExprCopy.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestExpr2DAG::test01() {
	const ExprSymbol& x1=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_(Dim::scalar());

	Array<const ExprSymbol> old_x(x1,x2);
	Array<const ExprSymbol> new_x(2);
	varcopy(old_x,new_x);

	const ExprNode& e1=((x1+x2)-(x1+x2));
	const ExprNode& e2 = Expr2DAG().transform(old_x,(Array<const ExprNode> const&) new_x,e1);

	CPPUNIT_ASSERT(e1.size==5 && e2.size==4);

}

void TestExpr2DAG::test02() {
	const ExprSymbol& x1=ExprSymbol::new_(Dim::scalar());
	const ExprSymbol& x2=ExprSymbol::new_(Dim::scalar());

	Array<const ExprSymbol> old_x(x1,x2);
	Array<const ExprSymbol> new_x(2);
	varcopy(old_x,new_x);

	const ExprNode& e1=(exp(x1)-(x1+x2))*exp(x1) + (exp(x1)-(x1+x2));
	const ExprNode& e2 = Expr2DAG().transform(old_x,(Array<const ExprNode> const&) new_x,e1);

	CPPUNIT_ASSERT(e2.size==7);

}

} // end namespace
