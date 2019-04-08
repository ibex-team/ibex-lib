//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderDefault.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_DEFAULT_H__
#define __IBEX_LOUP_FINDER_DEFAULT_H__

#include "ibex_LoupFinder.h"
#include "ibex_System.h"
#include "ibex_LoupFinderXTaylor.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Default upper-bounding algorithm (for inequalities only).
 *
 * The algorithm uses two upper-bounding techniques:
 * - one based on the search of an inner box:
 *      simple sampling/line probing or in-HC4
 * - one based on the search of an inner polytope:
 *      XTaylor restriction.
 *
 * Note: currently, line probing is disabled unless there is
 * a constraint-free NLP problem (a simple sampling is done
 * otherwise).
 *
 * \note Only works with inequality constraints.
 */
class LoupFinderDefault : public LoupFinder {
public:
	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys   - The NLP problem.
	 * \param inHC4 - Flag for building inner boxes. If true, apply inHC4 (inner arithmetic).
	 *                Otherwise, use forward/backward contractor on reversed inequalities.
	 *                Drawbacks of the current implement of inHC4:
	 *                1/ does not work with vector/matrix constraints
	 *                2/ generates symbolically components of the main function (heavy)
	 *
	 */
	LoupFinderDefault(const System& sys, bool inHC4=true);

	/**
	 * \brief Delete this.
	 */
	virtual ~LoupFinderDefault();

	/**
	 * \brief Find a new loup in a given box.
	 *
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

	/**
	 * \brief Add properties required by finder_probing and finder_x_taylor.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	/*
	 * Loup finder using inner boxes.
	 *
	 * Either HC4 or CtcUnion (of CtcFwdBwd).
	 */
	LoupFinder& finder_probing;

	/**
	 * Loup finder using inner polytopes.
	 */
	LoupFinderXTaylor finder_x_taylor;
};

inline std::pair<IntervalVector, double> LoupFinderDefault::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	BoxProperties prop(box);
	return find(box, loup_point, loup, prop);
}

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_DEFAULT_H__ */
