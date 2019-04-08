/* ============================================================================
 * I B E X - ibex_FncProj.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec, 26 2018
 * ---------------------------------------------------------------------------- */

#include "ibex_FncProj.h"

using namespace std;

namespace ibex {

FncProj::FncProj(const Fnc& fnc, const BitSet& components) : Fnc(fnc.nb_var(), components.size()), fnc(fnc), components(components) {

}

Interval FncProj::eval(const IntervalVector& x) const {
	assert(components.size()==1);
	return fnc.eval(components.begin(), x);
}

IntervalVector FncProj::eval_vector(const IntervalVector& x, const BitSet& components2) const {
	return fnc.eval_vector(x, components.compose(components2));
}

void FncProj::jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components2, int v) const {
	fnc.jacobian(x, J, components.compose(components2),v);
}

} /* namespace ibex */
