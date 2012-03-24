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

namespace ibex {

void TestExpr::symbol() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x");
	TEST_ASSERT(&x.context==&f);
	TEST_ASSERT(x.dim==Dim(0,0,0));
	TEST_ASSERT(x.height==0);
	TEST_ASSERT(x.id==0);
	TEST_ASSERT(x.key==0);
	TEST_ASSERT(strcmp(x.name,"x")==0);
	TEST_ASSERT(x.size==1);
	TEST_ASSERT(x.deco==NULL);
	TEST_ASSERT(!x.is_zero());
	TEST_ASSERT(x.type()==Dim::SCALAR);
}



} // end namespace
