/* ============================================================================
 * I B E X - ibex_FncActivation.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_FncActivation.h"

using namespace std;

namespace ibex {

namespace {

BitSet get_activated_ctrs(const System& sys, const Vector& pt, double activation_threshold, bool trace) {

	BitSet activated=BitSet::empty(sys.f_ctrs.image_dim());

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
	return activated;
}

} // end namespace

FncActivation::FncActivation(const System& sys, const Vector& pt, double activation_threshold, bool trace) : FncProj(sys.f_ctrs, get_activated_ctrs(sys, pt, activation_threshold, trace)) {

}

} /* namespace ibex */
