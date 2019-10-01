//============================================================================
//                                  I B E X
// File        : ibex_CtcKuhnTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
// Created     : Jul 11, 2019
//============================================================================

#include "ibex_CtcKuhnTucker.h"
#include "ibex_FncKuhnTucker.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_ExprDiff.h"
#include "ibex_CtcNewton.h"

using namespace std;

namespace ibex {

CtcKuhnTucker::CtcKuhnTucker(const NormalizedSystem& sys, bool reject_unbounded) : Ctc(sys.nb_var+1 /* extended box expected*/), sys(sys), reject_unbounded(reject_unbounded) {
	try {
		df = new Function(*sys.goal,Function::DIFF);

		if (sys.nb_ctr>0) {
			dg = new Function*[sys.f_ctrs.image_dim()];

			for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
				dg[i] = new Function(sys.f_ctrs[i], Function::DIFF);
			}
		} else {
			dg = NULL;
		}

	} catch(Exception&) {
		//TODO: replace with ExprDiffException.
		// Currently, DimException is also sometimes raised.
		cerr << "Warning: symbolic differentiation has failed ==> KKT contractor disabled" << endl;
		df = NULL;
		dg = NULL;
	}
}

CtcKuhnTucker::~CtcKuhnTucker() {
	if (df) delete df;
	if (dg!=NULL) {
		for (int i=0; i<sys.f_ctrs.image_dim(); i++)
			delete dg[i];
		delete[] dg;
	}
}

void CtcKuhnTucker::contract(IntervalVector& box) {

	assert(df!=NULL);

	if (box.is_empty()) return;

	if (reject_unbounded && box.is_unbounded()) return;

	int n=sys.nb_var; // !! <> nb_var !!

	IntervalVector x=box.subvector(0,n-1);

	FncKuhnTucker fkkt(sys,*df,dg,x);

	if (fkkt.nb_mult==1) { // <=> no active constraint
		// for unconstrained optimization we benefit from a cheap
		// contraction with gradient=0, before running Newton.

		if (n==1)
			df->backward(Interval::zero(),x);
		else
			df->backward(IntervalVector(n,Interval::zero()),x);

		if (x.is_empty()) { box.set_empty(); return; }
		else box.put(0,x);
	}

	// not qualified constraints ==> useless to go further
	if (!fkkt.qualified()) {
		return;
	}

	if (fkkt.rejection_test(x)) {
		box.set_empty();
		return;
	}

	/* Note:
	 * Up to commit #08cb750930f49d0186bdf81f9bc6afcd8970ac98
	 * there was a preprocessing of multipliers before calling Newton
	 * (was not convincing).
	 */

	CtcNewton newton(fkkt,2); //ceiling=2 because equality multipliers often have domain=[-1,1]

	IntervalVector full_box = cart_prod(x, fkkt.multiplier_domain());

	IntervalVector save(full_box);

	newton.contract(full_box);

	if (full_box.is_empty()) {
		box.set_empty();
	}
	else {
		box.put(0,full_box.subvector(0,n-1));
	}
	// =========================================================================================
}

} /* namespace ibex */
