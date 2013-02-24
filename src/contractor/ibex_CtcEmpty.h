//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEmpty.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CTC_EMPTY_H__
#define __IBEX_CTC_EMPTY_H__

#include "ibex_Ctc.h"

namespace ibex {

/** \ingroup contractor
 * \brief Empty contractor
 *
 * This contractor contracts any box to the empty box (and
 * raises an EmptyBoxException).
 *
 */
class CtcEmpty : public Ctc {
public:

	CtcEmpty(int n);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);
};

} // end namespace ibex
#endif // __IBEX_CTC_EMPTY_H__
