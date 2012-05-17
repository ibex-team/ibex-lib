//============================================================================
//                                  I B E X                                   
// File        : Composition of contractors
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#ifndef __IBEX_CTC_COMPO_H__
#define __IBEX_CTC_COMPO_H__

#include "ibex_Ctc.h"
#include "ibex_Array.h"

namespace ibex {

/** \ingroup contractor
 * \brief Composition of contractors
 *
 * For a box [x] the composition of {c_0,...c_n} performs
 * c_n(...(c_1(c_0([x])))).
 */
class CtcCompo : public Ctc {
public:
	/**
	 * \brief build a composition
	 *
	 * If incremental is true, manages the impact.
	 */
	CtcCompo(const Array<Ctc>& list, bool incremental=false, double ratio=default_ratio);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Contract a box with indicators.
	 */
	virtual void contract(IntervalVector& box, const Indicators& idc);

	/** The list of sub-contractors */
	Array<Ctc> list;

	/** Incremental mode? */
	bool incremental;

	/** Ratio used in incremental mode */
	double ratio;

	/** Default ratio used in incremental mode, set to 0.1. */
	static const double default_ratio;

protected:
	Indicators **idc;
};

} // end namespace ibex
#endif // __IBEX_CTC_COMPO_H__
