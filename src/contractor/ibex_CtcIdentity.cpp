//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcIdentity.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 18, 2013
// Last Update : Jul 18, 2013
//============================================================================

#include "ibex_CtcIdentity.h"

namespace ibex {

CtcIdentity::CtcIdentity(int n) : Ctc(n) { }

void CtcIdentity::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcIdentity::contract(IntervalVector& box, CtcContext& context) {
	context.set_flag(CtcContext::FIXPOINT);
	context.set_flag(CtcContext::INACTIVE);
}

} // end namespace ibex
