//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemFactory.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#include "ibex_SystemFactory.h"
#include "ibex_Exception.h"
#include "ibex_ExprCtr.h"
#include "ibex_ExprCopy.h"

using std::vector;

namespace ibex {

SystemFactory::SystemFactory() : goal(NULL) { }

void SystemFactory::add_var(Variable& v) {
	if (goal || !exprs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");
	vars.push_back(&((const ExprSymbol&) v));

}
void SystemFactory::add_goal(const ExprNode& goal) {
	this->goal = &goal;
}

void SystemFactory::add_ctr(const ExprCtr& ctr) {
	exprs.push_back(&ctr.e);
	ops.push_back(ctr.op);
}


System::System(const SystemFactory& fac) : nb_var(0) /* tmp */,
		nb_ctr(fac.exprs.size()), func(0), vars(fac.vars.size()), box(1) /* tmp */, ctrs(nb_ctr)  {

	int nb=fac.vars.size();

	// =========== init vars
	for (int j=0; j<nb; j++) {
		vars.set_ref(j,ExprSymbol::new_(fac.vars[j]->name, fac.vars[j]->dim));
		(int&) nb_var += fac.vars[j]->dim.size();
	}

	// =========== init goal
	if (fac.goal) {
		Array<const ExprSymbol> goal_vars(nb);
		for (int j=0; j<nb; j++)
			goal_vars.set_ref(j,ExprSymbol::new_(fac.vars[j]->name, fac.vars[j]->dim));

		goal = new Function(goal_vars, ExprCopy().copy(fac.vars,goal_vars,*fac.goal));
	}

	// =========== init f
	Array<const ExprNode> y(nb_ctr);
	for (int i=0; i<nb_ctr; i++) {
		y.set_ref(i,ExprCopy().copy(fac.vars,vars,*fac.exprs[i]));
	}
	f.init(vars, nb_ctr>1 ? ExprVector::new_(y,false) : y[0]);

	// =========== resize the box
	box.resize(nb_var);

	// =========== init ctrs
	for (int i=0; i<nb_ctr; i++)
		ctrs.set_ref(i,*new NumConstraint(f[i], LEQ));
}

} // end namespace

