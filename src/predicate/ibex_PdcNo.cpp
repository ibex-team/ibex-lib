//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcNo.cpp
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 10, 2014
// Last Update : July 10, 2014
//============================================================================


#include "ibex_PdcNo.h"

namespace ibex {

PdcNo::PdcNo(int n) : Pdc(n) {
}

BoolInterval PdcNo::test(const IntervalVector& box) {
		return NO;
}

} // end namespace ibex
