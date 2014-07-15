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


CtcEmpty::CtcEmpty(int n) : Ctc(n), pdc(*new PdcYes(n)), own_pdc(true) { }

CtcEmpty::CtcEmpty(Pdc& pdc, bool own) : Ctc(pdc.nb_var), pdc(pdc), own_pdc(own){ }

void CtcEmpty::contract(IntervalVector& box) {
	if (pdc.test(box)==YES) {
		box.set_empty();
		throw EmptyBoxException();
	}
}

CtcEmpty::~CtcEmpty() {
	if (own_pdc) delete &pdc;
}

} // end namespace ibex
