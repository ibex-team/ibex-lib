/* ============================================================================
 * I B E X - Eval Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#include "TestEval.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_Eval.h"

using namespace std;

namespace ibex {

void TestEval::add01() {

	Function f;
	const ExprSymbol& x = f.add_symbol("x");
	const ExprSymbol& y = f.add_symbol("y");
	f.set_expr(x+y);

	Eval e(f);

	std::cout << e.f << std::endl;

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	Interval res=e.eval(box);

	//TEST_ASSERT(res==Interval(4,6));
}


} // end namespace
