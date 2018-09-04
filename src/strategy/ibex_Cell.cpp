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

Cell::Cell(const IntervalVector& box, int var) : box(box), prop(this->box), bisected_var(var) {

}

Cell::Cell(const Cell& e) : box(e.box), prop(this->box, e.prop), bisected_var(e.bisected_var) {

}

pair<Cell*,Cell*> Cell::bisect(const BisectionPoint& pt) const {

	Cell* cleft;
	Cell* cright;

	if (pt.rel_pos) {
		pair<IntervalVector,IntervalVector> boxes=box.bisect(pt.var,pt.pos);
		cleft = new Cell(boxes.first, pt.var);
		cright = new Cell(boxes.second, pt.var);
	} else {
		IntervalVector b1(box);
		IntervalVector b2(box);
		b1[pt.var]=Interval(box[pt.var].lb(), pt.pos);
		b2[pt.var]=Interval(pt.pos, box[pt.var].ub());
		cleft = new Cell(b1, pt.var);
		cright = new Cell(b2, pt.var);
	}

	prop.update_bisect(Bisection(box, pt, cleft->box, cright->box), cleft->prop, cright->prop);

	return pair<Cell*,Cell*>(cleft,cright);
}

Cell::~Cell() {

}

std::ostream& operator<<(std::ostream& os, const Cell& c) {
	os << c.box;
	return os;
}

} // end namespace ibex
