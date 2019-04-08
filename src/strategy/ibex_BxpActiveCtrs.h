//============================================================================
//                                  I B E X
// File        : ibex_BxpActiveCtrs.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 25, 2018
//============================================================================

#ifndef __IBEX_BXP_ACTIVE_CTRS_H__
#define __IBEX_BXP_ACTIVE_CTRS_H__

#include "ibex_BxpActiveCtr.h"
#include "ibex_Map.h"
#include "ibex_System.h"
#include "ibex_BoxProperties.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Which inequalities are potentially active in a system.
 *
 */
class BxpActiveCtrs : public Bxp {
public:
	/**
	 * \brief Build the property value, associated to a system.
	 *
	 * \param sys    - The system
	 */
	BxpActiveCtrs(const System& sys);

	/**
	 * \brief Copy the property
	 */
	virtual BxpActiveCtrs* copy(const IntervalVector& box, const BoxProperties& prop) const;

	/**
	 * \brief Update the property after box modification.
	 *
	 * Lazy variant. Does not actually calculate the image
	 * of the function if active=true. Use check() instead.
	 * \see Bxp::update(...) and BxpActiveCtr::check().
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop);

	/**
	 * \brief To string
	 *
	 * By default: the id.
	 */
	virtual std::string to_string() const;

	/**
	 * \brief Check activity of constraints
	 *
	 * Force functions evaluation.
	 */
	void check(BoxProperties& prop);

	/**
	 * \brief Get the BxpActiveCtrs id of a system.
	 */
	static long get_id(const System&);

	/**
	 * \brief The associated system.
	 */
	const System& sys;

	/**
	 * Active constraints
	 */
	BitSet active;

protected:
	void _update(const BoxProperties& prop);
	BitSet ineq;   // constraints that are inequalities
	static Map<long,false>& ids();
};

/*================================== inline implementations ========================================*/

} /* namespace ibex */

#endif /* __IBEX_BXP_ACTIVE_CTRS_H__ */
