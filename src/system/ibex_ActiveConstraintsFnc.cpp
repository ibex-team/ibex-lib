/* ============================================================================
 * I B E X - ibex_ActiveConstraintsFnc.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_ActiveConstraintsFnc.h"

using namespace std;

namespace ibex {

ActiveConstraintsFnc::ActiveConstraintsFnc(const System& sys, const Vector& pt, double activation_threshold, bool trace) : sys(sys), activated(sys.f_ctrs.image_dim()) {

	BitSet ith=BitSet::empty(sys.f_ctrs.image_dim());

	for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
		if (sys.ops[i]==EQ) activated.add(i);
		else {
			ith.add(i);

			// call to eval_vector to avoid component generation
			if (sys.f_ctrs.eval_vector(pt,ith)[0].mag() < activation_threshold) {
				if (trace) cout << " activate inequality n°" << i << endl;
				activated.add(i);
			}
			ith.remove(i);
		}
	}

	(int &) _nb_var = sys.nb_var;
	(int &) _image_dim = activated.size();
}


ActiveConstraintsFnc::~ActiveConstraintsFnc() {

}

IntervalVector ActiveConstraintsFnc::eval_vector(const IntervalVector& x) const {
	return sys.f_ctrs.eval_vector(x,activated);
}

void ActiveConstraintsFnc::jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v) const {
	sys.f_ctrs.jacobian(x,J,activated.compose(components),v);
}

} /* namespace ibex */
