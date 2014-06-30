//============================================================================
//                                  I B E X                                   
// File        : Largest First bisector
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 19, 2012
// Last Update : July 19, 2012
//============================================================================

#ifndef __IBEX_LARGEST_FIRST_H__
#define __IBEX_LARGEST_FIRST_H__

#include "ibex_Bsc.h"
#include "ibex_Backtrackable.h"

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
	 * \brief Create a bisector with largest first heuristic.
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
	 * \brief Bisect the largest box.
	 *
	 * called by Bsc::bisect
	 */
	virtual std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box);



	/**
	 * \brief Ratio to chose the split point.
	 *
	 * Ratio between the diameters of the left and right parts of a bisected
	 * interval.
	 */
	const double ratio;


};

} // end namespace ibex

#endif // __IBEX_LARGEST_FIRST_H__
