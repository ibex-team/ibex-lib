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

class LoupFinderFwdBwd : public LoupFinder {
public:
	LoupFinderFwdBwd(const System& sys);

	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup);

	virtual ~LoupFinderFwdBwd();

	/**
	 * \brief The system g(x)<=0
	 */
	const System& sys;

	/**
	 * \brief Codimension of g (0 if no constraint).
	 */
	int m;

	/** Inner contractor (for the negation of g<=0) */
	CtcUnion* is_inside;

	/** Flag for applying monotonicity analysis.
	 * The value can be fixed by the user. By default: true. */
	bool mono_analysis_flag;
};

} /* namespace ibex */

#endif /* IBEX_INFWDBWDFINDER_H_ */
