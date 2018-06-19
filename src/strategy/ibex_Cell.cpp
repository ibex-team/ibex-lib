//============================================================================
//                                  I B E X                                   
// File        : ibex_Cell.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : Jun 07, 2018
//============================================================================

#include "ibex_Cell.h"
#include "ibex_Bsc.h"
#include "ibex_BoxProp.h"
#include "ibex_SearchNodeProp.h"

#include <limits.h>

using namespace std;

namespace ibex {

Cell::Cell(const IntervalVector& box) : box(box) { }

Cell::Cell(const Cell& e) : box(e.box) {
	for (Map<Property>::const_iterator it=e.prop.begin(); it!=e.prop.end(); it++) {
		prop.insert_new(it->first, it->second->copy());
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

	for (Map<Property>::const_iterator it=prop.begin(); it!=prop.end(); it++) {
		SearchNodeProp* snp = dynamic_cast<SearchNodeProp*>(it->second);
		if (snp) {
			std::pair<SearchNodeProp*,SearchNodeProp*> child_data=snp->update_bisect(b);
			cleft->prop.insert_new(it->first,child_data.first);
			cright->prop.insert_new(it->first,child_data.second);
		} else {
			BoxProp* bp = dynamic_cast<BoxProp*>(it->second);
			if (bp) {
				std::pair<BoxProp*,BoxProp*> child_data=bp->update_bisect(b);
				cleft->prop.insert_new(it->first,child_data.first);
				cright->prop.insert_new(it->first,child_data.second);
			} else
				ibex_error("[Cell]: unexpected property");
		}
	}

	return pair<Cell*,Cell*>(cleft,cright);
}

Cell::~Cell() {
	for (Map<Property>::iterator it=prop.begin(); it!=prop.end(); it++)
		delete it->second;
}

std::ostream& operator<<(std::ostream& os, const Cell& c) {
	os << c.box;
	return os;
}

} // end namespace ibex
