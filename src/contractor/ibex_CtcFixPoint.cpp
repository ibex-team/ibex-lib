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

void CtcFixPoint::add_property(const IntervalVector& init_box, BoxProperties& map) {
	ctc.add_property(init_box, map);
}

void CtcFixPoint::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcFixPoint::contract(IntervalVector& box, CtcContext& context) {
	IntervalVector init_box(box);
	IntervalVector old_box(box);

	CtcContext subcontext;
	BitSet impact(nb_var);
	if (context.impact())
		impact = *context.impact();
	else
		impact.fill(0,nb_var-1);
	subcontext.set_impact(&impact);
	BitSet flags(nb_var);
	subcontext.set_output_flags(&flags);
	subcontext.set_properties(context.data(),true);

	do {
		old_box=box;

		flags.clear();

		ctc.contract(box,subcontext);

		if (box.is_empty()) {
			context.set_flag(CtcContext::FIXPOINT);
			return;
		}

		// calculate impact for next call
		impact.clear();
		for (int i=0; i<nb_var; i++) {
			if (box[i]!=old_box[i]) impact.add(i);
		}

	} while (!flags[CtcContext::FIXPOINT] && !flags[CtcContext::INACTIVE] && old_box.rel_distance(box)>ratio);

	if (flags[CtcContext::FIXPOINT]) context.set_flag(CtcContext::FIXPOINT);
	if (flags[CtcContext::INACTIVE] && init_box==box) context.set_flag(CtcContext::INACTIVE);
}

} // end namespace ibex
