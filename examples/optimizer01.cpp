//============================================================================
//                                  I B E X                                   
// File        : optimizer01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;

int main() {

	// Example #18
	// ------------------------------------------------
	// Optimizer (with an hard-coded function)
	//

	Variable x,y;

	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_goal(x+y);
	fac.add_ctr(sqr(x)+sqr(y)<=1);

	System sys(fac);

	DefaultOptimizer o(sys);

	IntervalVector box(2);

	o.optimize(box);

	o.report();
	return 0;
}
