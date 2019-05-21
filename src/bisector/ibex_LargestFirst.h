//============================================================================
//                                  I B E X                                   
// File        : Largest First bisector
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2012
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_LARGEST_FIRST_H__
#define __IBEX_LARGEST_FIRST_H__

#include "ibex_Bsc.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief largest-first bisector.
 *
 */
class LargestFirst : public Bsc {
public:

	/**
	 * \brief Create a bisector with largest-first heuristic.
	 *
	 * \param prec             - see #Bsc::Bsc(double). By default, 0 which means an endless uniform bisection process.
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	LargestFirst(double prec=0, double ratio=Bsc::default_ratio());

	/**
	 * \brief Create a bisector with largest first heuristic.
	 *
	 * \param prec             - see #Bsc::Bsc(double).
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	LargestFirst(const Vector& prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Return next variable to be bisected.
	 *
	 * called by Bsc::bisect(...)
	 */
	virtual BisectionPoint choose_var(const Cell& cell);

	/**
	 * \brief Ratio to choose the split point.
	 *
	 * Ratio between the diameters of the left and right parts of a bisected
	 * interval.
	 */
	const double ratio;

 protected :
	virtual bool nobisectable (const IntervalVector& box, int i) const ;
};

} // end namespace ibex

#endif // __IBEX_LARGEST_FIRST_H__
