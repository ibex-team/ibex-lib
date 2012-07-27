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
int main(int argc, char** argv){

	// Example #19
	// ------------------------------------------------
	// Optimizer (with a system loaded from a file)
	//

         // Load a problem to optimize
	// --------------------------
	try {

	if (argc<2) {
		cerr << "usage: optimizer02 [filename]" << endl;
		exit(1);
	}

	System sys(argv[1]);

	RoundRobin rr;

	Optimizer o(*sys.goal,sys.f,rr);

        //o.trace=true;
	o.optimize(sys.box);
	o.report();

	return 0;
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
