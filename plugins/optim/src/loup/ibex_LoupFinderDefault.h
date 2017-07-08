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

class LoupFinderDefault : public LoupFinder {
public:
	/**
	 * \brief Update loup either using line_probing or random_probing.
	 *
	 * Main function for probing ;
	 * search for an inner box ;
	 * call line_probing or random_probing. (in the current version, random probing is called)
	 * return true if the loup has been modified.
	 *
	 * \param inHC4 -  Flag for applying inHC4.
	 * If true, apply inHC4. Otherwise, apply is_inside.
	 * The value can be fixed by the user. By default: true. */
	LoupFinderDefault(const /*Normalized*/System& sys, bool inHC4=false);

	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& loup_point, double loup);

	virtual ~LoupFinderDefault();

	// either HC4 or FwdBwd
	LoupFinder& finder_probing;

	LoupFinderXTaylor finder_x_taylor;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_DEFAULT_H__ */
