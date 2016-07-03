//============================================================================
//                                  I B E X
// File        : ibex_SepNot.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 12, 2014
//============================================================================

#include "ibex_SepNot.h"

namespace ibex {

SepNot::SepNot(Sep& sep) : Sep(sep.nb_var), sep(sep) {

}

void SepNot::separate(IntervalVector& x_in, IntervalVector& x_out) {
	sep.separate(x_out,x_in);
}

} // namespace ibex
