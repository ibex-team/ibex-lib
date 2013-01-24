//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcCleared.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_PDC_CLEARED_H__
#define __IBEX_PDC_CLEARED_H__

#include "ibex_Pdc.h"
#include "ibex_Ctc.h"

namespace ibex {

class PdcCleared : public Pdc {
public:
	PdcCleared(Ctc& c);

	/**
	 * \brief Apply the predicate to the given box.
	 */
	virtual BoolInterval test(const IntervalVector& box);

	Ctc& ctc;
};

} // end namespace ibex
#endif // IBEX_PDCCLEARED_H_
