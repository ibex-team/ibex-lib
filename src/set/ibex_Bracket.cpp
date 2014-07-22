//============================================================================
//                                  I B E X                                   
// File        : ibex_Bracket.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 21, 2014
//============================================================================

#include "ibex_Bracket.h"
#include "ibex_CtcFwdBwd.h"

namespace ibex {

Bracket::Bracket(Function& f) :
		ctc_in(*new CtcFwdBwd(*new NumConstraint(f,GT))),
		ctc_out(*new CtcFwdBwd(*new NumConstraint(f,LEQ))) {

}

Bracket::~Bracket() {
	// TODO
}

} // namespace ibex
