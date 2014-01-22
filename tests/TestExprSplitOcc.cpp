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


void TestExprSplitOcc::test03() {
	Variable x(2,"x");
	Function f1(x,(-x)[1]+x[0]);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(sameExpr(y2,"((-x_0_)[1]+x[0])"));
}

void TestExprSplitOcc::test04() {
	Variable x(2,"x");
	// same index node with several fathers
	const ExprNode& idx=x[0];
	Function f1(x,idx-idx);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim.is_scalar());
	TEST_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
}


void TestExprSplitOcc::test05() {
	Variable x(2,"x");
	// several occurrences of an index (but different nodes)
	Function f1(x,x[0]-x[0]);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim.is_scalar());
	TEST_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
}


void TestExprSplitOcc::test06() {
	Variable x(3,"x");
	// several occurrences of an index (but different nodes)
	Function f1(x,x[0]-x[1]+x[2]+x[0]);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(sameExpr(y2,"(((x[0]-x[1])+x[2])+x[0]_1_)"));
}

void TestExprSplitOcc::test07() {
	Variable x(3,"x");
	// several occurrences of an index (but different nodes)
	Function f1(x,(x[0]+(-x)[1])+x[1]+x[0]+x[1]);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==4);
	TEST_ASSERT(sameExpr(y2,"((((x[0]+(-x_0_)[1])+x[1])+x[0]_1_)+x[1]_1_)"));
}

} // end namespace
