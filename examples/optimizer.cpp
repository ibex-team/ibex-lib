//============================================================================
//                                  I B E X                                   
// File        : optimizer04.cpp
// Author      : Gilles Chabert  Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex.h"

using namespace std;
using namespace ibex;

// ----------------------------------------------------------------------------
// Parameterized Optimizer (with a system loaded from a file, and choice of
//  contractor, linearization  and bisector)
// ----------------------------------------------------------------------------

double eqeps= 1.e-8;

int main(int argc, char** argv) {

	if (argc<8) {
		cerr << "usage: optimizer filename filtering linear_relaxation bisection prec goal_prec timelimit "  << endl;
		exit(1);
	}

	 // we build the following array because some parameters are chosen to be not configurable
	 // for the moment
	const char* args[10] = { argv[1], argv[2], argv[3], argv[4], argv[5], argv[6],
			/* relative=absolute precision! */ argv[6],
			/* sample size=1 */ "1",
			/* time limit */ argv[7],
			/* thickness of equation=1e-08 */ "1e-08"
	};

	try {
		UserFriendlyOptimizer o(args);

		// the trace
		o.trace=1;

		// the search itself
		Optimizer::Status result=o.optimize();

		// printing the results
		o.report();

		return (result==Optimizer::SUCCESS? 0 : -1);

	}

	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
