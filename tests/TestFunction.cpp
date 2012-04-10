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
#include "ibex_Expr.h"
#include <sstream>

namespace ibex {

static bool sameExpr(const ExprNode& node, const char* expr) {
	std::stringstream s;
	s << node;
	return strcmp(s.str().c_str(),expr)==0;
}

void TestFunction::add_symbol() {
	Function f;
	TEST_ASSERT(f.nb_nodes()==0);
	TEST_ASSERT(f.nb_symbols()==0);

	const ExprSymbol& x=f.add_symbol("x");
	TEST_ASSERT(f.nb_nodes()==1);
	TEST_ASSERT(f.nb_symbols()==1);
	TEST_ASSERT(strcmp(f.symbol_name(0),"x")==0);
	TEST_ASSERT(!f.used("x"));
	TEST_ASSERT(!f.used(0));

	//TEST_THROWS_ANYTHING(f.add_symbol("x"));

	const ExprSymbol& y=f.add_symbol("y");
	TEST_ASSERT(f.nb_nodes()==2);
	TEST_ASSERT(&f.node(0)==&x);
	TEST_ASSERT(&f.node(1)==&y);
	TEST_ASSERT(f.nb_symbols()==2);
	TEST_ASSERT(strcmp(f.symbol_name(0),"x")==0);
	TEST_ASSERT(strcmp(f.symbol_name(1),"y")==0);
	TEST_ASSERT(!f.used("x"));
	TEST_ASSERT(!f.used(0));
	TEST_ASSERT(!f.used("y"));
	TEST_ASSERT(!f.used(1));
}

void TestFunction::copy() {
	Function f;
	const ExprSymbol& x=f.add_symbol("x",Dim(0,3,0));
	const ExprSymbol& A=f.add_symbol("A",Dim(0,2,3));

	f.set_expr(A*x);

	Function f2(f);
	TEST_ASSERT(f2.nb_nodes()==3);
	TEST_ASSERT(f2.nb_symbols()==2);

	const ExprSymbol* x2=dynamic_cast<const ExprSymbol*>(&f2.node(0));
	TEST_ASSERT(x2!=NULL);
	TEST_ASSERT(strcmp(x2->name,"x")==0);
	TEST_ASSERT(x2->dim==x.dim);

	const ExprSymbol* A2=dynamic_cast<const ExprSymbol*>(&f2.node(1));
	TEST_ASSERT(A2!=NULL);
	TEST_ASSERT(strcmp(A2->name,"A")==0);
	TEST_ASSERT(A2->dim==A.dim);

	const ExprMul* e2=dynamic_cast<const ExprMul*>(&f2.node(2));
	TEST_ASSERT(e2!=NULL);

	TEST_ASSERT(sameExpr(f2.expr(),"(A*x)"));
}

void TestFunction::separate() {

	Function f;

	const ExprSymbol& x=f.add_symbol("x");
	const ExprSymbol& y=f.add_symbol("y");
	const ExprSymbol& z=f.add_symbol("z");

	const ExprNode& e1=x+y-z;
	const ExprNode& e2=x*z;
	const ExprNode& e3=y-z;
	const ExprNode* v[3] = { &e1, &e2, &e3 };

	const ExprVector& e=ExprVector::new_(v, 3, false);

	f.set_expr(e);

	Function* fcomp=f.separate();

	TEST_ASSERT(fcomp[0].nb_symbols()==3);
	TEST_ASSERT(sameExpr(fcomp[0].expr(),"((x+y)-z)"));

	TEST_ASSERT(fcomp[1].nb_symbols()==3);
	TEST_ASSERT(sameExpr(fcomp[1].expr(),"(x*z)"));

	TEST_ASSERT(fcomp[2].nb_symbols()==3);
	TEST_ASSERT(sameExpr(fcomp[2].expr(),"(y-z)"));

	delete[] fcomp;
}

} // end namespace
