//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcIdentity.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CTC_IDENTITY_H__
#define __IBEX_CTC_IDENTITY_H__

#include "ibex_Ctc.h"

namespace ibex {

/** \ingroup contractor
 * \brief Identity contractor
 *
 */
class CtcIdentity : public Ctc {
public:

	/**
	 *  \brief Create a contractor which not modified the boxes
	 */
	CtcIdentity(int n);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);
};

} // end namespace ibex
#endif // __IBEX_CTC_IDENTITY_H__
