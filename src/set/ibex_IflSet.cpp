//============================================================================
//                                  I B E X                                   
// File        : ibex_IflSet.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 23, 2015
//============================================================================

#include "ibex_IflSet.h"

namespace ibex {

namespace {

class FoundException { };
class BadPointException { };

class IflSetVisitor : public SetVisitor {
public:
	IflSetVisitor(const IntervalVector& pt) : pt(pt), pt_box(IntervalVector::empty(pt.size())) { }

	virtual void visit_leaf(const IntervalVector& box, BoolInterval status) {
		if (box.is_superset(pt)) {
			if (status==YES) {
				pt_box=box;
				throw FoundException();
			} else if (status==NO) {
				if (pt.is_interior_subset(box)) // at the boundary, can also be MAYBE!
					throw BadPointException();
			}
		}
	}

	// the point to inflate
	const IntervalVector& pt;
	// the inflated box. Empty if not found.
	IntervalVector pt_box;
};
}

IflSet::IflSet(Set& set) : set(set) {


}
Ifl::ifl_status IflSet::inflate(IntervalVector& box, const IntervalVector& pt) {

	IflSetVisitor visitor(pt);
	try {
		set.visit(visitor);
		box = pt;
		return UNKNOWN_POINT;
	} catch(BadPointException&) {
		box.set_empty();
		return BAD_POINT;
	} catch(FoundException&) {
		if (visitor.pt_box.is_superset(box))
			return FULL_INFLATE;
		else {
			box &= visitor.pt_box;
			return INFLATE;
		}
	}
}

} // namespace ibex
