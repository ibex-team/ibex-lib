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
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&x);
}

void TestExprSplitOcc::test02() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	Function f1(x,y,(((x+y)-(pow(x,2)*y)+y)));

	ExprSplitOcc eso(f1.args(),f1.expr());
	const Array<const ExprSymbol>& x2=eso.get_x();
	Function f2(x2, eso.get_y());
	TEST_ASSERT(sameExpr(f2.expr(),"(((x_0_+y_0_)-(x_1_^2*y_1_))+y_2_)"));
	TEST_ASSERT(x2.size()==5);
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&x);
	TEST_ASSERT(&eso.node(x2[2])==&y);
	TEST_ASSERT(&eso.node(x2[3])==&y);
	TEST_ASSERT(&eso.node(x2[4])==&y);
}


void TestExprSplitOcc::test03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));
	const ExprNode& e1=x[0];
	Function f1(x,(-x)[1]+e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(sameExpr(y2,"((-x_0_)[1]+x[0])"));
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&x);
}

void TestExprSplitOcc::test04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));
	// same index node with several fathers
	const ExprNode& e1=x[0];
	Function f1(x,e1-e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim.is_scalar());
	TEST_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&e1);
}


void TestExprSplitOcc::test05() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));

	// several occurrences of an index (but different nodes)
	const ExprNode& e1=x[0];
	Function f1(x,x[0]-e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(x2[1].dim.is_scalar());
	TEST_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&e1);
}


void TestExprSplitOcc::test06() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,3,1));

	// several occurrences of an index (but different nodes)
	const ExprNode& e1=x[0];
	Function f1(x,x[0]-x[1]+x[2]+e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==2);
	TEST_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	TEST_ASSERT(sameExpr(y2,"(((x[0]-x[1])+x[2])+x[0]_1_)"));
	TEST_ASSERT(&eso.node(x2[0])==&x);
	TEST_ASSERT(&eso.node(x2[1])==&e1);
}

void TestExprSplitOcc::test07() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,3,1));

	// several occurrences of an index (but different nodes)
	const ExprNode& e1=x[0];
	const ExprNode& e2=x[1];
	Function f1(x,(x[0]+(-x)[1])+x[1]+e1+e2);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	TEST_ASSERT(x2.size()==4);
	TEST_ASSERT(sameExpr(y2,"((((x[0]+(-x_0_)[1])+x[1])+x[0]_1_)+x[1]_1_)"));
	TEST_ASSERT(&eso.node(x2[0])==&x); // the "special node" (which is inserted first)
	TEST_ASSERT(&eso.node(x2[1])==&x);
	TEST_ASSERT(&eso.node(x2[2])==&e1);
	TEST_ASSERT(&eso.node(x2[3])==&e2);
}

} // end namespace
