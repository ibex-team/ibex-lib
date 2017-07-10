//============================================================================
//                                  I B E X
// File        : ibex_LinearRestrictXTaylor.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Jun 22, 2017
//============================================================================

#ifndef __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__
#define __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__

#include "ibex_NormalizedSystem.h"
#include "ibex_BitSet.h"
#include "ibex_LinearRestrict.h"

namespace ibex {

class LinearRestrictXTaylor : public LinearRestrict {
public:

	/**
	 * \brief Build the linear restriction for a given system.
	 */
	LinearRestrictXTaylor(const System& sys);

	/**
	 * \brief Generation of a linear approximation of the inner region.
	 */
	int linearization(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Delete this.
	 */
	virtual ~LinearRestrictXTaylor();

protected:
	/**
	 * \brief The system
	 */
	const System& sys;

	/**
	 * \brier Number of variables
	 */
	int n;

	/*
	 * Boolean array indicating which corner in direction i is used :
	 * true for inferior corner, false for superior one.
	 */
	bool* inf;

	/**
	 * \brief Get the point corresponding to "inf"
	 */
	IntervalVector get_corner_point(const IntervalVector& box);
};

} /* namespace ibex */

#endif /* __IBEX_LINEAR_RESTRICT_X_TAYLOR_H__ */
