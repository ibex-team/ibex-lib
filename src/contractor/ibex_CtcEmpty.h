//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEmpty.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CTC_EMPTY_H__
#define __IBEX_CTC_EMPTY_H__

#include "ibex_Ctc.h"
#include "ibex_Pdc.h"
#include "ibex_PdcYes.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Empty contractor
 *
 * This contractor contracts any box to the empty box (and
 * raises an EmptyBoxException) iff the predicate returns
 * YES on this box. Otherwise, nothing happens.
 *
 */
class CtcEmpty : public Ctc {
public:


	/**
	 * \brief Build the contractor for n-dimentional box.
	 */
	CtcEmpty(int n);

	/**
	 * \brief Build the contractor for the predicate \a pdc.
	 */
	CtcEmpty(Pdc& pdc, bool own=false);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/**
	 * \brief Delete this.
	 */
	~CtcEmpty();

	/**
	 * \brief The predicate.
	 */
	Pdc& pdc;


private:
	bool own_pdc;
};

} // end namespace ibex

#endif // __IBEX_CTC_EMPTY_H__
