//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcCleared.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#include "ibex_PdcCleared.h"
#include <cassert>

namespace ibex {

PdcCleared::PdcCleared(Ctc& c) : Pdc(c.nb_var), ctc(c) {

}

BoolInterval PdcCleared::test(const IntervalVector& box) {
	IntervalVector tmpbox(box);
	ctc.contract(tmpbox);
	return tmpbox.is_empty() ? YES : MAYBE;
}

} // end namespace ibex
