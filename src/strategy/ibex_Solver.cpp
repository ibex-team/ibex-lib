//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Solver.h"
#include "ibex_EmptyBoxException.h"
#include <cassert>

using std::vector;

namespace ibex {

vector<IntervalVector> Solver::solve(const IntervalVector& init_box) {
	start(init_box);

	ctc.init_root(*buffer.top()); // ugly

	vector<IntervalVector> sols;
	IntervalVector tmpbox(init_box.size());
	Cell* c;

	while ((c=next_cell())) {

		tmpbox = c->box;

		try {
			ctc.contract(*c);
		} catch(EmptyBoxException&) {
			assert(c->box.is_empty());
			continue;
		}

		try {
			((Ctc&) prec).contract(*c);
		} catch(EmptyBoxException&) {
			assert(c->box.is_empty());
			sols.push_back(tmpbox);
		}
	}
	return sols;
}

} // end namespace ibex
