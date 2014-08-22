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
#include "ibex_ExtendedSystem.h"

namespace ibex {

LinearRelax::LinearRelax(const System& sys) : _nb_ctr(sys.nb_ctr), _nb_var(sys.nb_var), _goal_var(-1)/* by default */ {
	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		_goal_var=((const ExtendedSystem&) sys).goal_var();
	}

}

LinearRelax::LinearRelax(int nb_ctr, int nb_var, int goal_var) : _nb_ctr(nb_ctr), _nb_var(nb_var), _goal_var(goal_var) {

}

LinearRelax::~LinearRelax() { }

bool LinearRelax::isInner(const IntervalVector & box, const System& sys, int j) {
	Interval eval=sys.ctrs[j].f.eval(box);

	if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
			(sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
		return false;
	else
		return true;
}
} // end namespace ibex
