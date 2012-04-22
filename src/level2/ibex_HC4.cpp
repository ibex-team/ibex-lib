//============================================================================
//                                  I B E X                                   
// File        : ibex_HC4.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 22, 2012
// Last Update : Apr 22, 2012
//============================================================================

#include "ibex_HC4.h"
#include "ibex_HC4Revise.h"

namespace ibex {

namespace {
Array<Contractor> convert(const Array<NumConstraint>& csp) {
	std::vector<const Contractor*> vec(csp.size());
	for (int i=0; i<csp.size(); i++) {
		vec.push_back(new HC4Revise(csp[i]));
	}
	return vec;
}
}

HC4::HC4(const Array<NumConstraint>& csp, double ratio, bool incremental) :
		Propagation(convert(csp), default_ratio, incremental) {
}

HC4::~HC4() {
	for (int i=0; i<list.size(); i++)
		delete &list[i];
}

} // end namespace ibex
