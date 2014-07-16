//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcYes.cpp
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 10, 2014
// Last Update : July 10, 2014
//============================================================================


#include "ibex_PdcYes.h"

namespace ibex {

PdcYes::PdcYes(int n) : Pdc(n) {
}

BoolInterval PdcYes::test(const IntervalVector& box) {
		return YES;
}

} // end namespace ibex
