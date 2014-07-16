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

CtcQInter::CtcQInter(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


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

CtcQInter2::CtcQInter2(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInter2::contract(IntervalVector& box) {
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

	box = qinter2(refs,q);

	if (box.is_empty()) throw EmptyBoxException();
}

CtcQInterProjF::CtcQInterProjF(const Array<Ctc>& list, int q) : Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInterProjF::contract(IntervalVector& box) {
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

	box = qinter_projf(refs,q);

	if (box.is_empty()) throw EmptyBoxException();
}

CtcQInterCoreF::CtcQInterCoreF(const Array<Ctc>& list, int q) :  Ctc(list), list(list), q(q), boxes(list.size(), nb_var) { }


void CtcQInterCoreF::contract(IntervalVector& box) {
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

	box = qinter_coref(refs,q);

	if (box.is_empty()) throw EmptyBoxException();
}

} // end namespace ibex
