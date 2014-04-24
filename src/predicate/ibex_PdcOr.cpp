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

PdcOr::PdcOr(const Array<Pdc>& list) : list(list) {
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2) : list(2) {
	list.set_ref(0,c1);
	list.set_ref(1,c2);
}

PdcOr::PdcOr(Pdc& c1, Pdc& c2, Pdc& c3) : list(3) {
	list.set_ref(0,c1);
	list.set_ref(1,c2);
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
