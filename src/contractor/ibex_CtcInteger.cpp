//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcInteger.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 20, 2012
//============================================================================

#include "ibex_CtcInteger.h"

namespace ibex {

CtcInteger::~CtcInteger() {
	delete input;
	delete output;
}

CtcInteger::CtcInteger(int nb_var, const BoolMask& is_int) : Ctc(nb_var), is_int(is_int) {
	input = new BoolMask(nb_var);
	output = new BoolMask(nb_var);

	for (int v=0; v<nb_var; v++)
		(*output)[v]=(*input)[v]=is_int[v];
}

void CtcInteger::contract(IntervalVector& box) {
	for (int i=0; i<nb_var; i++) {
		if (!is_int[i]) continue;
		proj_integer(box[i]);
		if (box[i].is_empty()) {
			box.set_empty();
			throw EmptyBoxException();
		}
	}
}

void CtcInteger::contract(IntervalVector& box, const BoolMask& impact) {
	for (int i=0; i<nb_var; i++) {
		if (is_int[i] && impact[i]) {
			proj_integer(box[i]);
			if (box[i].is_empty()) {
				box.set_empty();
				throw EmptyBoxException();
			}
		}
	}
}

bool CtcInteger::idempotent() {
	return true;
}

} // end namespace ibex
