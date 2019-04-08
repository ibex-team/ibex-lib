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
 * \brief FixPoint of a contractor
 *
 */
class CtcFixPoint : public Ctc {
public:
	/**
	 * \brief build a fix point.
	 *
	 * When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fix-point is considered to be reached.
	 */
	CtcFixPoint(Ctc& ctc, double ratio=default_ratio);

	/**
	 * \brief Delete *this.
	 */
	virtual ~CtcFixPoint();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 *
	 * \note When the FIXPOINT output flag is set, this means a stronger
	 *       property (the fixpoint is reached for a null ratio).
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add sub-contractor properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/** The sub-contractor */
	Ctc& ctc;

	/**
	 * Ratio. When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fix-point is considered to be reached.
	 */
	double ratio;

	/** Default ratio used, set to 0.1. */
	static constexpr double default_ratio = 0.1;
};

} // end namespace ibex
#endif // __IBEX_CTC_FIX_POINT_H__
