//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderProbing.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_PROBING_H__
#define __IBEX_LOUP_FINDER_PROBING_H__

#include "ibex_LoupFinder.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Upper-bounding algorithm based on simple sampling and probing.
 *
 * The algorithm has two steps:
 * 1- random search: pick random points in any directions.
 * 2- intensification (in the current version of the code, only in case
 *    of unconstrained optimization): perform a line probing search.
 *
 * \note Only works with inequality constraints.
 *
 */
class LoupFinderProbing : public LoupFinder {
public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys         - The NLP problem.
	 * \param sample_size - Number of sample points inside the box.
	 */
	LoupFinderProbing(const System& sys, int sample_size=default_sample_size);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Second method (line probing).
	 *
	 * Performs a dichotomic search between the current loup-point and its projection on the
	 * facet of the input box in the opposite direction of its gradient.
	 *
	 * return true if the loup has been modified.
	 */
	bool line_probing(Vector& loup_point, double& loup, const IntervalVector& box);

	/**
	 * \brief Perform a dichotomic search of a minimum in a line (see Hansen's book).
	 *
	 * The search is performed on the segment delimited by the current loup-point and \a end_point.
	 *
	 * If \a exit_if_above_loup is true, the search stops as soon as we fall on
	 * a candidate x with f(x)>loup.
	 */
	bool dichotomic_line_search(Vector& loup_point, double& loup, const Vector& end_point, bool exit_if_above_loup);

	/**
	 * Default sample size
	 */
	static constexpr int default_sample_size = 1;

	/**
	 * \brief The NLP problem.
	 */
	const System& sys;

	/**
	 * Number of samples used to update the loup
	 */
	const int sample_size;

protected:
//
//	/**
//	 * Current loup-point
//	 */
//	Vector loup_point;
//
//	/**
//	 * Current loup
//	 */
//	double loup;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_PROBING_H__ */
