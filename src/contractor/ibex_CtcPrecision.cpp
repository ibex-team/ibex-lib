//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPrecision.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_CtcPrecision.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

CtcPrecision::CtcPrecision(int nb_var, double ceil) : Ctc(nb_var), ceil(ceil) {

}

CtcPrecision::~CtcPrecision() {

}

void CtcPrecision::contract(IntervalVector& box) {
	if (box.max_diam()<=ceil) {
		box.set_empty();
		throw EmptyBoxException();
	}
}

bool CtcPrecision::idempotent() {
	return true;
}

} // end namespace ibex
