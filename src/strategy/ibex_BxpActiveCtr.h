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
	BxpActiveCtr(const IntervalVector& box, const NumConstraint& ctr, bool active=true, bool up2date=false);

	/**
	 * \brief Copy the property
	 */
	virtual BxpActiveCtr* copy(const IntervalVector& box, const BoxProperties& prop) const;

	/**
	 * \brief Update the property after box modification.
	 *
	 * Lazy variant. Does not actually calculate the image
	 * of the function if active=true. Use check() instead.
	 * \see Bxp::update(...).
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop);

	/**
	 * \brief To string
	 *
	 * By default: the id.
	 */
	virtual std::string to_string() const;

	/**
	 * \brief Check if the constraint is inactive
	 *
	 * Force function evaluation.
	 */
	void check();

	/**
	 * \brief The associated constraint.
	 */
	const NumConstraint& ctr;

	/**
	 * \brief Activity flag.
	 *
	 * true=possibly active, false=inactive.
	 *
	 * To force constraint evaluation, use check().
	 */
	bool active() const;

	/**
	 * \brief Mark the constraint as inactive.
	 */
	void set_inactive();

	/**
	 * \brie Get the BxpActiveCtr id of a constraint.
	 */
	static long get_id(const NumConstraint&);

protected:
	const IntervalVector& box;
	bool _active;
	bool up2date;
	static Map<long,false>& ids();
};

/*================================== inline implementations ========================================*/

inline BxpActiveCtr::BxpActiveCtr(const IntervalVector& box, const NumConstraint& ctr, bool active, bool up2date) :
		Bxp(get_id(ctr)), ctr(ctr), box(box), _active(active), up2date(up2date) {

}

inline BxpActiveCtr* BxpActiveCtr::copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpActiveCtr(box, ctr, _active, up2date);
}

inline bool BxpActiveCtr::active() const {
	return _active;
}

inline void BxpActiveCtr::set_inactive() {
	_active=false;
	up2date=true;
}

} /* namespace ibex */

#endif /* __IBEX_BXP_ACTIVE_CTR_H__ */
