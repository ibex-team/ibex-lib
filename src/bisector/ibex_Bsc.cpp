//============================================================================
//                                  I B E X                                   
// File        : ibex_Bisector.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : May 8, 2012
//============================================================================

#include "ibex_Bsc.h"
#include "ibex_Cell.h"

using std::pair;

namespace ibex {

const double Bsc::default_prec = 1.e-7;
const double Bsc::default_ratio = 0.45;

pair<IntervalVector,IntervalVector> Bsc::bisect(Cell& cell) {
	return bisect(cell.box);
}

void Bsc::add_backtrackable(Cell& root) {
	root.add<BisectedVar>();
}

} // end namespace ibex
