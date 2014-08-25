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

		System sys1("../benchs/benchs-satisfaction/benchs-IBB/ponts-geo.bch");
		System sys2(sys1,System::COPY);

		double prec=1e-08;

		DefaultSolver solver1(sys1,prec);
		DefaultSolver solver2(sys2,prec);

		solver1.trace=0;
		pair<IntervalVector,IntervalVector> pair=sys1.box.bisect(4);

		vector<IntervalVector> sols1, sols2;

		//sols1=solver1.solve(sys1.box);


#pragma omp sections
		{
			sols1=solver1.solve(pair.first);
#pragma omp section
			sols2=solver2.solve(pair.second);
		}
		cout << "solver #1 found " << sols1.size() << endl;
		cout << "solver #2 found " << sols2.size() << endl;

	}

}
