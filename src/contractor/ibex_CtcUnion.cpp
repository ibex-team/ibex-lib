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

CtcUnion::CtcUnion(const Array<Ctc>& list) : Ctc(list[0].nb_var), list(list) {
	for (int i=1; i<list.size(); i++) {
		assert(list[i].nb_var==nb_var);
	}
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2) : Ctc(c1.nb_var), list(2) {
	list.set_ref(0,c1);
	assert(c2.nb_var==nb_var);
	list.set_ref(1,c2);
}

CtcUnion::CtcUnion(Ctc& c1, Ctc& c2, Ctc& c3) : Ctc(c1.nb_var), list(3) {
	list.set_ref(0,c1);
	assert(c2.nb_var==nb_var);
	list.set_ref(1,c2);
	assert(c3.nb_var==nb_var);
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
