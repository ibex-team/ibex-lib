//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcOr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_PdcOr.h"

namespace ibex {

PdcOr::PdcOr(const Array<Pdc>& list) : Pdc(list[0].nb_var), list(list) {
	for (int i=1; i<list.size(); i++) {
		assert(list[i].nb_var==nb_var);
	}
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2) : Pdc(c1.nb_var), list(2) {
	list.set_ref(0,c1);
	assert(c2.nb_var==nb_var);
	list.set_ref(1,c2);
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3) : Pdc(c1.nb_var), list(3) {
	list.set_ref(0,c1);
	assert(c2.nb_var==nb_var);
	list.set_ref(1,c2);
	assert(c3.nb_var==nb_var);
	list.set_ref(2,c3);
}


BoolInterval PdcOr::test(const IntervalVector& box) {
	assert(list.size()>0);
	BoolInterval res=list[0].test(box);
	for (int i=1; i<list.size(); i++) {
		res |= list[i].test(box);
	}
	return res;
}

} // end namespace ibex
