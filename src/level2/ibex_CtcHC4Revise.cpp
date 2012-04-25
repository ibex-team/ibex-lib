/* ============================================================================
 * I B E X - HC4Revise contractor
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcHC4Revise.h"

namespace ibex {

CtcHC4Revise::CtcHC4Revise(const NumConstraint& ctr) : Ctc(ctr.f.nb_symbols()), ctr(ctr), hc4r(ctr) {

}

void CtcHC4Revise::contract(IntervalVector& box) {
	hc4r.contract(box);

	//hc4r.eval.f.cf.print<Domain>();
}

bool CtcHC4Revise::can_contract(int v) const {
	return ctr.f.used(v);
}

} // namespace ibex
