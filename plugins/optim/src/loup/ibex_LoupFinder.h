//============================================================================
//                                  I B E X
// File        : ibex_LoupFinder.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER__
#define __IBEX_LOUP_FINDER__

#include "ibex_Vector.h"
#include "ibex_Exception.h"
#include "ibex_System.h"

#include <utility>

namespace ibex {

class LoupFinder {
public:
	class NotFound : Exception { };

	LoupFinder();

	//
	// Note: x0 is the last loup, not necessarily inside the box.
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup)=0;

	virtual ~LoupFinder();


protected:

	/**
	 * \brief Monotonicity analysis.
	 *
	 * When f is increasing (resp. decreasing) w.r.t. variable x_i, the interval [x_i]
	 * is replaced by the lower bound (resp. upper bound) of [x_i].
	 */
	void monotonicity_analysis(const System& sys, IntervalVector& box, bool inner_found);

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param pt       - The candidate point.
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #is_inner(const IntervalVector&)) is performed.
	 *
	 * \note In rigorous mode, the equalities have to be checked anyway (even if
	 *       is_inner==true) because the innership is only wrt the relaxed system.
	 *       In this case, the resulting loup_point may be different than \a pt (the
	 *		 procedure used to check satisfiability
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check(const System& sys, const Vector& pt, double& loup, bool is_inner);
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER__ */
