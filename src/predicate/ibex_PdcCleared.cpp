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
	// TODO Auto-generated constructor stub

}

BoolInterval PdcCleared::test(const IntervalVector& box) {
	try {
		IntervalVector tmpbox(box);
		ctc.contract(tmpbox);
		assert(!tmpbox.is_empty());
		return MAYBE;
	} catch(EmptyBoxException& ) {
		return YES;
	}
}

} // end namespace ibex
