//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDuality.h
// Author      : Gilles Chabert, Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 20, 2019
// Last Update : Mar 20, 2019
//============================================================================

#ifndef __IBEX_LOUP_FINDER_DUALITY_H__
#define __IBEX_LOUP_FINDER_DUALITY_H__

#include "ibex_LinearizerXTaylor.h"
#include "ibex_LoupFinder.h"

namespace ibex {
/**
 * \ingroup optim
 *
 * \brief Upper-bounding algorithm based on duality.
 *
 * The algorithm builds an inner (feasible) polytope inside the
 * current box (see #LinearizerXTaylor) and then minimizes a
 * linear approximation of the goal function on this polytope via
 * a LP solver. The resulting point is verified a posteriori to
 * be feasible (wrt nonlinear constraint) and a new "loup".
 *
 * \note Only works with inequality constraints.
 */
class LoupFinderDuality : public LoupFinder {
public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys         - The NLP problem.
	 */
	LoupFinderDuality(const System& sys);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

	/**
	 * \brief Add properties required by this loup finder.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	/**
	 * \brief The NLP problem.
	 */
	const System& sys;

protected:

	/* Linearization technique
	 *
	 * Return the LP solver with variables, constraints, bounds and objective set.
	 *
	 * \throw NotFound if a problem occurs in the linearization
	 *
	 * TODO: the following function should be embedded in a Linearizer object, but
	 * this is not currently possible as the number of variables in the LPSolver is dynamic
	 * (depends on the number of active constraints on the box).
	 */
	LPSolver* linearize(const IntervalVector& box);
};


inline std::pair<IntervalVector, double> LoupFinderDuality::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	BoxProperties prop(box);
	return find(box, loup_point, loup, prop);
}

} /* namespace ibex */



#endif /* __IBEX_LOUP_FINDER_DUALITY_H__ */
