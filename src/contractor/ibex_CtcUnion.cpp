//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcUnion.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_CtcUnion.h"

namespace ibex {

CtcUnion::CtcUnion(const Array<Ctc>& list) : list(list) { }

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2) : list(2) {
	list.set_ref(0,c1);
	list.set_ref(1,c2);
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3) : list(3) {
	list.set_ref(0,c1);
	list.set_ref(1,c2);
	list.set_ref(2,c3);
}


void CtcUnion::contract(IntervalVector& box) {
	IntervalVector savebox(box);
	IntervalVector result(IntervalVector::empty(box.size()));

	for (int i=0; i<list.size(); i++) {
		if (i>0) box=savebox;
		try {
			list[i].contract(box);
			result |= box;
		}
		catch(EmptyBoxException&) {
		}
	}
	box = result;
	if (box.is_empty()) throw EmptyBoxException();
} // end namespace ibex

}
