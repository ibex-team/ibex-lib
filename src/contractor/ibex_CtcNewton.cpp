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
		Ctc(f.nb_var()), f(f), vars(NULL), ceil(ceil), prec(prec), gauss_seidel_ratio(ratio) {

	if (f.nb_var()!=f.image_dim()) {
		not_implemented("Newton operator with rectangular systems.");
	}
}

CtcNewton::CtcNewton(const Function& f, const VarSet& vars, double ceil, double prec, double ratio) :
		Ctc(f.nb_var()), f(f), vars(&vars), ceil(ceil), prec(prec), gauss_seidel_ratio(ratio) {

	if (vars.nb_var!=f.image_dim()) {
		not_implemented("Newton operator with rectangular systems.");
	}
}

void CtcNewton::contract(IntervalVector& box) {
	if (!(box.max_diam()<=ceil)) return;
	else {
		if (!vars)
			newton(f,box,prec,gauss_seidel_ratio);
		else
			newton(f,*vars,box,prec,gauss_seidel_ratio);
	}

	if (box.is_empty()) {
		set_flag(FIXPOINT);
	}
}

} // end namespace ibex
