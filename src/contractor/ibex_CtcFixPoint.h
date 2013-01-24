//============================================================================
//                                  I B E X                                   
// File        : Fix-point of a contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 01, 2012
// Last Update : May 01, 2012
//============================================================================

#ifndef __IBEX_CTC_FIX_POINT_H__
#define __IBEX_CTC_FIX_POINT_H__

#include "ibex_Ctc.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief CtcFixPoint of a contractor
 *
 */
class CtcFixPoint : public Ctc {
public:
	/**
	 * \brief build a CtcFixPoint.
	 *
	 * When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fixpoint is considered to be reached.
	 */
	CtcFixPoint(Ctc& ctc, double ratio=default_ratio);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/** The list of sub-contractors */
	Ctc& ctc;

	/** Ratio. When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fixpoint is considered to be reached.
	 */
	double ratio;

	/** Default ratio used, set to 0.1. */
	static const double default_ratio;
};

} // end namespace ibex
#endif // __IBEX_CTC_FIX_POINT_H__
