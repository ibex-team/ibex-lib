//============================================================================
//                                  I B E X                                   
// File        : Composition of contractors
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
		ratio(ratio), impacts(new BoolMask[list.size()]) {

	for (int i=0; i<list.size(); i++) {
		assert(list[i].nb_var==nb_var);
		impacts[i].resize(nb_var);
		impacts[i].set_all();
	}
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, bool incremental, double ratio) :
		Ctc(c1.nb_var), list(Array<Ctc>(c1,c2)), incremental(incremental),
		ratio(ratio), impacts(new BoolMask[list.size()]) {

	for (int i=0; i<list.size(); i++) {
		assert(list[i].nb_var==nb_var);
		impacts[i].resize(nb_var);
		impacts[i].set_all();
	}
}

CtcCompo::~CtcCompo() {
	delete[] impacts;
}

void CtcCompo::contract(IntervalVector& box) {
	for (int i=0; i<list.size(); i++) {
		list[i].contract(box);
	}
}

void CtcCompo::contract(IntervalVector& box, const BoolMask& impact) {
	if (!incremental) {
		contract(box);
		return;
	}

	for (int i=0; i<list.size(); i++) {
		IntervalVector old_box(box);
		impacts[i] |= impact;

		list[i].contract(box);

		for (int j=0; j<nb_var; j++) {
			if (old_box[j].rel_distance(box[j])>ratio)
				for (int i2=0; i2<list.size(); i2++)
					if (i2!=i) impacts[i2].set(j);
		}
	}

}

} // end namespace ibex
