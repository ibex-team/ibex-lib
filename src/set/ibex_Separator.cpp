//============================================================================
//                                  I B E X                                   
// File        : ibex_Separator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 21, 2014
//============================================================================

#include "ibex_Separator.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

Separator::Separator(Function& f) :
		ctc_in(*new CtcFwdBwd(*new NumConstraint(f,GT))),
		ctc_out(*new CtcFwdBwd(*new NumConstraint(f,LEQ))),
		own_ctc(true) {

}

Separator::Separator(Ctc& _ctc_in, Ctc& _ctc_out) : ctc_in(_ctc_in), ctc_out(_ctc_out), own_ctc(false) {

}

Separator::~Separator() {
	if (own_ctc) {
		delete &ctc_in;
		delete &ctc_out;
	}
}

} // namespace ibex
