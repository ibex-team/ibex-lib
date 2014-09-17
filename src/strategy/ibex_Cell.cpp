//============================================================================
//                                  I B E X                                   
// File        : ibex_Cell.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : May 10, 2012
//============================================================================

#include "ibex_Cell.h"
#include <limits.h>

namespace ibex {

namespace {

int id_count=0;
}

 Cell::Cell(const IntervalVector& box) : box(box), id(id_count++) {
	 assert(id_count<ULONG_MAX);
}

std::pair<Cell*,Cell*> Cell::bisect(const IntervalVector& left, const IntervalVector& right) {
	Cell* cleft = new Cell(left);
	Cell* cright = new Cell(right);
	for (IBEXMAP(Backtrackable*)::iterator it=data.begin(); it!=data.end(); it++) {
		std::pair<Backtrackable*,Backtrackable*> child_data=it->second->down();
		cleft->data.insert_new(it->first,child_data.first);
		cright->data.insert_new(it->first,child_data.second);
	}
	return std::pair<Cell*,Cell*>(cleft,cright);
}

Cell::~Cell() {
	for (IBEXMAP(Backtrackable*)::iterator it=data.begin(); it!=data.end(); it++)
		delete it->second;
}


std::ostream& operator<<(std::ostream& os, const Cell& c){
	os << c.box;
	return os;
}

} // end namespace ibex
