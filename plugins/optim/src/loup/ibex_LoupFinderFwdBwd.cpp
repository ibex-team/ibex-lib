//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderFwdBwd.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderFwdBwd.h"
#include "ibex_LoupFinderProbing.h"

namespace ibex {

LoupFinderFwdBwd::LoupFinderFwdBwd(const System& sys) : sys(sys), m(sys.nb_ctr) {

	// ====== build the reversed inequalities g_i(x)>0 ===============
	is_inside=m>0? new CtcUnion(sys) : NULL;

	mono_analysis_flag=true;
}

LoupFinderFwdBwd::~LoupFinderFwdBwd() {
	if (is_inside) {
		for (int i=0; i<m; i++) {
			delete &(is_inside->list[i]);
		}

		delete is_inside;
	}
}

std::pair<IntervalVector, double> LoupFinderFwdBwd::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {

	IntervalVector inbox=box;

	bool inner_found=false;

	if (m==0)  // unconstrained problem
		inner_found=true;
	else {

		is_inside->contract(inbox); // compared to in_HC4, works the other way around: if inbox is inner, it is emptied.

		if (inbox.is_empty()) {
			inbox = box;
			inner_found=true;
		} else {
			inner_found=false;
			inbox.set_empty();
		}
	}

	if (mono_analysis_flag)
		monotonicity_analysis(sys, inbox, inner_found);

	return LoupFinderProbing(sys).find(inner_found? inbox : box,loup_point,loup);
}

} /* namespace ibex */
