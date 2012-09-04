//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcSubBox.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 3, 2012
// Last Update : Sep 3, 2012
//============================================================================

#include "ibex_CtcSubBox.h"

namespace ibex {

CtcSubBox::CtcSubBox(Ctc& c, const BoolMask& m) : Ctc(m.size()), ctc(c), mask(m), y(m.nb_set()) {
	if (!c.nb_var==mask.nb_set()) ibex_error("CtcSubBox: the number of active variables in the mask does not match the one expected by the contractor");
}

void CtcSubBox::contract(IntervalVector& x) {

	assert(x.size()==mask.size());

	int j=0;
	for (int i=0; i<mask.size(); i++)
		if (mask[i]) y[j++]=x[i];

	try {
		ctc.contract(y);
	} catch(EmptyBoxException& e) {
		x.set_empty();
		throw e;
	}

	j=0;
	for (int i=0; i<mask.size(); i++)
		if (mask[i]) x[i]=y[j++];
}

bool CtcSubBox::idempotent() {
	return ctc.idempotent();
}

} // end namespace ibex
