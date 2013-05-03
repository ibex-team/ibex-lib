/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Ctc.h"
#include "ibex_Cell.h"

namespace ibex {

Ctc::Ctc(int n) : nb_var(n), input(n), output(n), _impact(NULL), _cell(NULL) {
	input.set_all();   // by default
	output.set_all();  // by default
}

Ctc::~Ctc() {
}

void Ctc::contract(Cell& cell) {
	_cell = &cell;

	try {
		contract(cell.box);
	}
	catch(EmptyBoxException& e) {
		_cell = NULL;
		throw e;
	}
	_cell = NULL;
}

void Ctc::contract(IntervalVector& box, const BoolMask& impact) {
	_impact = &impact;

	try {
		contract(box);
	}
	catch(EmptyBoxException& e) {
		_impact = NULL;
		throw e;
	}

	_impact = NULL;
}

void Ctc::contract(Cell& cell, const BoolMask& impact) {
	_cell = &cell;
	_impact = &impact;

	try {
		contract(cell.box);
	}
	catch(EmptyBoxException& e) {
		_cell = NULL;
		_impact = NULL;
		throw e;
	}

	_cell = NULL;
	_impact = NULL;
}


void Ctc::add_backtrackable(Cell& root) {

}

bool Ctc::idempotent() {
	return false;
}

} // namespace ibex
