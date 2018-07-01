/*
 * ibex_BoxPropActiveIneq.h
 *
 *  Created on: Jun 18, 2018
 *      Author: gilles
 */

#ifndef IBEX_BOXPROPACTIVECTR_H_
#define IBEX_BOXPROPACTIVECTR_H_

#include "ibex_BoxProp.h"

namespace ibex {

class BxpActiveCtr : public Bxp {
public:
	BxpActiveCtr(const NumConstraint& ctr, bool active);

	virtual BxpActiveCtr* copy() const;

	virtual void update(const IntervalVector& new_box, bool contract, const BitSet& impact, const BoxProperties& prop);

	const NumConstraint& ctr;

	// true=possibly active, false=inactive
	bool active;
};

/*================================== inline implementations ========================================*/
inline BxpActiveCtr::BxpActiveCtr(const NumConstraint& ctr, bool active) : Bxp(ctr.active_prop_id), ctr(ctr), active(active) {

}

inline BxpActiveCtr* BxpActiveCtr::copy() const {
	return new BxpActiveCtr(ctr, active);
}

inline void BxpActiveCtr::update(const IntervalVector& new_box, bool contract, const BitSet& impact, const BoxProperties& prop) {

	if (active || !contract) {
		Domain y=ctr.right_hand_side();
		switch (y.dim.type()) {
		case Dim::SCALAR:       active = !(ctr.f.eval(new_box).is_subset(y.i())); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   active = !(ctr.f.eval_vector(new_box).is_subset(y.v())); break;
		case Dim::MATRIX:       active = !(ctr.f.eval_matrix(new_box).is_subset(y.m())); break;
		}
	}
}


} /* namespace ibex */

#endif /* IBEX_BOXPROPACTIVECTR_H_ */
