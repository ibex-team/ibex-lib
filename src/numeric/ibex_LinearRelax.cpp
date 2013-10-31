//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxation.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 30, 2013
// Last Update : Oct 30, 2013
//============================================================================

#include "ibex_LinearRelax.h"

namespace ibex {

LinearRelax::LinearRelax(const System& sys) : sys(sys) { }

LinearRelax::~LinearRelax() { }

bool LinearRelax::isInner(IntervalVector & box, const System& sys, int j) {
	Interval eval=sys.ctrs[j].f.eval(box);

	if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
			(sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
		return false;
	else
		return true;
}
} // end namespace ibex
