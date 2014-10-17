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
 * TODO: add comment
 */
class CtcSegment : public Ctc {

public:
	/**
	 * \brief TODO: add comment
	 */
    CtcSegment(double ax, double ay, double bx, double by);

	/**
	 * \brief Contract a box.
	 */
    virtual void contract(IntervalVector& box);

	/**
	 * \brief TODO: add comment
	 */
    void contract(IntervalVector &box, double ax, double ay, double bx, double by);

	/**
	 * \brief Delete this.
	 */
    ~CtcSegment();

protected:
    /** TODO: add comment */
    IntervalVector X_with_params;

    /** TODO: add comment */
    NumConstraint *ncf, *ncg;

    /** Contractors associated to ncf and ncg. */
    CtcFwdBwd *ctc_f, *ctc_g;
};

} // end namespace

#endif // __IBEX_CTC_POINT_IN_SEGMENT_H__
