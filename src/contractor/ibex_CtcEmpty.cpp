//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEmpty.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 18, 2013
// Last Update : Jul 18, 2013
//============================================================================

#include "ibex_CtcEmpty.h"

namespace ibex {


CtcEmpty::CtcEmpty(int n) : Ctc(n), pdc(*new PdcYes(n)), own_pdc(true) { }

CtcEmpty::CtcEmpty(Pdc& pdc, bool own) : Ctc(pdc.nb_var), pdc(pdc), own_pdc(own){ }

void CtcEmpty::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcEmpty::contract(IntervalVector& box, ContractContext& context) {
	BoolInterval t= pdc.test(box);
	switch (t) {
	case YES:
		box.set_empty();
		context.output_flags.add(FIXPOINT);
		break;
	case NO:
		// The constraint is inactive only if
		// the test is monotonous w.r.t. inclusion
		//	set_flag(INACTIVE);
		break;
	default: break;
	}
}

CtcEmpty::~CtcEmpty() {
	if (own_pdc) delete &pdc;
}

} // end namespace ibex
