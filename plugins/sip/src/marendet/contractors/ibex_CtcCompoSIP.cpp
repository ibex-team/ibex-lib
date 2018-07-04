//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcCompoSIP.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_CtcCompoSIP.h"

#include "ibex_BitSet.h"
#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"

using namespace std;

namespace ibex {

/*! Default composition ratio. */

void CtcCompoSIP::init_impacts() {

//	impacts = new BoolMask[list.size()];
//
//	for (int i=0; i<list.size(); i++) {
//		assert(list[i].nb_var==nb_var);
//		impacts[i].resize(nb_var);
//		impacts[i].set_all();
//	}
}

CtcCompoSIP::CtcCompoSIP(const Array<CellCtc>& list, bool incremental,
		double ratio) :
		CellCtc(list), list(list), incremental(incremental), ratio(ratio) {

	init_impacts();
}

CtcCompoSIP::~CtcCompoSIP() {
//	delete[] impacts;
}

void CtcCompoSIP::contractCell(Cell& cell) {

	// TODO: wrong algorithm here
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
	bool inactive = true;
	BitSet flags(BitSet::empty(CellCtc::NB_OUTPUT_FLAGS));
	BitSet impact(BitSet::all(nb_var)); // always set to "all" for the moment (to be improved later)
	for (int i = 0; i < list.size(); i++) {
		if (inactive) {
			flags.clear();
			list[i].contractCell(cell, impact, flags);
			if (!flags[INACTIVE])
				inactive = false;
		} else {
			list[i].contractCell(cell);
		}

		if (cell.box.is_empty()) {
			set_flag(FIXPOINT);
			return;
		}
	}
	if (inactive)
		set_flag(INACTIVE);
}

} // end namespace ibex
