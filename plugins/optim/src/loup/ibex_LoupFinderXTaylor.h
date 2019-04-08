//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderXTaylor.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_X_TAYLOR_H__
#define __IBEX_LOUP_FINDER_X_TAYLOR_H__

#include "ibex_LinearizerXTaylor.h"
#include "ibex_LoupFinder.h"

namespace ibex {
/**
 * \ingroup optim
 *
 * \brief Upper-bounding algorithm based on XTaylor restriction.
 *
 * The algorithm builds an inner (feasible) polytope inside the
 * current box (see #LinearizerXTaylor) and then minimizes a
 * linear approximation of the goal function on this polytope via
 * a LP solver. The resulting point is verified a posteriori to
 * be feasible (wrt nonlinear constraint) and a new "loup".
 *
 * \note Only works with inequality constraints.
 */
class LoupFinderXTaylor : public LoupFinder {
public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys         - The NLP problem.
	 */
	LoupFinderXTaylor(const System& sys);

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

	/** Linearization technique. */
	LinearizerXTaylor lr;

	/** linear solver */
	LPSolver lp_solver;

	/** Miscellaneous   for statistics */
//	int nb_simplex;
//	double diam_simplex;
};

/*============================================ inline implementation ============================================ */

inline std::pair<IntervalVector, double> LoupFinderXTaylor::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	BoxProperties prop(box);
	return find(box, loup_point, loup, prop);
}

} /* namespace ibex */



#endif /* __IBEX_LOUP_FINDER_X_TAYLOR_H__ */
