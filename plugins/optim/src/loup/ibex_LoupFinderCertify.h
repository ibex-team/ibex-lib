//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderCertify.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_CERTIFY_H__
#define __IBEX_LOUP_FINDER_CERTIFY_H__

#include "ibex_LoupFinder.h"
#include "ibex_Exception.h"

#include <utility>

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Certify a loup point.
 *
 * This finder works like a post-processing: it first runs an unsafe
 * finder working on relaxed equalities. The "pseudo" loup found
 * is then made rigorous by running a Newton-inflation algorithm.
 *
 * This results in a box that rigorously includes a feasible
 * point.
 */
class LoupFinderCertify : public LoupFinder {
public:

	/**
	 * \param sys    - The real system (that is, without relaxation).
	 * \param finder - A loup finder for the relaxed system.
	 */
	LoupFinderCertify(const System& sys, LoupFinder& finder);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Return true.
	 */
	virtual bool rigorous() const {
		return true;
	}

protected:

	/**
	 * \brief The NLP problem.
	 */
	const System& sys;

	/**
	 * \brief True iff there is an equality.
	 */
	const bool has_equality;

	/**
	 * \brief Finder for the relaxed system.
	 */
	LoupFinder& finder;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_CERTIFY_H__ */
