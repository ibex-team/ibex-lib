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
	Function f(x,y,x+y);
	Function g(x,y,sqr(x)+sqr(y)-1.0);
	NumConstraint c(g,NumConstraint::LEQ);

	CtcProj proj(c);
	RoundRobin rr;

	Optimizer o(f,g,proj,rr);

	IntervalVector box(2);
	//o.trace=true;
	o.optimize(box);

	o.report();
	return 0;
}
