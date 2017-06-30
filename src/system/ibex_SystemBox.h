/* ============================================================================
 * I B E X - System Box
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 23th, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_SYSTEM_BOX_H__
#define __IBEX_SYSTEM_BOX_H__

#include "ibex_IntervalVector.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Box that represents the domain of some system variables.
 *
 * This class stores in a cache typical interval computations
 * based on a system, like the evaluation of goal/constraints, etc.
 *
 * This cache is used (if present) in the System class functions
 * like eval_goal(...).
 */
class SystemBox : public IntervalVector {
public:
	/**
	 * \brief build a system box.
	 *
	 * \param sys - The system this box is related to
	 * \param update_ratio - Indicates when a modification of the box is considered
	 *                       as significant enough so that the cache is updated. E.g.,
	 *                       the 0 value means "as soon as the box has changed" and the
	 *                       extreme other value 1 means "when one interval is reduced
	 *                       to a single point". See Interval::rel_distance(...).
	 */
	SystemBox(const System& sys, double update_ratio=default_update_ratio);

	/**
	 * \brief Delete this.
	 */
	~SystemBox();

	/**
	 * Set to 0.01
	 */
	static double default_update_ratio;

protected:
	friend class System;

	/**
	 *  Check if something has changed and udpdate the
	 *  flags accordingly.
	 */
	void update() const;

	/**
	 * The system.
	 */
	const System& sys;

	/**
	 * Update ratio.
	 */
	double update_ratio;

	/**
	 * The stored box.
	 */
	mutable IntervalVector cache;

	mutable Interval _goal_eval;    // last memorized computation
	mutable bool goal_eval_updated; // is it up-to-date?

	mutable IntervalVector _goal_gradient; // etc.
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
