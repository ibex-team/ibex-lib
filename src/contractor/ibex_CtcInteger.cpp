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

CtcInteger::CtcInteger(int nb_var, const BitSet& is_int) : Ctc(nb_var), is_int(is_int) { // warning is_int.size()<>nb_var
	assert(nb_var>0);
	assert(is_int.max()<nb_var);

	input = new BitSet(is_int);
	output = new BitSet(is_int);
}

void CtcInteger::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcInteger::contract(IntervalVector& box, ContractContext& context) {

	assert(box.size()==nb_var);

	for (int i=0; i<nb_var; i++) {
		if (is_int[i] && context.impact[i]) {
			if (!bwd_integer(box[i])) {
				box.set_empty();
				context.output_flags.add(FIXPOINT);
				return;
			}
		}
	}
}

bool CtcInteger::idempotent() {
	return true;
}

} // end namespace ibex
