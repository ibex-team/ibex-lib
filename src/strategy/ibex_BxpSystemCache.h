//============================================================================
//                                  I B E X
// File        : ibex_BxpSystemCache.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 23, 2017
// Last Update : Jul 29, 2018
//============================================================================

#ifndef __IBEX_BXP_SYSTEM_CACHE_H__
#define __IBEX_BXP_SYSTEM_CACHE_H__

#include "ibex_BxpActiveCtr.h"
#include "ibex_Map.h"
#include "ibex_System.h"
#include "ibex_BoxProperties.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Cache for system computations
 *
 * This class stores in a cache typical interval computations
 * based on a system, like the evaluation of goal/constraints, etc.
 *
 */
class BxpSystemCache : public Bxp {
public:

	/**
	 * \brief Get the BxpSystemCache id of a system.
	 */
	static long get_id(const System&);

	/**
	 * \brief Build the property value, associated to a system.
	 *
	 * \param sys - The system this cache is related to
	 * \param update_ratio - Indicates when a modification of the box is considered
	 *                       as significant enough so that the cache is updated. E.g.,
	 *                       the 0 value means "as soon as the box has changed" and the
	 *                       extreme other value 1 means "when one interval is reduced
	 *                       to a single point". See Interval::rel_distance(...).
	 *
	 */
	BxpSystemCache(const System& sys, double update_ratio);

	/**
	 * \brief Copy the property
	 */
	virtual BxpSystemCache* copy(const IntervalVector& box, const BoxProperties& prop) const;

	/**
	 * \brief Update the property after box modification.
	 *
	 *  Check if something has changed and udpdate the
	 *  flags accordingly.
	 *
	 * TODO: If the box size is n+1 where n is the number of
	 * variables of the system, it means that the box actually
	 * corresponds to the extended system! Fixed so far by a hack.
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop);

	/**
	 * \brief To string
	 *
	 * By default: the id.
	 */
	virtual std::string to_string() const;

	/**
	 * \brief Interval evaluation of the goal.
	 */
	Interval goal_eval() const;

	/**
	 * \brief Interval gradient of the goal.
	 */
	IntervalVector goal_gradient() const;

	/**
	 * \brief Interval gradient of the goal.
	 */
	void goal_gradient(IntervalVector& g) const;

	/**
	 * \brief Interval evaluation of the constraint functions.
	 *
	 * E.g. if there are two constraints x<=0 and y+x>=0, the
	 * function returns ([x],[y]+[x]).
	 * Usually, this function is called either on a system containing
	 * only equalities or a normalized system (see NormalizedSystem).
	 *
	 */
	IntervalVector ctrs_eval() const;

	/**
	 * \brief Interval evaluation of the constraint functions.
	 */
	void ctrs_eval(IntervalVector& ev) const;

	/**
	 * \brief Get the jacobian matrix of the constraints.
	 */
	IntervalMatrix ctrs_jacobian() const;

	/**
	 * \brief Get the jacobian matrix of the constraints.
	 *
	 * See comments above.
	 */
	void ctrs_jacobian(IntervalMatrix& J) const;

	/**
	 * \brief Get the (potentially) active constraints.
	 *
	 * A constraint which is not in the bitset is proven to be inactive.
	 *
	 * \warning A "constraint" here corresponds to a component of "f_ctrs"
	 *          **not** a constraint in the "ctrs" array (this is different
	 *          in the case of vector/matrix constraints).
	 */
	BitSet& active_ctrs() const;

	/**
	 * \brief Quick check that the box is inside g(x)<=0.
	 *
	 * \return True only if all the constraints are inactive
	 */
	bool is_inner() const;

	/**
	 * \brief Interval evaluation of the active constraints.
	 *
	 * \pre The number of (potentially) active constraints must be >0
	 */
	IntervalVector active_ctrs_eval() const;

	/**
	 * \brief Interval jacobian matrix of the active constraints.
	 *
	 * * \pre The number of (potentially) active constraints must be >0
	 */
	IntervalMatrix active_ctrs_jacobian() const;

	/**
	 * \brief The associated system.
	 */
	const System& sys;

	/**
	 * Set to 0.01
	 */
	static constexpr double default_update_ratio = 0.1;

	/**
	 * \brief Is the set of active constraints up-to-date?
	 */
	bool is_active_ctrs_uptodate() const;

protected:

	/**
	 * Number of variables of the system
	 */
	int nb_var;

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
	// - the "active" bitset is up-to-date
	// - the components of _ctrs_eval corresponding to active constraints is up-to-date
	mutable bool active_ctr_updated;

	mutable bool active_ctr_jacobian_updated;

	// If <>-1 then the box will be extended but the system is not.
	// Note: Not very clean. To be fixed at some point...
	int goal_var;

	static Map<long,false>& ids();
};

/*================================== inline implementations ========================================*/

inline bool BxpSystemCache::is_active_ctrs_uptodate() const {
	return active_ctr_updated;
}

} /* namespace ibex */

#endif /* __IBEX_BXP_SYSTEM_CACHE_H__ */
