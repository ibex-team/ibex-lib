//============================================================================
//                                  I B E X
// File        : ibex_BxpActiveCtr.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2018
// Last update : Jul 09, 2018
//============================================================================

#ifndef __IBEX_BXP_ACTIVE_CTR_H__
#define __IBEX_BXP_ACTIVE_CTR_H__

#include "ibex_Bxp.h"
#include "ibex_NumConstraint.h"
#include "ibex_Map.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Whether an inequality is potentially active or not.
 *
 * The #active field is false only if the inequality #ctr is
 * inactive, i.e., satisfied for all x in the box. If #active
 * is true, the inequality may be active.
 */
class BxpActiveCtr : public Bxp {
public:
	/**
	 * \brief Build the property value, associated to a constraint.
	 *
	 * \param ctr    - The constraint
	 * \param active - Default activity value
	 */
	BxpActiveCtr(const NumConstraint& ctr, bool active=true);

	/**
	 * \brief Copy the property
	 */
	virtual BxpActiveCtr* copy() const;

	/**
	 * \brief Update the property after box modification.
	 * \see Bxp::update(...).
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop);

	/**
	 * \brief The associated constraint.
	 */
	const NumConstraint& ctr;

	/**
	 * \brief Activity flag.
	 *
	 * true=possibly active, false=inactive
	 */
	bool active;

	/**
	 * \brief Get the BxpActiveCtr id of a constraint.
	 */
	static long get_id(const NumConstraint&);

protected:
	static Map<long,false> ids;
};

/*================================== inline implementations ========================================*/

} /* namespace ibex */

#endif /* __IBEX_BXP_ACTIVE_CTR_H__ */
