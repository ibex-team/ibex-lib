//============================================================================
//                                  I B E X                                   
// File        : ibex_InnerInflator.h
// Author      : Gilles Chabert, Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_INNER_INFLATOR_H__
#define __IBEX_INNER_INFLATOR_H__

#include "ibex_IntervalVector.h"

namespace ibex {

class InnerInflator {
public:

	IntervalVector inner_box(const IntervalVector& box, const Interval& root);

	InnerInflator();
	virtual ~InnerInflator();
};

} // end namespace ibex
#endif // __IBEX_INNER_INFLATOR_H__
