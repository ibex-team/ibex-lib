//============================================================================
//                                  I B E X                                   
// File        : ibex_SepCtcPair.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 17, 2014
//============================================================================

#ifndef __IBEX_SEP_CTC_PAIR_H__
#define __IBEX_SEP_CTC_PAIR_H__

#include "ibex_Sep.h"
#include "ibex_Ctc.h"

namespace ibex {

/**
 * \ingroup set
 *
 * \brief A pair of two independent/complementary contractors
 */
class SepCtcPair : public Sep {
public:
	/**
	 * \brief Build the separator from two contractors.
	 */
	SepCtcPair(Ctc& ctc_in, Ctc& ctc_out);

	/**
	 * \brief Separate a box.
	 *
	 * \see ibex::Sep::separate(IntervalVector&, IntervalVector&).
	 */
	virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

	/** Inner contractor */
	Ctc& ctc_in;
	/** Outer contractor */
	Ctc& ctc_out;

};

} // namespace ibex

#endif // __IBEX_SEP_CTC_PAIR_H__
