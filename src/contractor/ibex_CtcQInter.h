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
 * \ingroup contractor
 * \brief Q-intersection contractor.
 *
 */
class CtcQInter : public Ctc {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 *
	 * The list itself is not kept by reference.
	 */
	CtcQInter(const Array<Ctc>& list, int q);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Add sub-contractors properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/**
	 * List of contractors
	 */
	Array<Ctc> list;

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
