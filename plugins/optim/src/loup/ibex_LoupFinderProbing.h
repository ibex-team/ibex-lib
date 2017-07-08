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
 *
 * \brief "Loup" search based on simple probing (unused)
 *
 * Only works with inequality constraints.
 *
 */
class LoupFinderProbing : public LoupFinder {
public:
	LoupFinderProbing(const System& sys, int sample_size=default_sample_size);

	/**
	 * \brief First method for probing
	 *
	 * Take random points in any directions.
	 *
	 * \param box  the box in which a random point is searched for (the found inner box if is_inner i strue)
	 * \param fullbox the box where an intensification is made (in the current version only in case of unconstrained optimization)
	 * \param is_inner - If true, the box is already known to be an inner box so there
	 *                   is no need to check constraints again.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup);

	/** Default sample size */
	static const int default_sample_size;

	/**
	 * \brief The system
	 */
	const System& sys;

	/** Number of samples used to update the loup */
	const int sample_size;

protected:

	/**
	 * \brief Second method for probing
	 *
	 * Performs a dichotomic search between the current loup-point and its projection on the
	 * facet of the input box in the opposite direction of its gradient.
	 *
	 * return true if the loup has been modified.
	 */
	bool line_probing(const IntervalVector& box);

	/**
	 * \brief Perform a dichotomic search of a minimum in a line (see Hansen's book).
	 *
	 * The search is performed on the segment delimited by the current loup-point and \a end_point.
	 *
	 * If \a exit_if_above_loup is true, the search stops as soon as we fall on
	 * a candidate x with f(x)>loup.
	 */
	bool dichotomic_line_search(const Vector& end_point, bool exit_if_above_loup);

	Vector loup_point;

	double loup;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_PROBING_H__ */
