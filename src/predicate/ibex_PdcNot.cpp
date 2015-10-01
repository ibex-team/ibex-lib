//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcNot.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 12, 2014
//============================================================================

#include "ibex_PdcNot.h"

namespace ibex {

PdcNot::PdcNot(Pdc& p) : Pdc(p.nb_var), p(p) {
}

BoolInterval PdcNot::test(const IntervalVector& box) {
	switch(p.test(box).value) {
	case YES: return NO;
	case NO: return YES;
	default: return MAYBE;
	}
}

} // end namespace ibex

