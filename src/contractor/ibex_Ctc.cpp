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

namespace ibex {

Ctc::Ctc(int n) : nb_var(n), input(NULL), output(NULL), _impact(NULL) {

}

Ctc::~Ctc() {
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


void Ctc::contract(IntervalVector& box, const BoolMask& impact, BoolMask& flags) {
	_impact = &impact;
	_output_flags = &flags;

	flags.unset_all();

	try {
		contract(box);
	}
	catch(EmptyBoxException& e) {
		_impact = NULL;
		_output_flags = NULL;
		throw e;
	}

	_impact = NULL;
	_output_flags = NULL;
}

} // namespace ibex
