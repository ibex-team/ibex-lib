//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcEpsPrecision.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 12, 2014
// Last Update : May 12, 2014
//============================================================================

#include "ibex_CtcEpsPrecision.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

CtcEpsPrecision::CtcEpsPrecision(const Vector &eps) : Ctc(eps.size()), eps(eps) {

}

CtcEpsPrecision::~CtcEpsPrecision() {

}

void CtcEpsPrecision::contract(IntervalVector& box) {
	assert(box.size()==nb_var);
	for (int i=0;i<box.size();i++) {
		if (box[i].diam()<=eps[i]) {
			box.set_empty();
			throw EmptyBoxException();
		}
	}
}

bool CtcEpsPrecision::idempotent() {
	return true;
}

} // end namespace ibex
