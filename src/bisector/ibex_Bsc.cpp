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
#include "ibex_Exception.h"

using std::pair;

namespace ibex {

double Bsc::default_ratio() {
	return 0.45;
}

Bsc::Bsc(double prec) : _prec(1,prec) {
	if (prec<0) ibex_error("precision must be a nonnegative number");
	// note: prec==0 allowed with, e.g., LargestFirst
}

Bsc::Bsc(const Vector& prec) : _prec(prec) {
	for (int i=0; i<prec.size(); i++)
		if (prec[i]<=0) ibex_error("precision must be a nonnegative number");
}

pair<IntervalVector,IntervalVector> Bsc::bisect(Cell& cell) {
	return bisect(cell.box);
}

void Bsc::add_backtrackable(Cell& root) {
	root.add<BisectedVar>();
}

} // end namespace ibex
