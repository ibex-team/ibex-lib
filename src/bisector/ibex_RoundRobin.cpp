//============================================================================
//                                  I B E X                                   
// File        : ibex_RoundRobin.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : Jun 29, 2018
//============================================================================

#include "ibex_RoundRobin.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

RoundRobin::RoundRobin(double prec, double ratio) : Bsc(prec), ratio(ratio) {

}

RoundRobin::RoundRobin(const Vector& prec, double ratio) : Bsc(prec), ratio(ratio) {

}

BisectionPoint RoundRobin::choose_var(const Cell& cell) {

	int last_var=cell.bisected_var;

	const IntervalVector& box=cell.box;

	int n = box.size();

	if (last_var == -1) last_var = n-1;

	int var = (last_var+1)%n;

	while (var != last_var && too_small(box,var))
		var = (var + 1)%n;

	// if no variable can be bisected an exception is thrown
	if (var==last_var && too_small(box,var))
		throw NoBisectableVariableException();

	else
		return BisectionPoint(var,ratio,true); // output
}

} // end namespace ibex
