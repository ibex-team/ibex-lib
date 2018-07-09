//============================================================================
//                                  I B E X
// File        : ibex_Bisection.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 7, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_BisectionPoint.h"

#ifndef __IBEX_BISECTION_H__
#define __IBEX_BISECTION_H__

namespace ibex {

/**
 * \ingroup bsc
 *
 * \brief Box bisection.
 */
class Bisection {
public:
	Bisection(const IntervalVector& box, const BisectionPoint& pt, const IntervalVector& left, const IntervalVector& right);

	/** The box before bisection. */
	const IntervalVector& box;

	/** The bisection point. */
	const BisectionPoint& pt;

	/** The left box after bisection. */
	const IntervalVector& left;

	/** The right box after bisection. */
	const IntervalVector& right;
};

inline Bisection::Bisection(const IntervalVector& box, const BisectionPoint& pt, const IntervalVector& left, const IntervalVector& right) :
		box(box), pt(pt), left(left), right(right) {

}

} /* namespace ibex */

#endif /* __IBEX_BISECTION_H__ */
