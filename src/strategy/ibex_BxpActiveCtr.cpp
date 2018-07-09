//============================================================================
//                                  I B E X
// File        : ibex_BxpActiveCtr.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 05, 2018
//============================================================================

#include "ibex_BxpActiveCtr.h"
#include "ibex_Id.h"

namespace ibex {

Map<long,false> BxpActiveCtr::ids;

BxpActiveCtr::BxpActiveCtr(const NumConstraint& ctr, bool active) : Bxp(get_id(ctr)), ctr(ctr), active(active) {

}

BxpActiveCtr* BxpActiveCtr::update_copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpActiveCtr(ctr, active);
}

void BxpActiveCtr::update(const BoxEvent& e, const BoxProperties& prop) {
	if (active || e.type!=BoxEvent::CONTRACT) {
		// TODO: if the impact contains no variable involved in the constraint
		// we can avoid evaluation
		Domain y=ctr.right_hand_side();
		switch (y.dim.type()) {
		case Dim::SCALAR:       active = !(ctr.f.eval(e.box).is_subset(y.i())); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   active = !(ctr.f.eval_vector(e.box).is_subset(y.v())); break;
		case Dim::MATRIX:       active = !(ctr.f.eval_matrix(e.box).is_subset(y.m())); break;
		}
	}
}

long BxpActiveCtr::get_id(const NumConstraint& ctr) {
	try {
		return ids[ctr.id];
	} catch(Map<long,false>::NotFound&) {
		long new_id=next_id();
		ids.insert_new(ctr.id, new_id);
		return new_id;
	}
}

} // end namespace ibex
