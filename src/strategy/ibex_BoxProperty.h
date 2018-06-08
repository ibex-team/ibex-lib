//============================================================================
//                                  I B E X
// File        : ibex_BoxData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 2, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_Property.h"
#include "ibex_BitSet.h"
#include "ibex_BisectionPoint.h"

#ifndef __IBEX_BOX_PROPERTY_H__
#define __IBEX_BOX_PROPERTY_H__

namespace ibex {

class BoxProperty : public Property {
public:
	/**
	 * \brief Create a copy.
	 */
	virtual BoxProperty* copy() const =0;

	/**
	 * \brief Create data associated to child cells.
	 */
	virtual std::pair<BoxProperty*,BoxProperty*> update_bisect(const BisectionPoint&)=0;

	void update_contract(const IntervalVector& new_box);

	virtual void update_contract(const IntervalVector& new_box, const BitSet& impact) { }

	void update_change(const IntervalVector& new_box);

	virtual void update_change(const IntervalVector& new_box, const BitSet& impact) { }

	virtual ~BoxProperty();
};

/*================================== inline implementations ========================================*/

inline void BoxProperty::update_contract(const IntervalVector& new_box) {
	update_contract(new_box, BitSet::all(new_box.size()));
}

inline void BoxProperty::update_change(const IntervalVector& new_box) {
	update_change(new_box, BitSet::all(new_box.size()));
}

inline BoxProperty::~BoxProperty() {
}

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTY_H__ */
