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

CtcFixPoint::CtcFixPoint(Ctc& ctc, double ratio) : Ctc(ctc.nb_var), ctc(ctc), ratio(ratio) {

}

CtcFixPoint::~CtcFixPoint(){

}

void CtcFixPoint::add_property(const IntervalVector& init_box, BoxProperties& map) {
	ctc.add_property(init_box, map);
}

void CtcFixPoint::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcFixPoint::contract(IntervalVector& box, ContractContext& context) {
	IntervalVector init_box(box);
	IntervalVector old_box(box);

	BitSet input_impact(context.impact); // saved-->useful?
	BitSet& flags=context.output_flags;

	do {
		old_box=box;

		flags.clear();

		ctc.contract(box,context);

		if (box.is_empty()) {
			flags.add(FIXPOINT);
			break;
		}

		// calculate impact for next call
		context.impact.clear();
		for (int i=0; i<nb_var; i++) {
			if (box[i]!=old_box[i]) context.impact.add(i);
		}

	} while (!flags[FIXPOINT] && !flags[INACTIVE] && old_box.rel_distance(box)>ratio);

	bool fixpoint=flags[FIXPOINT];
	bool inactive=flags[INACTIVE];

	flags.clear();
	if (fixpoint) flags.add(FIXPOINT);
	if (inactive && init_box==box) flags.add(INACTIVE);
}

} // end namespace ibex
