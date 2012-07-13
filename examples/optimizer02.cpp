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

	// Example #19
	// ------------------------------------------------
	// Optimizer (with a system loaded from a file)
	//

	System sys("ex2_1_3.bch");

	RoundRobin rr;

	Optimizer o(*sys.goal,sys.f,rr);

	//o.trace=true;

	o.optimize(sys.box);
	o.report();

	return 0;
}
