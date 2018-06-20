//============================================================================
//                                  I B E X
// File        : ibex_BoxData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 2, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_SearchNodeProp.h"
#include "ibex_BitSet.h"
#include "ibex_BisectionPoint.h"

#ifndef __IBEX_BOX_PROPERTY_H__
#define __IBEX_BOX_PROPERTY_H__

namespace ibex {

class BoxProp : public SearchNodeProp {
public:

	BoxProp();

	/**
	 * \brief Create a copy.
	 */
	virtual BoxProp* copy() const=0;

	void update_contract(const IntervalVector& new_box);

	/*
	 * Default implementation: does nothing.
	 */
	virtual void update_contract(const IntervalVector& new_box, const BitSet& impact);

	void update_change(const IntervalVector& new_box);

	virtual void update_change(const IntervalVector& new_box, const BitSet& impact) { }

	virtual ~BoxProp();
};

/*================================== inline implementations ========================================*/

inline BoxProp::BoxProp() {
}

inline void BoxProp::update_contract(const IntervalVector& new_box, const BitSet& impact) {

}

inline void BoxProp::update_contract(const IntervalVector& new_box) {
	update_contract(new_box, BitSet::all(new_box.size()));
}

inline void BoxProp::update_change(const IntervalVector& new_box) {
	update_change(new_box, BitSet::all(new_box.size()));
}

inline BoxProp::~BoxProp() {
}

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTY_H__ */
