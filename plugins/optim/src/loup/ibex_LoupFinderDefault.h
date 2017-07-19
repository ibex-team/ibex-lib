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
 * \brief Default upper-bounding algorithm.
 *
 * The algorithm uses two upperbounding techniques:
 * - one based on inner box: simple sampling/line probing or in-HC4
 * - one based on inner polytope: XTaylor restriction.
 *
 * Note: currently, line probing is disabled unless there is
 * a constraint-free NLP problem (a simple sampling is done).
 */
class LoupFinderDefault : public LoupFinder {
public:
	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys   - The NLP problem.
	 * \param inHC4 - Flag for building inner boxes. If true, apply inHC4 (inner arithmetic).
	 *                Otherwise, use forward/backward contractor on reversed inequalities.
	 *                By default: true.
	 */
	LoupFinderDefault(const System& sys, bool inHC4=true);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup);

	/**
	 * \brief Delete this.
	 */
	virtual ~LoupFinderDefault();

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

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_DEFAULT_H__ */
