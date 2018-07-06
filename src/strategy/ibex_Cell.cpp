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
#include <limits.h>
#include "ibex_Bxp.h"
#include "ibex_Bxp.h"

using namespace std;

namespace ibex {

Cell::Cell(const IntervalVector& box, int var) : box(box), bisected_var(var) {

}

Cell::Cell(const Cell& e) : box(e.box), bisected_var(e.bisected_var) {
	e.prop.update_copy(prop);
}

pair<Cell*,Cell*> Cell::bisect(const BisectionPoint& b) const {

	Cell* cleft;
	Cell* cright;

	if (b.rel_pos) {
		pair<IntervalVector,IntervalVector> boxes=box.bisect(b.var,b.pos);
		cleft = new Cell(boxes.first, b.var);
		cright = new Cell(boxes.second, b.var);
	} else {
		IntervalVector b1(box);
		IntervalVector b2(box);
		b1[b.var]=Interval(box[b.var].lb(), b.pos);
		b2[b.var]=Interval(b.pos, box[b.var].ub());
		cleft = new Cell(b1, b.var);
		cright = new Cell(b2, b.var);
	}

	prop.update_bisect(b, cleft->box, cright->box, cleft->prop, cright->prop);

	return pair<Cell*,Cell*>(cleft,cright);
}

Cell::~Cell() {

}

std::ostream& operator<<(std::ostream& os, const Cell& c) {
	os << c.box;
	return os;
}

} // end namespace ibex
