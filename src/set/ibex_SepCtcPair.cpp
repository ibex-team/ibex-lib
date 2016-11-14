//============================================================================
//                                  I B E X
// File        : ibex_SepCtcPair.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 17, 2014
//============================================================================

#include "ibex_SepCtcPair.h"
#include <cassert>

namespace ibex {

SepCtcPair::SepCtcPair(Ctc& _ctc_in, Ctc& _ctc_out) : Sep(_ctc_in.nb_var), ctc_in(_ctc_in), ctc_out(_ctc_out) {

}

void SepCtcPair::separate(IntervalVector& x_in, IntervalVector& x_out) {

	assert(x_in==x_out);

	ctc_out.contract(x_out);

	ctc_in.contract(x_in);

}

} // namespace ibex
