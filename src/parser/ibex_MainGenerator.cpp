//============================================================================
//                                  I B E X                                   
// File        : ibex_MainGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_MainGenerator.h"
#include "ibex_CtrGenerator.h"
#include "ibex_P_ExprGenerator.h"

#include <utility>

using namespace std;
//extern void ibexerror (const std::string& msg);

namespace ibex {

namespace parser {

void MainGenerator::generate(Scope& scope, const P_Source& source, System& result) {

	//================= generate the functions =====================
	result.func.resize(source.func.size());
	int i=0;
	for (vector<Function*>::const_iterator it=source.func.begin(); it!=source.func.end(); it++) {
		result.func.set_ref(i++,**it);
	}

	//================= generate the variables & domains =====================
	int n=source.vars.size();
	int input_size=0;

	result.args.resize(n);
	Array<const Domain> domains(n);

	i=0;
	for (vector<const char*>::const_iterator it=source.vars.begin(); it<source.vars.end(); it++) {
		std::pair<const ExprSymbol*,const Domain*> p=scope.get_var(*it);

		result.args.set_ref(i,ExprSymbol::new_(*it, p.second->dim));
		domains.set_ref(i,*p.second); // domain by copy
		i++;
		input_size+=p.second->dim.size();
	}
	assert(i==n);

	(int&) result.nb_var = input_size;

	//================= generate the domain =====================
	result.box.resize(input_size);
	load(result.box, domains);

	//============== generate the goal function (if any) =================
	if (source.goal==NULL) result.goal=NULL;
	else {
		// we must create a copy of the array of variables
		// for the objective function (nodes cannot be shared by
		// two different functions)
		Array<const ExprSymbol> goal_vars(n);

		varcopy(result.args, goal_vars);

		const ExprNode& goal_expr=ExprGenerator(scope).generate(goal_vars, *source.goal);
		result.goal = new Function(goal_vars, goal_expr, "goal");
	}

	// ============== case of unconstrained optimization ===========
	if (source.ctrs==NULL) {
		(int&) result.nb_ctr = 0;
		return;
	}

	//================= generate the constraints =====================
	// we cannot generate first the global function f and
	// then each constraint with (f[i] op 0) because
	// a constraint can be vector or matrix valued.
	// so we do the contrary: we generate first the constraints,
	// and build f with the components of all constraints' functions.

	stack<Scope> scopes;
	scopes.push(scope);

	vector<NumConstraint*> ctrs; // tmp
	CtrGenerator().generate(scopes, source, ctrs);

	int m=ctrs.size();
	result.ctrs.resize(m);
	i=0;
	(int&) result.nb_ctr = 0;
	for (vector<NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		result.ctrs.set_ref(i,**it);
		i++;
		((int&) result.nb_ctr) += (*it)->f.image_dim();
	}


	//================= generate the global function =====================

	result.init_f_from_ctrs();
}

} // end namespace parser
} // end namespace ibex
