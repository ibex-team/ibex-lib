//============================================================================
//                                  I B E X                                   
// File        : ibex_RoundRobin.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : May 8, 2012
//============================================================================

#include "ibex_RoundRobin.h"
#include "ibex_NoBisectableVariableException.h"
using std::pair;

namespace ibex {

RoundRobin::RoundRobin(double prec, double ratio) : Bsc(prec), ratio(ratio) {

}

RoundRobin::RoundRobin(const Vector& prec, double ratio) : Bsc(prec), ratio(ratio) {

}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box, int& last_var) {

  int n = box.size();

  if (last_var == -1) last_var = n-1;

  int var = (last_var+1)%n;


  while (var != last_var && too_small(box,var))
    var = (var + 1)%n;  

  // if no variable can be bisected an exception is thrown
  if (var==last_var && too_small(box,var))
	  throw NoBisectableVariableException();

  last_var = var; // output

  return box.bisect(var,ratio);
}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box) {
	int i=-1;
	return bisect(box,i);
}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(Cell& cell) {
	BisectedVar& v=cell.get<BisectedVar>();

	// the following instruction will update v.var
	// and the new value of v.var will be copied to child nodes
	return bisect(cell.box,v.var);
}

void RoundRobin::add_backtrackable(Cell& root) {
	root.add<BisectedVar>();
}

} // end namespace ibex
