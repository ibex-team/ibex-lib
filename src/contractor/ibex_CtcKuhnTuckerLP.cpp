//============================================================================
//                                  I B E X
// File        : ibex_CtcKuhnTuckerLP.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 31, 2019
//============================================================================

#include "ibex_CtcKuhnTuckerLP.h"
#include "ibex_LinearizerXTaylor.h"

using namespace std;

namespace ibex {

CtcKuhnTuckerLP::CtcKuhnTuckerLP(const NormalizedSystem& _sys, bool reject_unbounded) : Ctc(_sys.nb_var+1 /* extended box expected*/),
		reject_unbounded(reject_unbounded) {
	try {
		kkt = new KuhnTuckerSystem(_sys, true);
		_lr = new LinearizerXTaylor(*kkt);
		ph = new CtcPolytopeHull(*_lr); // note: I would prefer to use CtcLinearRelax directly.

	} catch(Exception&) {
		//TODO: replace with ExprDiffException.
		// Currently, DimException is also sometimes raised.
		cerr << "Warning: symbolic differentiation has failed ==> KKT contractor disabled" << endl;
		kkt = NULL;
	}

}

CtcKuhnTuckerLP::~CtcKuhnTuckerLP() {
	if (kkt) {
		delete kkt;
		delete _lr;
		delete ph;
	}
}

void CtcKuhnTuckerLP::contract(IntervalVector& box) {
	if (kkt==NULL || box.is_empty()) return;

	if (reject_unbounded && box.is_unbounded()) return;

	IntervalVector x=box.subvector(0,kkt->n-1); // !! sys.n <> this->nb_var if extended system !!
	IntervalVector xlambda = kkt->box; // we keep all the domains for multipliers
	xlambda.put(0,x);                  // and overwrite the domain for x with current domain

	ph->contract(xlambda);

	if (xlambda.is_empty()) {
		box.set_empty();
	}
	else {
		box.put(0,xlambda.subvector(0,kkt->n-1));
	}
}

} /* namespace ibex */
