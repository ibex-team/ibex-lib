/*
 * ibex_LinearRestrictXTaylor.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#ifndef __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__
#define __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__

#include "ibex_NormalizedSystem.h"
#include "ibex_BitSet.h"
#include "ibex_LinearRestrict.h"

namespace ibex {

class LinearRestrictXTaylor : public LinearRestrict {
public:
	LinearRestrictXTaylor(const NormalizedSystem& sys);

	/**
	 * \brief Generation of a linear approximation of the inner region
	 *
	 */
	int linearization(const IntervalVector& box, LinearSolver& lp_solver);

	void set_inactive_ctr(const bool* inactive);

	virtual ~LinearRestrictXTaylor();

protected:
	/**
	 * \brief The system
	 */
	const NormalizedSystem& sys;

	const bool* inactive;

	/*
	 * Boolean array indicating which corner in direction i is used :
	 * true for inferior corner, false for superior one.
	 */
	bool* corner;
};

inline void LinearRestrictXTaylor::set_inactive_ctr(const bool* inactive) {
	this->inactive = inactive;
}

} /* namespace ibex */

#endif /* __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__ */
