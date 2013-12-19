/* ============================================================================
 * I B E X - Test Split occurrences
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 18, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExprSplitOcc.h"
#include "ibex_ExprSplitOcc.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestExprSplitOcc::test01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& y=x+x;
	ExprSplitOcc eso(x,y);
	const ExprNode& y2=eso.get_y();
	const Array<const ExprSymbol>& x2=eso.get_x();
	TEST_ASSERT(x2.size()==2);
	const ExprBinaryOp* sum=dynamic_cast<const ExprBinaryOp*>(&y2);
	TEST_ASSERT(sum!=NULL);
	TEST_ASSERT(&sum->left==&x2[0]);
	TEST_ASSERT(&sum->right==&x2[1]);
	TEST_ASSERT(strcmp(x2[0].name,"x_0_")==0);
	TEST_ASSERT(strcmp(x2[1].name,"x_1_")==0);
}

void TestExprSplitOcc::test02() {
	Variable x("x"),y("y");
	Function f1(x,y,(((x+y)-(pow(x,2)*y)+y)));

	ExprSplitOcc eso(f1.args(),f1.expr());
	Function f2(eso.get_x(), eso.get_y());
	TEST_ASSERT(sameExpr(f2.expr(),"(((x_0_+y_0_)-(x_1_^2*y_1_))+y_2_)"));
	TEST_ASSERT(eso.get_x().size()==5);
}
} // end namespace
