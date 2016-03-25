//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimData.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#include "ibex_OptimData.h"

namespace ibex {

OptimData::OptimData() : pf(), pu(0) {

}

OptimData::OptimData(const OptimData& e) : pf(e.pf), pu(e.pu) {

}

std::pair<Backtrackable*,Backtrackable*> OptimData::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new OptimData(*this),new OptimData(*this));
}

OptimData::~OptimData() {

}

void OptimData::compute_pf(Function& goal, const IntervalVector& box) {
	pf=goal.eval(box);
}


void OptimData::compute_pu(System& sys, const IntervalVector& box, const EntailedCtr& entailed) {
	double pu=1;

	for (int j=1; j<sys.nb_ctr;j++) {
		if (entailed.normalized(j)) continue;
		Interval eval=sys.f[j].eval(box);
		double pui=1;
		if (eval.diam()>0)
			pui= -eval.lb()/eval.diam();
		pu=pu*pui;
	}
}
} // end namespace ibex
