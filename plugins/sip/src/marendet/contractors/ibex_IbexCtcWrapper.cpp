//============================================================================
//                                  I B E X                                   
// File        : ibex_IbexCtcWrapper.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_IbexCtcWrapper.h"

#include "ibex_Cell.h"

using namespace std;

namespace ibex {

IbexCtcWrapper::IbexCtcWrapper(Ctc& ctc, bool own_ctc)
: CellCtc(ctc.nb_var), ctc_(ctc), own_ctc_(own_ctc) {
	// TODO Auto-generated constructor stub

}

IbexCtcWrapper::~IbexCtcWrapper() {
	if(own_ctc_) {
		delete &ctc_;
	}
}

void IbexCtcWrapper::contractCell(Cell& cell) {
	ctc_.contract(cell.box);
}

/*
void IbexCtcWrapper::contractCell(Cell& cell,
		const BitSet& impact) {
	ctc_.contract(cell.box, impact);
}

void IbexCtcWrapper::contractCell(Cell& cell, const BitSet& impact,
		BitSet& flags) {
	ctc_.contract(cell.box, impact, flags);
}
*/
} // end namespace ibex
