//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_CTC_POINT_IN_SEGMENT_H__
#define __IBEX_CTC_POINT_IN_SEGMENT_H__

#include "ibex_IntervalVector.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;

namespace ibex {

/**
 * \ingroup geometry
 *
 * Minimal contractor for a segment */
class CtcSegment : public Ctc {

public:
	/**
	 * \brief create the contracteur for the segment [a, b] where
	 *	a = (ax, ay) and b = (bx, by)
	 *	This contractor is associated with the set of all point which
	 *	are on the segment.
	 */
    CtcSegment(double ax, double ay, double bx, double by);

	/**
	 * \brief Contract a box.
	 */
    virtual void contract(IntervalVector& box);

	/**
	 * \brief Contracts the input box on the segment [a, b] passed as parameters
	 */
    void contract(IntervalVector &box, double ax, double ay, double bx, double by);

	/**
	 * \brief Delete this.
	 */
    ~CtcSegment();

protected:
    /** Box which containts the box to be contracted and the segment parameters  */
    IntervalVector X_with_params;

    /** Constraint used by the contractor : the point must belong the the line and 
    	to the box which encloses the segment */
    NumConstraint *ncf, *ncg;

    /** Contractors associated to ncf and ncg. */
    CtcFwdBwd *ctc_f, *ctc_g;
};

} // end namespace

#endif // __IBEX_CTC_POINT_IN_SEGMENT_H__
