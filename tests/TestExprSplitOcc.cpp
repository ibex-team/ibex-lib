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
	CPPUNIT_ASSERT(x2.size()==2);
	const ExprBinaryOp* sum=dynamic_cast<const ExprBinaryOp*>(&y2);
	CPPUNIT_ASSERT(sum!=NULL);
	CPPUNIT_ASSERT(&sum->left==&x2[0]);
	CPPUNIT_ASSERT(&sum->right==&x2[1]);
	CPPUNIT_ASSERT(strcmp(x2[0].name,"x_0_")==0);
	CPPUNIT_ASSERT(strcmp(x2[1].name,"x_1_")==0);
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&x);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==2);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==0);
}

void TestExprSplitOcc::test02() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	Function f1(x,y,(((x+y)-(pow(x,2)*y)+y)));

	ExprSplitOcc eso(f1.args(),f1.expr());
	const Array<const ExprSymbol>& x2=eso.get_x();
	Function f2(x2, eso.get_y());
	CPPUNIT_ASSERT(sameExpr(f2.expr(),"(((x_0_+y_0_)-(x_1_^2*y_1_))+y_2_)"));
	CPPUNIT_ASSERT(x2.size()==5);
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[2])==&y);
	CPPUNIT_ASSERT(&eso.node(x2[3])==&y);
	CPPUNIT_ASSERT(&eso.node(x2[4])==&y);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==5);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==0);
	CPPUNIT_ASSERT(var[2]==1);
	CPPUNIT_ASSERT(var[3]==1);
	CPPUNIT_ASSERT(var[4]==1);
}


void TestExprSplitOcc::test03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));
	const ExprNode& e1=x[0];
	Function f1(x,(-x)[1]+e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	CPPUNIT_ASSERT(x2.size()==2);
	CPPUNIT_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	CPPUNIT_ASSERT(x2[1].dim==((const ExprNode&) x).dim);
	CPPUNIT_ASSERT(sameExpr(y2,"((-x_0_)[1]+x[0])"));
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&x);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==4);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==1);
	CPPUNIT_ASSERT(var[2]==0);
	CPPUNIT_ASSERT(var[3]==1);
}

void TestExprSplitOcc::test04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));
	// same index node with several fathers
	const ExprNode& e1=x[0];
	Function f1(x,e1-e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	CPPUNIT_ASSERT(x2.size()==2);
	CPPUNIT_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	CPPUNIT_ASSERT(x2[1].dim.is_scalar());
	CPPUNIT_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&e1);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==3);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==1);
	CPPUNIT_ASSERT(var[2]==0);
}


void TestExprSplitOcc::test05() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,2,1));

	// several occurrences of an index (but different nodes)
	const ExprNode& e1=x[0];
	Function f1(x,x[0]-e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	CPPUNIT_ASSERT(x2.size()==2);
	CPPUNIT_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	CPPUNIT_ASSERT(x2[1].dim.is_scalar());
	CPPUNIT_ASSERT(sameExpr(y2,"(x[0]-x[0]_1_)"));
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&e1);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==3);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==1);
	CPPUNIT_ASSERT(var[2]==0);
}


void TestExprSplitOcc::test06() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim(1,3,1));

	// several occurrences of an index (but different nodes)
	const ExprNode& e1=x[0];
	Function f1(x,x[0]-x[1]+x[2]+e1);
	ExprSplitOcc eso(f1.args(),f1.expr());

	const Array<const ExprSymbol>& x2=eso.get_x();
	const ExprNode& y2=eso.get_y();
	CPPUNIT_ASSERT(x2.size()==2);
	CPPUNIT_ASSERT(x2[0].dim==((const ExprNode&) x).dim);
	CPPUNIT_ASSERT(sameExpr(y2,"(((x[0]-x[1])+x[2])+x[0]_1_)"));
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[1])==&e1);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==4);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==1);
	CPPUNIT_ASSERT(var[2]==2);
	CPPUNIT_ASSERT(var[3]==0);
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
	CPPUNIT_ASSERT(x2.size()==4);
	CPPUNIT_ASSERT(sameExpr(y2,"((((x[0]+(-x_0_)[1])+x[1])+x[0]_1_)+x[1]_1_)"));
	CPPUNIT_ASSERT(&eso.node(x2[0])==&x); // the "special node" (which is inserted first)
	CPPUNIT_ASSERT(&eso.node(x2[1])==&x);
	CPPUNIT_ASSERT(&eso.node(x2[2])==&e1);
	CPPUNIT_ASSERT(&eso.node(x2[3])==&e2);
	int* var;
	int n=eso.var_map(var);
	CPPUNIT_ASSERT(n==8);
	CPPUNIT_ASSERT(var[0]==0);
	CPPUNIT_ASSERT(var[1]==1);
	CPPUNIT_ASSERT(var[2]==2);
	CPPUNIT_ASSERT(var[3]==0);
	CPPUNIT_ASSERT(var[4]==1);
	CPPUNIT_ASSERT(var[5]==2);
	CPPUNIT_ASSERT(var[6]==0);
	CPPUNIT_ASSERT(var[7]==1);
}

} // end namespace
