//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcYes.h
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 10, 2014
// Last Update : July 10, 2014
//============================================================================

#ifndef __IBEX_PDC_YES_H__
#define __IBEX_PDC_YES_H__

#include "ibex_Pdc.h"

namespace ibex {

/** \ingroup predicate
 * \brief predicate which return YES every time
 *
 */
class PdcYes : public Pdc {
public:


	PdcYes(int n);

	/**
	 * \brief Return YES
	 */
	BoolInterval test(const IntervalVector& box);

};

} // end namespace ibex
#endif // __IBEX_PDC_YES_H__
