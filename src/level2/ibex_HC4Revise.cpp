/* ============================================================================
 * I B E X - ibex_HC4Revise.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_HC4Revise.h"

namespace ibex {

HC4Revise::HC4Revise(const NumConstraint& ctr) : Contractor(ctr.f.nb_symbols()), ctr(ctr), hc4r(ctr) {

}

void HC4Revise::contract(IntervalVector& box) {
	d=box;
	hc4r.contract(d);
	box=d;
}

bool HC4Revise::can_contract(int v) {
	return ctr.f.used(v);
}

} // namespace ibex
