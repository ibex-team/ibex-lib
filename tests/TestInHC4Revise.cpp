/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#include "TestInHC4Revise.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

void TestInHC4Revise::add01() {

	Variable x,y;
	Function f(x,y,x+y);

	double init_xy[][2]= { {-1,1}, {-1,1} };
	IntervalVector box(2,init_xy);

	f.iproj(Interval(0,1),box);

	TEST_ASSERT((box[0]+box[1]).is_subset(Interval(0,1)));
	check(box[0].lb()+box[1].lb(),0);
	check(box[0].ub()+box[1].ub(),1);
}


void TestInHC4Revise::add02() {

	Variable x;
	Function f(x,x+Interval(0,1));

	IntervalVector box(1,Interval(0,1));

	f.iproj(Interval(0,1),box);

	TEST_ASSERT(box.is_empty() || // TEMPORARY WRAP
			box[0]==Interval(1,1));
}


} // end namespace

