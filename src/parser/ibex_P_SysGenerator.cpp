//============================================================================
//                                  I B E X                                   
// File        : ibex_P_SysGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_P_SysGenerator.h"
#include "ibex_P_CtrGenerator.h"
#include "ibex_P_ExprGenerator.h"
#include "ibex_P_Scope.h"
#include "ibex_SyntaxError.h"
#include "ibex_ExprCopy.h"
#include "ibex_SystemFactory.h"

#include <utility>

using namespace std;

namespace ibex {

namespace parser {

void P_SysGenerator::add_garbage(NodeMap<bool>& garbage, const ExprNode& e) {
	ExprSubNodes nodes(e);
	for (int i=0; i<nodes.size(); i++)
		if (!garbage.found(nodes[i])) {
			garbage.insert(nodes[i],true);
		}
}

P_SysGenerator::P_SysGenerator(P_Scope& scopes) : scopes(scopes) {

}

void P_SysGenerator::generate(P_Source& source, System& sys, int simpl_level) {

	SystemFactory fac;

	fac.set_simplification_level(simpl_level);

	Array<const ExprSymbol> vars = scopes.var_symbols();

	//================= generate the variables & domains =====================
	if (vars.is_empty())
		throw SyntaxError("System with no variable");

	fac.add_var(vars);

	NodeMap<bool> garbage;

	//============== generate the goal function (if any) =================
	if (source.goal!=NULL) {
		const ExprNode& goal=source.goal->generate();
		fac.add_goal(goal);
		add_garbage(garbage,goal);
	}

	//================= generate the constraints =====================

	if (source.ctrs!=NULL) { // not in case of unconstrained optimization

		vector<ExprCtr*> ctrs = P_CtrGenerator(scopes).generate(*source.ctrs);

		for (vector<ExprCtr*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
			fac.add_ctr(**it); // by copy so...
			add_garbage(garbage,(*it)->e); // ... clean it up
			delete *it;
		}
	}

	sys.init(fac);

	//================= add mutable constants =====================
	sys.mutable_constants.import(source.mutable_constants);


	// add the original variables in the garbage
	// (in case some are not used)
	for (int i=0;i<vars.size(); i++)
		add_garbage(garbage, vars[i]);

	//================= set the domains =====================
	sys.box.resize(sys.nb_var);
	load(sys.box, scopes.var_domains());

	//==================== *** cleanup *** ====================
	for (IBEX_NODE_MAP(bool)::const_iterator it=garbage.begin(); it!=garbage.end(); it++) {
		delete it->first;
	}
}

} // end namespace parser
} // end namespace ibex
