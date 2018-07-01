//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimData.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#include "ibex_OptimData.h"
#include "ibex_Id.h"

namespace ibex {

const long BxpOptimData::prop_id = next_id();

BxpOptimData::BxpOptimData() : pf(), pu(0) {

}

BxpOptimData::BxpOptimData(const BxpOptimData& e) : pf(e.pf), pu(e.pu) {

}

BxpOptimData::~BxpOptimData() {

}

void BxpOptimData::compute_pf(const Function& goal, const IntervalVector& box) {
	pf=goal.eval(box);
}


void BxpOptimData::compute_pu(const System& sys, const IntervalVector& box) {
	double pu=1;

	if (sys.active_ctrs(box).empty()) return;

	IntervalVector e=sys.active_ctrs_eval(box);

	for (int j=1; j<e.size();j++) {
		double pui=1;
		if (e[j].diam()>0)
			pui= -e[j].lb()/e[j].diam();
		pu=pu*pui;
	}
}
} // end namespace ibex
