//============================================================================
//                                  I B E X                                   
// File        : ibex_SepBoundaryCtc.h
// Author      : Gilles Chabert, Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#ifndef __IBEX_SEP_BOUNDARY_CTC_H__
#define __IBEX_SEP_BOUNDARY_CTC_H__

#include "ibex_Sep.h"
#include "ibex_Ctc.h"
#include "ibex_Pdc.h"
#include "ibex_BoolInterval.h"

namespace ibex {

/**
 * \ingroup set
 *
 * \brief A Separator based on a boundary contractor and a membership predicate
 *
 * Given the set S associated to this separator:
 *
 * The contractor C contracts a box [x] verifies:
 *    ([x] \ C([x])) \subseteq S or ([x] \ C([x])) \subseteq complementary(S)
 *
 * The predicate P tests a vector x verifies:
 *  - P(x) is YES only if x is inside S
 *  - P(x) is NO only if it is outside S
 *  - P(x) is MAYBE if nothing could be proven
 *
 * \note The predicated is a Pdc object even though only vectors (and not boxes)
 * will be passed in argument. The reason is that interval computations
 * will have to be performed anyway by the predicate to provide guaranteed
 * results.
 *
 */
class SepBoundaryCtc : public Sep {

public:
	/**
	 * \brief Build the separator from a contractor and a predicate.
	 *
	 */
	SepBoundaryCtc(Ctc& ctc_boundary, Pdc& is_inside);

	/**
	 * \brief Separate a box.
	 *
	 * \see ibex::Sep::separate(IntervalVector&, IntervalVector&).
	 */
	virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:

	/**
	 * Number of sample points used to check if a box is
	 * inside or outside the set. The first sample point
	 * is the midpoint.
	 */
	static constexpr int NB_SAMPLES = 1;

	/** Boundary contractor. */
	Ctc& ctc_boundary;

	/** Membership predicate. */
	Pdc& is_inside;

};

} // namespace ibex

#endif // __IBEX_SEP_BOUNDARY_CTC_H__
