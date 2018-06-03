//============================================================================
//                                  I B E X
// File        : ibex_DomainData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 2, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_Backtrackable.h"
#include "ibex_BitSet.h"

#ifndef __IBEX_DOMAIN_DATA_H__
#define __IBEX_DOMAIN_DATA_H__

namespace ibex {

class DomainData: public Backtrackable {
public:
	void update_contract(const IntervalVector& new_box);
	virtual void update_contract(const IntervalVector& new_box, const BitSet& impact) { }
	void update_change(const IntervalVector& new_box);
	virtual void update_change(const IntervalVector& new_box, const BitSet& impact) { }
	virtual ~DomainData();
};

/*================================== inline implementations ========================================*/

inline void DomainData::update_contract(const IntervalVector& new_box) {
	update_contract(new_box, BitSet::all(new_box.size()));
}

inline void DomainData::update_change(const IntervalVector& new_box) {
	update_change(new_box, BitSet::all(new_box.size()));
}

inline DomainData::~DomainData() {
}

} /* namespace ibex */

#endif /* __IBEX_DOMAIN_DATA_H__ */
