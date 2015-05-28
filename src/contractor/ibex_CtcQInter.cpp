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

namespace ibex {

CtcQInter::CtcQInter(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInter::contract(IntervalVector& box) {
	Array<IntervalVector> refs(list.size());

	for (int i=0; i<list.size(); i++) {
		boxes[i]=box;
		list[i].contract(boxes[i]);
		refs.set_ref(i,boxes[i]);
	}

	box = qinter(refs,q);
}

CtcQInter2::CtcQInter2(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInter2::contract(IntervalVector& box) {
	Array<IntervalVector> refs(list.size());

	for (int i=0; i<list.size(); i++) {
		boxes[i]=box;
		list[i].contract(boxes[i]);
		refs.set_ref(i,boxes[i]);
	}

	box = qinter2(refs,q);
}

CtcQInterProjF::CtcQInterProjF(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInterProjF::contract(IntervalVector& box) {
	Array<IntervalVector> refs(list.size());

	for (int i=0; i<list.size(); i++) {
		boxes[i]=box;
		list[i].contract(boxes[i]);
		refs.set_ref(i,boxes[i]);
	}

	box = qinter_projf(refs,q);
}

CtcQInterCoreF::CtcQInterCoreF(const Array<Ctc>& list, int q) :  Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInterCoreF::contract(IntervalVector& box) {
	Array<IntervalVector> refs(list.size());

	for (int i=0; i<list.size(); i++) {
		boxes[i]=box;
		list[i].contract(boxes[i]);
		refs.set_ref(i,boxes[i]);
	}

	box = qinter_coref(refs,q);
}

} // end namespace ibex
