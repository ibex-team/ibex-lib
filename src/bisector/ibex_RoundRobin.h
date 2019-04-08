//============================================================================
//                                  I B E X                                   
// File        : Round-robin bisector
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 08, 2012
// Last Update : Jun 29, 2018
//============================================================================

#ifndef __IBEX_ROUND_ROBIN_H__
#define __IBEX_ROUND_ROBIN_H__

#include "ibex_Bsc.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief Round-robin bisector.
 *
 * See #choose_var(const Cell& cell) for details.
 *
 */
class RoundRobin : public Bsc {
public:
	/**
	 * \brief Create a bisector with round-robin heuristic.
	 *
	 * \param prec             - allow to skip variables whose domain is too small
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	RoundRobin(double prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Create a bisector with round-robin heuristic.
	 *
	 * \param prec             - allow to skip variables whose domain is too small
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	RoundRobin(const Vector& prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Return next variable to be bisected.
	 *
	 * called by Bsc::bisect(...)
	 *
	 * The next variable is the first one after the last bisected (using variable ordering based
	 * based on indices, and in a circular way) with a domain sufficiently large. Hence, if the
	 * domain of the var immediatly following the last one is wider than \a w, it will be bisected.
	 * If the cell is the root cell (no previous bisection), the variable inspected first is the
	 * first one. In case all the variables have domain diameter less than \a w, a
	 * NoBisectableVariableException is raised.
	 */
	virtual BisectionPoint choose_var(const Cell& cell);

	/**
	 * \brief Ratio to choose the split point.
	 *
	 * Ratio between the diameters of the left and right parts of a bisected
	 * interval.
	 */
	const double ratio;
};

} // end namespace ibex

#endif // __IBEX_ROUND_ROBIN_H__
