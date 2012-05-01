//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Apr 30, 2012
//============================================================================

#include "ibex_CtcQInter.h"
#include "ibex_QInter.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

CtcQInter::CtcQInter(const Array<Ctc>& list, int q) : Ctc(list[0].nb_var), list(list),
		n(list[0].nb_var), q(q), boxes(list.size(), n) {

	for (int i=0; i<list.size(); i++) {
		assert(list[i].nb_var==n);
	}
}

void CtcQInter::contract(IntervalVector& box) {
	Array<IntervalVector> refs(list.size());

	for (int i=0; i<list.size(); i++) {
		try {
			boxes[i]=box;
			list[i].contract(boxes[i]);
		} catch(EmptyBoxException&) {
			assert(boxes[i].is_empty());
		}
		refs.set_ref(i,boxes[i]);
	}

	box = qinter(refs,q);

	if (box.is_empty()) throw EmptyBoxException();
}

} // end namespace ibex
