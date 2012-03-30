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

namespace ibex {

void TestFunction::add_symbol() {
	Function f;
	TEST_ASSERT(f.nb_nodes()==0);
	TEST_ASSERT(f.nb_symbols()==0);

	const SymbolExpr& x=f.add_symbol("x");
	TEST_ASSERT(f.nb_nodes()==1);
	TEST_ASSERT(f.nb_symbols()==1);
	TEST_ASSERT(strcmp(f.symbol_name(0),"x")==0);
	TEST_ASSERT(!f.used("x"));
	TEST_ASSERT(!f.used(0));

	//TEST_THROWS_ANYTHING(f.add_symbol("x"));

	const SymbolExpr& y=f.add_symbol("y");
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



} // end namespace
