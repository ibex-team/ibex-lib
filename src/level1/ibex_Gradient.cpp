/* ============================================================================
 * I B E X - ibex_Gradient.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Gradient.h"

namespace ibex {

Gradient::Gradient(const Function& f) : eval(f) {

	f.decorate(GradDecorator());
}

void Gradient::calculate(const IntervalVector& box, IntervalVector& g) const {
	eval.eval(box);
	eval.f.backward<Gradient,GradLabel>(*this);
	//for (int i=0; i<)
	// TO COMPLETE
}

} // namespace ibex
