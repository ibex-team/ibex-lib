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

#include "ibex_System.h"
#include "ibex_Exception.h"

#include <utility>

namespace ibex {

/**
 * \brief Find a certified point
 *
 * Note: This class may be a subclass of LoupFinder in the future.
 */
class LoupCorrection {
public:
	/**
	 * \brief Raised when no loup is found.
	 */
	class NotFound : Exception { };

	LoupCorrection(const System& sys, bool trace);

	std::pair<IntervalVector, double> find(double loup, const Vector& pseudo_loup_point, double pseudo_loup);

protected:

	/**
	 * \brief The NLP problem.
	 */
	const System& sys;

	bool trace;
};

} /* namespace ibex */

#endif /* __IBEX_LOUP_CORRECTION_H__ */
