/*
 * ibex_SystemBox.h
 *
 *  Created on: Jun 23, 2017
 *      Author: gilles
 */

#ifndef __IBEX_SYSTEM_BOX_H__
#define __IBEX_SYSTEM_BOX_H__

#include "ibex_IntervalVector.h"
#include "ibex_System.h"

namespace ibex {

class SystemBox : public IntervalVector {
public:
	SystemBox(const System& sys, double update_ratio=default_update_ratio);

	~SystemBox();

protected:
	friend class System;

	void update() const;

	static double default_update_ratio;
	double update_ratio;

	const System& sys;

	/**
	 * A stored box.
	 */
	mutable IntervalVector cache;

	mutable Interval _goal_eval;
	mutable bool goal_eval_updated;

	mutable IntervalVector _goal_gradient;
	mutable bool goal_gradient_updated;

	mutable IntervalVector _ctrs_eval;
	mutable bool ctr_eval_updated;

	mutable IntervalMatrix _ctrs_jacobian;
	mutable bool ctr_jacobian_updated;

	mutable BitSet active;

	// means two things:
	// - the "active" bitset is uptodate
	// - the components of _ctrs_eval corresponding to active constraints is uptodate
	mutable bool active_ctr_updated;

	mutable bool active_ctr_jacobian_updated;

};

} /* namespace ibex */

#endif /* __IBEX_SYSTEM_BOX_H__ */
