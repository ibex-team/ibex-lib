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

 Cell::Cell(const Cell& e) : box(e.box), id(id_count++)  {
 	for ( IBEXMAP(Backtrackable*)::const_iterator it=e.data.begin(); it!=e.data.end(); it++) {
 		data.insert_new(it->first, it->second->copy());
 	}
 }



std::pair<Cell*,Cell*> Cell::bisect(const IntervalVector& left, const IntervalVector& right) const {
	Cell* cleft = new Cell(*this);
	Cell* cright = new Cell(*this);
	cleft->box  = left;
	cright->box = right;
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
