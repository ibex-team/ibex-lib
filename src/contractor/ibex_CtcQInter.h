//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Apr 30, 2012
//============================================================================

#ifndef __IBEX_CTC_Q_INTER_H__
#define __IBEX_CTC_Q_INTER_H__

#include "ibex_Ctc.h"
#include "ibex_Array.h"
#include "ibex_IntervalMatrix.h"

namespace ibex {

/**
 * \ingroup level2
 * \brief Q-intersection contractor.
 *
 */
class CtcQInter : public Ctc {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 */
	CtcQInter(const Array<Ctc>& list, int q);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * List of contractors
	 */
	Array<Ctc> list;

	/**
	 * Dimension of the box (number of variables)
	 */
	int n;

	/**
	 * The number of contractors we have to intersect the
	 * result.
	 */
	int q;

protected:
	IntervalMatrix boxes; // store boxes for each contraction
};

} // end namespace ibex
#endif // __IBEX_CTC_Q_INTER_H__
