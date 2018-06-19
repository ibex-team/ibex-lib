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

class BoxPropActiveCtr : public BoxProp {
public:
	BoxPropActiveCtr();

	BoxPropActiveCtr(bool active);

	virtual BoxPropActiveCtr* copy() const;

	virtual void update_change(const IntervalVector& new_box, const BitSet& impact);

	bool active;
};

/*================================== inline implementations ========================================*/
inline BoxPropActiveCtr::BoxPropActiveCtr() : active(true) {

}

inline BoxPropActiveCtr::BoxPropActiveCtr(bool active) : active(active) {

}

inline BoxPropActiveCtr* BoxPropActiveCtr::copy() const {
	return new BoxPropActiveCtr(active);
}

inline void BoxPropActiveCtr::update_change(const IntervalVector& new_box, const BitSet& impact) {
	active=true;
}

} /* namespace ibex */

#endif /* IBEX_BOXPROPACTIVECTR_H_ */
