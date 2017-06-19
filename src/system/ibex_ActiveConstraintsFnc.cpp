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

ActiveConstraintsFnc::ActiveConstraintsFnc(const System& sys, const Vector& pt, double activation_threshold, bool trace) : sys(sys) {

	activated.initialise(0,sys.ctrs.size()-1,BitSet::empt);

	for (int i=0; i<sys.ctrs.size(); i++) {
		if (sys.ctrs[i].op==EQ ||
				sys.ctrs[i].f.eval(pt).mag() < activation_threshold) {
			if (trace && sys.ctrs[i].op!=EQ) cout << " activate inequality n°" << i << endl;
			activated.add(i);
		}
	}

	(int &) _nb_var = sys.nb_var;
	(int &) _image_dim = activated.size();
}


ActiveConstraintsFnc::~ActiveConstraintsFnc() {

}

IntervalVector ActiveConstraintsFnc::eval_vector(const IntervalVector& x) const {
	// TODO: use the DAG structure (needs to create a new "eval_vector" function in Function with Bitset argument?)
	IntervalVector y(activated.size());
	int c;
	for (unsigned int i=0; i<activated.size(); i++) {
		c = (i==0)? activated.min() : activated.next(c);
		y[i]=sys.ctrs[c].f.eval(x);
	}
	return y;
}

void ActiveConstraintsFnc::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	// TODO: use the DAG structure (needs to create a new "jacobian" function in Function with Bitset argument?)
	//	IntervalMatrix J(activated.size(), x.size());
	int c;
	for (unsigned int i=0; i<activated.size(); i++) {
		c = (i==0)? activated.min() : activated.next(c);
		J[i]=sys.ctrs[c].f.gradient(x);
	}
}

} /* namespace ibex */
