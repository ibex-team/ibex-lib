//============================================================================
//                                  I B E X                                   
// File        : Fix-point of a contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 01, 2012
// Last Update : May 01, 2012
//============================================================================

#include "ibex_CtcFixPoint.h"

namespace ibex {

/*! Default composition ratio. */
const double CtcFixPoint::default_ratio = 0.1;

CtcFixPoint::CtcFixPoint(Ctc& ctc, double ratio) : Ctc(ctc.nb_var), ctc(ctc), ratio(ratio) {

}

CtcFixPoint::~CtcFixPoint(){

}

void CtcFixPoint::add_property(BoxProperties& map) {
	ctc.add_property(map);
}

void CtcFixPoint::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcFixPoint::contract(IntervalVector& box, CtcContext& context) {
	IntervalVector init_box(box);
	IntervalVector old_box(box);
	BitSet flags(BitSet::empty(CtcContext::NB_OUTPUT_FLAGS));
	BitSet impact(BitSet::all(nb_var)); // always set to "all" for the moment (to be improved later)

	do {
		old_box=box;

		ctc.contract(box,impact,flags);

		if (box.is_empty()) {
			context.set_flag(CtcContext::FIXPOINT);
			return;
		}

	} while (!flags[CtcContext::FIXPOINT] && !flags[CtcContext::INACTIVE] && old_box.rel_distance(box)>ratio);

	if (flags[CtcContext::FIXPOINT]) context.set_flag(CtcContext::FIXPOINT);
	if (flags[CtcContext::INACTIVE] && init_box==box) context.set_flag(CtcContext::INACTIVE);
}

} // end namespace ibex
