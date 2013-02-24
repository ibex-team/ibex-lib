//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEmpty.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 18, 2013
// Last Update : Jul 18, 2013
//============================================================================

#include "ibex_CtcEmpty.h"

namespace ibex {

CtcEmpty::CtcEmpty(int n) : Ctc(n) { }

void CtcEmpty::contract(IntervalVector& box) {
	box.set_empty();
	throw EmptyBoxException();
}

} // end namespace ibex
