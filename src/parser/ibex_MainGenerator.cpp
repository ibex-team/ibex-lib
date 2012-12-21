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
extern void ibexerror (const std::string& msg);

namespace ibex {

namespace parser {

void MainGenerator::generate(const P_Source& source, System& result) {

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
	Array<const ExprSymbol> srcvars(n);

	Array<const Domain> domains(n);

	i=0;
	for (vector<Entity*>::const_iterator it=source.vars.begin(); it<source.vars.end(); it++) {
		const Entity& x=**it;
		if (x.type==Entity::EPR) result.eprs.push_back(i);
		else if (x.type==Entity::SYB) result.sybs.push_back(i);
		srcvars.set_ref(i,x.symbol);
		result.args.set_ref(i,ExprSymbol::new_(x.symbol.name, x.symbol.dim));
		domains.set_ref(i,x.domain);
		i++;
		input_size+=x.symbol.dim.size();
	}
	assert(i==n);

	(int&) result.nb_var = input_size;

	//================= generate the domain =====================
	result.box.resize(input_size);
	load(result.box, domains);
	Scope scope;

	//============== generate the goal function (if any) =================
	if (source.goal==NULL) result.goal=NULL;
	else {
		// we must create a copy of the array of variables srcvars
		// for the objective function (nodes cannot be shared by
		// two different functions)
		Array<const ExprSymbol> objvars(n);

		varcopy(result.args, objvars); // TODO: should we remove eprs and sybs from the objective variables?
		const ExprNode& goal=ExprGenerator(Scope()).generate(srcvars, objvars, *source.goal);
		result.goal = new Function(objvars, goal, "goal");
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

	vector<NumConstraint*> ctrs; // tmp
	CtrGenerator().generate(srcvars, *source.ctrs, ctrs);

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
