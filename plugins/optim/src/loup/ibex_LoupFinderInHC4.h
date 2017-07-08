//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderInHC4.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_IN_HC4__
#define __IBEX_LOUP_FINDER_IN_HC4__

#include "ibex_LoupFinder.h"
#include "ibex_System.h"

namespace ibex {

class LoupFinderInHC4 : public LoupFinder {
public:
	/**
	 * \brief Reduce the box to an inner box using inHC4 algorithm.
	 *
	 * \return true if the resulting box is proven to be inner, false otherwise.
	 *
	 * Note: even if the result is false, a new box is constructed.
	 */

	LoupFinderInHC4(const System& sys);

	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup);

	// statistics on upper bounding
	//void report();

	/**
	 * \brief The system
	 */
	const System& sys;

	/** Flag for applying monotonicity analysis.
	 * The value can be fixed by the user. By default: true. */
	bool mono_analysis_flag;

	/** Miscellaneous   for statistics */
//	int nb_inhc4;
//	double diam_inhc4;
//	int nb_rand;
//	double diam_rand;
};

} /* namespace ibex */

#endif /* IBEX_INHC4FINDER_H_ */
