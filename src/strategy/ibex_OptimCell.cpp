//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimCell.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 7, 2014
// Last Update : Apr 7, 2014
//============================================================================

#include "ibex_OptimCell.h"

namespace ibex {

  OptimCell::OptimCell(const IntervalVector& box) : Cell(box),heap_present(0),loup(0) {

}

std::pair<OptimCell*,OptimCell*> OptimCell::bisect(const IntervalVector& left, const IntervalVector& right) {

	OptimCell* cleft = new OptimCell(left);
	OptimCell* cright = new OptimCell(right);
	for (IBEXMAP(Backtrackable*)::iterator it=data.begin(); it!=data.end(); it++) {
		std::pair<Backtrackable*,Backtrackable*> child_data=it->second->down();
		cleft->data.insert_new(it->first,child_data.first);
		cright->data.insert_new(it->first,child_data.second);
	}
	return std::pair<OptimCell*,OptimCell*>(cleft,cright);
}



} // end namespace ibex
