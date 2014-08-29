//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcNo.h
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 10, 2014
// Last Update : July 10, 2014
//============================================================================

#ifndef __IBEX_PDC_NO_H__
#define __IBEX_PDC_NO_H__

#include "ibex_Pdc.h"

namespace ibex {

/** \ingroup predicate
 * \brief predicate which return YES every time
 *
 */
class PdcNo : public Pdc {
public:


	PdcNo(int n);

	/**
	 * \brief Return YES
	 */
	BoolInterval test(const IntervalVector& box);

};

} // end namespace ibex
#endif // __IBEX_PDC_NO_H__
