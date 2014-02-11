/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestFunction.h"
#include "ibex_Function.h"
#include "ibex_NumConstraint.h"
#include "ibex_Expr.h"
#include <sstream>

using namespace std;

namespace ibex {

void TestFunction::build01() {
	delete new Function();
	TEST_ASSERT(true);
}

void TestFunction::add_symbol() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	Function f(x,y,x);

	TEST_ASSERT(f.nb_nodes()==2);
	TEST_ASSERT(f.nb_arg()==2);
	TEST_ASSERT(strcmp(f.arg_name(0),"x")==0);
	TEST_ASSERT(strcmp(f.arg_name(1),"y")==0);
	TEST_ASSERT(f.used(0));
	TEST_ASSERT(f.used(0));
	TEST_ASSERT(!f.used(1));
	TEST_ASSERT(!f.used(1));
	TEST_ASSERT(&f.node(0)==&x);
	TEST_ASSERT(&f.node(1)==&y);
}

void TestFunction::copy() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));

	Function f(x,A,A*x);

	Function f2(f);
	TEST_ASSERT(f2.nb_nodes()==3);
	TEST_ASSERT(f2.nb_arg()==2);

	const ExprMul* e2=dynamic_cast<const ExprMul*>(&f2.node(0));
	TEST_ASSERT(e2!=NULL);
	TEST_ASSERT(sameExpr(f2.expr(),"(A*x)"));

	const ExprSymbol* x2=dynamic_cast<const ExprSymbol*>(&f2.node(1));
	TEST_ASSERT(strcmp(x2->name,"x")==0);
	TEST_ASSERT(x2->dim==x.dim);

	const ExprSymbol* A2=dynamic_cast<const ExprSymbol*>(&f2.node(2));
	TEST_ASSERT(A2!=NULL);
	TEST_ASSERT(strcmp(A2->name,"A")==0);
	TEST_ASSERT(A2->dim==A.dim);

}

void TestFunction::separate() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");

	const ExprNode& e1=x+y-z;
	const ExprNode& e2=x*z;
	const ExprNode& e3=y-z;
	const ExprNode* v[3] = { &e1, &e2, &e3 };

	const ExprVector& e=ExprVector::new_(v, 3, false);

	Function f(x,y,z,e);

	TEST_ASSERT(f[0].nb_arg()==3);
	TEST_ASSERT(sameExpr(f[0].expr(),"((x+y)-z)"));

	TEST_ASSERT(f[1].nb_arg()==3);
	TEST_ASSERT(sameExpr(f[1].expr(),"(x*z)"));

	TEST_ASSERT(f[2].nb_arg()==3);
	TEST_ASSERT(sameExpr(f[2].expr(),"(y-z)"));
}

void TestFunction::used() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");

	const ExprNode& e1=x+y-z;
	const ExprNode& e2=x*z;
	const ExprNode& e3=y-z;
	const ExprNode* v[3] = { &e1, &e2, &e3 };

	const ExprVector& e=ExprVector::new_(v, 3, false);

	Function f(x,y,z,e);

	TEST_ASSERT(f[0].used(0));
	TEST_ASSERT(f[0].used(1));
	TEST_ASSERT(f[0].used(2));
	TEST_ASSERT(f[0].used(0));
	TEST_ASSERT(f[0].used(1));
	TEST_ASSERT(f[0].used(2));

	TEST_ASSERT(f[1].used(0));
	TEST_ASSERT(!f[1].used(1));
	TEST_ASSERT(f[1].used(2));
	TEST_ASSERT(f[1].used(0));
	TEST_ASSERT(!f[1].used(1));
	TEST_ASSERT(f[1].used(2));

	TEST_ASSERT(!f[2].used(0));
	TEST_ASSERT(f[2].used(1));
	TEST_ASSERT(f[2].used(2));
	TEST_ASSERT(!f[2].used(0));
	TEST_ASSERT(f[2].used(1));
	TEST_ASSERT(f[2].used(2));
}

void TestFunction::used02() {
	Variable x(3,"x");
	Variable y(2,3,"y");

	Function f(x,y,x[0]+x[2]-(y[1][1]*y[1][2]));

	TEST_ASSERT(f.used(0));
	TEST_ASSERT(!f.used(1));
	TEST_ASSERT(f.used(2));

	TEST_ASSERT(!f.used(3));
	TEST_ASSERT(!f.used(4));
	TEST_ASSERT(!f.used(5));

	TEST_ASSERT(!f.used(6));
	TEST_ASSERT(f.used(7));
	TEST_ASSERT(f.used(8));

}

void TestFunction::numctr01() {
	Variable x("x");
	Variable y("y");
	Function f(x,y,x+y);
	NumConstraint c(f,EQ);
}

void TestFunction::apply01() {
	Variable x("x");
	Variable y("y");
	Function f(x,y,x+y,"f");
	Interval c(0,1);
	Variable z("z");
	Function f2(z,f(z,c));

	TEST_ASSERT(sameExpr(f2.expr(),"f(z,[0, 1])"));
}

} // end namespace
