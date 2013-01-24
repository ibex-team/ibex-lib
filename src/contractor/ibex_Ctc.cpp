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

Ctc::Ctc(int n) : nb_var(n), input(n), output(n) {
	input.set_all();   // by default
	output.set_all();  // by default
}

Ctc::~Ctc() {
}

//void Ctc::contract(Cell& cell) {
//	contract(cell.box);
//}

void Ctc::contract(IntervalVector& box, const BoolMask& impact) {
	contract(box);
}

} // namespace ibex
