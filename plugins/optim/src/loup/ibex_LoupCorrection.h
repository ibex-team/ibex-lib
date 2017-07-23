//============================================================================
//                                  I B E X
// File        : ibex_LoupCorrection.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#ifndef __IBEX_LOUP_CORRECTION_H__
#define __IBEX_LOUP_CORRECTION_H__

#include "ibex_LoupFinder.h"
#include "ibex_Exception.h"

#include <utility>

namespace ibex {

/**
 * \brief Find a certified point
 *
 * a loup point will not be safe (pseudo loup is not the real loup)

	* Loup correction (certification) algorithm.
 *
 */
class LoupCorrection : public LoupFinder {
public:
	/**
	 * \brief Raised when no loup is found.
	 */
	class NotFound : Exception { };

	LoupCorrection(const System& sys, LoupFinder& finder, bool trace=false);

	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

protected:

	/**
	 * \brief The NLP problem.
	 */
	const System& sys;

	/**
	 * \brief True iff there is an equality.
	 */
	const bool has_equality;

	LoupFinder& finder;

	bool trace;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_CORRECTION_H__ */
