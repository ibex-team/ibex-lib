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

void CtcCompo::init_impacts() {

//	impacts = new BoolMask[list.size()];
//
//	for (int i=0; i<list.size(); i++) {
//		assert(list[i].nb_var==nb_var);
//		impacts[i].resize(nb_var);
//		impacts[i].set_all();
//	}
}

CtcCompo::CtcCompo(const Array<Ctc>& list, bool incremental, double ratio) :
	Ctc(list[0].nb_var), list(list), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, bool incremental, double ratio) :
	Ctc(c1.nb_var), list(Array<Ctc>(c1,c2)), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, bool incremental, double ratio) :
	Ctc(c1.nb_var), list(Array<Ctc>(c1,c2,c3)), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}


CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, bool incremental, double ratio) :
	Ctc(c1.nb_var), list(Array<Ctc>(c1,c2,c3,c4)), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, bool incremental, double ratio) :
	Ctc(c1.nb_var), list(Array<Ctc>(c1,c2,c3,c4,c5)), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}

CtcCompo::CtcCompo(Ctc& c1, Ctc& c2, Ctc& c3, Ctc& c4, Ctc& c5, Ctc& c6, bool incremental, double ratio) :
	Ctc(c1.nb_var), list(Array<Ctc>(c1,c2,c3,c4,c5,c6)), incremental(incremental),
	ratio(ratio) {

	init_impacts();
}


CtcCompo::~CtcCompo() {
//	delete[] impacts;
}


void CtcCompo::contract(IntervalVector& box) {

//	if (incremental) {
//		for (int i=0; i<list.size(); i++) {
//			IntervalVector old_box(box);
//			impacts[i] |= impact;
//
//			list[i].contract(box);
//
//			for (int j=0; j<nb_var; j++) {
//				if (old_box[j].rel_distance(box[j])>ratio)
//					for (int i2=0; i2<list.size(); i2++)
//						if (i2!=i) impacts[i2].set(j);
//			}
//		}
//		return;
//	}

	for (int i=0; i<list.size(); i++) {
		list[i].contract(box);
	}

}

} // end namespace ibex
