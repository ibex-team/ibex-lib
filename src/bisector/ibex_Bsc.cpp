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

pair<Cell,Cell> Bsc::bisect(const Cell& cell) {
	pair<IntervalVector,IntervalVector> boxes=bisect(cell.box);
	return pair<Cell,Cell>(Cell(boxes.first),Cell(boxes.second));
}

} // end namespace ibex
