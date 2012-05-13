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

using std::pair;

namespace ibex {

RoundRobin::RoundRobin(double w, double ratio) : w(w), ratio(ratio) {

}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box, int& last_var) {

  int n = box.size();

  if (last_var == -1) last_var = n-1;

  int var = (last_var+1)%n;

  while (var != last_var && box[var].diam()< w) var = (var + 1)%n;

  if (var==last_var) var = (last_var+1)%n;

  last_var = var; // output

  return box.bisect(var,ratio);
}

pair<IntervalVector,IntervalVector> RoundRobin::bisect(const IntervalVector& box) {
	int i=-1;
	return bisect(box,i);
}

pair<Cell*,Cell*> RoundRobin::bisect(Cell& cell) {
	BisectedVar& v=cell.get<BisectedVar>();

	// the following instruction will update v.var
	pair<IntervalVector,IntervalVector> boxes=bisect(cell.box,v.var);

	// and the new value of v.var will be copied to child nodes
	return cell.bisect(boxes.first, boxes.second);
}

void RoundRobin::init_root(Cell& root) {
	root.add<BisectedVar>();
}

} // end namespace ibex
