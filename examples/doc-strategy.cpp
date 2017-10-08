//============================================================================
//                                  I B E X
// File        : doc-strategy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2014
// Last Update : Apr 25, 2014
//============================================================================


#include "ibex.h"

#ifndef _IBEX_WITH_SOLVER_
#error "You need to install the IbexSolve plugin (--with-solver)."
#endif

#ifndef IBEX_BENCHS_DIR
  #define IBEX_BENCHS_DIR "../plugins/solver/benchs"
#endif

using namespace std;
using namespace ibex;

/**
 * This file contains examples of the documentation.
 */

int main() {

	{
	//! [bsc-different-prec]
	double _prec[]={1e-8,1e-8,1e-4,1};

	Vector prec(4,_prec);

	RoundRobin rr(prec);
	//! [bsc-different-prec]
	}


	}


}
