//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcCompo.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_CtcCompo.h"

namespace ibex {

/*! Default composition ratio. */
const double CtcCompo::default_ratio = 0.1;

CtcCompo::CtcCompo(const Array<Ctc>& list, bool incremental, double ratio) :
		Ctc(list[0].nb_var), list(list), incremental(incremental),
		ratio(ratio), idc(new Indicators*[list.size()]) {

	for (int i=0; i<list.size(); i++) {
		assert(list[i].nb_var==nb_var);
		idc[i]=new Indicators(nb_var);
		idc[i]->impact.set_all();
	}
}

void CtcCompo::contract(IntervalVector& box) {
	for (int i=0; i<list.size(); i++) {
		list[i].contract(box);
	}
}

void CtcCompo::contract(IntervalVector& box, const Indicators& _idc) {
	if (!incremental) {
		contract(box);
		return;
	}
	// rem: we cannot take the scope into account.
	for (int i=0; i<list.size(); i++) {
		IntervalVector old_box(box);
		idc[i]->impact |= _idc.impact;

		list[i].contract(box);

		for (int j=0; j<nb_var; j++) {
			if (old_box[j].rel_distance(box[j])>ratio)
				for (int i2=0; i2<list.size(); i2++)
					if (i2!=i) idc[i2]->impact.set(j);
		}
	}

}

} // end namespace ibex
