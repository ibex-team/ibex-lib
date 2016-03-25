//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_CTC_SEGMENT_H__
#define __IBEX_CTC_SEGMENT_H__

#include "ibex_IntervalVector.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

/**
 * \ingroup geometry
 *
 * Minimal contractor for a segment
 */
class CtcSegment : public Ctc {

public:
	/**
	 * \brief create the contractor for a fixed segment.
	 *
	 * The segment is [a, b] where a = (ax, ay) and b = (bx, by).
	 *
	 * This contractor is associated with the set of all points which
	 * are on the segment, which is fixed. It works with 2 variables.
	 */
	CtcSegment(double ax, double ay, double bx, double by);

	/**
	 * \brief create the contractor for "x in a segment [a,b]".
	 *
	 * In this constructor, x, a and b are considered as variables.
	 * The segment is not fixed and may have uncertain endpoints.
	 *
	 * This contractor works with 6 variables.
	 * [gch]: question: is it still optimal in this case??
	 */
	CtcSegment();

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Delete this.
	 */
	~CtcSegment();

protected:
	/** Box which contains the box to be contracted and the segment parameters.
	 * Only used when the segment is fixed.*/
	IntervalVector X_with_params;

	/** Constraint used by the contractor : the point must belong the the line and
    	to the box which encloses the segment */
	NumConstraint *ncf, *ncg;

	/** Contractors associated to ncf and ncg. */
	CtcFwdBwd *ctc_f, *ctc_g;

private:
	/* Initialize ncf, ncg, ctc_f, ctc_g. */
	void init();
};

} // end namespace

#endif // __IBEX_CTC_SEGMENT_H__
