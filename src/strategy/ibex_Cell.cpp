//============================================================================
//                                  I B E X                                   
// File        : ibex_Cell.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : Dec 12, 2017
//============================================================================

#include "ibex_Cell.h"
#include "ibex_Bsc.h"

#include <limits.h>

using namespace std;

namespace ibex {

Cell::Cell(const IntervalVector& box) : box(box) { }

Cell::Cell(const Cell& e) : box(e.box) {
	for (IBEXMAP(Backtrackable*)::const_iterator it=e.data.begin(); it!=e.data.end(); it++) {
		data.insert_new(it->first, it->second->copy());
	}
}

pair<Cell*,Cell*> Cell::subcells(const BisectionPoint& b) const {

	Cell* cleft;
	Cell* cright;

	if (b.rel_pos) {
		pair<IntervalVector,IntervalVector> boxes=box.bisect(b.var,b.pos);
		cleft = new Cell(boxes.first);
		cright = new Cell(boxes.second);
	} else {
		IntervalVector b1(box);
		IntervalVector b2(box);
		b1[b.var]=Interval(box[b.var].lb(), b.pos);
		b2[b.var]=Interval(b.pos, box[b.var].ub());
		cleft = new Cell(b1);
		cright = new Cell(b2);
	}

	for (IBEXMAP(Backtrackable*)::const_iterator it=data.begin(); it!=data.end(); it++) {
		std::pair<Backtrackable*,Backtrackable*> child_data=it->second->down(b);
		cleft->data.insert_new(it->first,child_data.first);
		cright->data.insert_new(it->first,child_data.second);
	}
	return pair<Cell*,Cell*>(cleft,cright);
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
