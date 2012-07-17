//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcNewton.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//====================================f========================================

#include "ibex_CtcNewton.h"
#include "ibex_Exception.h"

namespace ibex {

const double CtcNewton::default_ceil = 0.01;

CtcNewton::CtcNewton(const Function& f, double ceil, double prec, double ratio) :
		Ctc(f.input_size()), f(f), ceil(ceil), prec(prec), gauss_seidel_ratio(ratio) {

	if (f.input_size()!=f.output_size()) {
		not_implemented("Newton operator with rectangular systems.");
	}
}

void CtcNewton::contract(IntervalVector& box) {
	if (!(box.max_diam()<=ceil)) return;
	else newton(f,box,prec,gauss_seidel_ratio);

}

} // end namespace ibex
