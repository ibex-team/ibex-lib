//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFixPointSIP.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_CtcFixPointSIP.h"

#include "ibex_BitSet.h"
#include "ibex_Cell.h"
#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"

namespace ibex {

/*! Default composition ratio. */

CtcFixPointSIP::CtcFixPointSIP(CellCtc& ctc, double ratio) : CellCtc(ctc.nb_var), ctc(ctc), ratio(ratio) {
}

CtcFixPointSIP::~CtcFixPointSIP(){
}

void CtcFixPointSIP::contractCell(Cell& cell) {
	IntervalVector init_box(cell.box);
	IntervalVector old_box(cell.box);
	BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));
	BitSet impact(BitSet::all(nb_var)); // always set to "all" for the moment (to be improved later)

	do {
		old_box=cell.box;
		ctc.contractCell(cell,impact,flags);

		if (cell.box.is_empty()) {
			set_flag(FIXPOINT);
			return;
		}

	} while (!flags[FIXPOINT] && !flags[INACTIVE] && old_box.rel_distance(cell.box)>ratio);

	if (flags[FIXPOINT]) set_flag(FIXPOINT);
	if (flags[INACTIVE] && init_box==cell.box) set_flag(INACTIVE);
}

} // end namespace ibex
