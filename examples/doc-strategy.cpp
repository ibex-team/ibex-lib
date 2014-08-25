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

	{
	//! [solver-parallel-C-1]

	// Get the system
	System sys1("../benchs/benchs-satisfaction/benchs-IBB/ponts-geo.bch");

	// Create a copy for the second solver
	System sys2(sys1,System::COPY);

	// Precision of the solution boxes
	double prec=1e-08;

	// Create two solvers
	DefaultSolver solver1(sys1,prec);
	DefaultSolver solver2(sys2,prec);

	// Create a partition of the initial box into two subboxes,
	// by bisecting any variable (here, n°4)
	pair<IntervalVector,IntervalVector> pair=sys1.box.bisect(4);

	// Create a vector of solutions for each solver
	vector<IntervalVector> sols1, sols2;

	// =======================================================
	// Run the solvers in parallel
	// =======================================================
#pragma omp parallel sections
	{
		sols1=solver1.solve(pair.first);
#pragma omp section
		sols2=solver2.solve(pair.second);
	}
	// =======================================================


	cout << "solver #1 found " << sols1.size() << endl;
	cout << "solver #2 found " << sols2.size() << endl;
	//! [solver-parallel-C-1]

	{
	//! [solver-parallel-C-2]
	Solver solver1(*new CtcCompo(*new CtcHC4(sys1),*new CtcNewton(sys1.f)), *new RoundRobin(prec), *new CellStack());
	Solver solver2(*new CtcCompo(*new CtcHC4(sys2),*new CtcNewton(sys2.f)), *new RoundRobin(prec), *new CellStack());
	//! [solver-parallel-C-2]
	}


	}


}
