//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderFwdBwd.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_FWD_BWD__
#define __IBEX_LOUP_FINDER_FWD_BWD__

#include "ibex_LoupFinder.h"
#include "ibex_CtcUnion.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Contractor-based upper-bounding algorithm.
 *
 * This loup finder first builds an inner box using
 * forward-backward contractors on reversed inequalities
 * and, in case of success, looks for a point inside
 * the inner box with a #LoupFinderProbing algorithm.
 *
 * \note Only works with inequality constraints.
 *
 */
class LoupFinderFwdBwd : public LoupFinder {
public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys   - The NLP problem.
	 */
	LoupFinderFwdBwd(const System& sys);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Delete this.
	 */
	virtual ~LoupFinderFwdBwd();

	/**
	 * \brief The NLP problem (min f(x) s.t. g(x)<=0)
	 */
	const System& sys;

	/**
	 * Flag for applying "monotonicity analysis".
	 *
	 * The value can be fixed by the user.
	 * By default: true.
	 */
	bool mono_analysis_flag;

protected:

	/**
	 * \brief Codimension of g (0 if no constraint).
	 */
	const int m;

	/*
	 * Inner contractor (for the negation of g<=0)
	 */
	CtcUnion* is_inside;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_FWD_BWD__ */
