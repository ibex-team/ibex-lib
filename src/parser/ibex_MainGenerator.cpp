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
#include "ibex_ExprCopy.h"
#include "ibex_CtrGenerator.h"
#include "ibex_P_ExprGenerator.h"
#include "ibex_Scope.h"
#include "ibex_SystemFactory.h"

#include <utility>
#include <stack>

using namespace std;

namespace ibex {

namespace parser {

extern stack<Scope>& scopes();

void MainGenerator::add_garbage(NodeMap<bool>& garbage, const ExprNode& e) {
	ExprSubNodes nodes(e);
	for (int i=0; i<nodes.size(); i++)
		if (!garbage.found(nodes[i]))
			garbage.insert(nodes[i],true);
}

void MainGenerator::generate(const P_Source& source, System& sys) {

	SystemFactory fac;
	Array<const ExprSymbol> vars = scopes().top().var_symbols();

	//================= generate the variables & domains =====================
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

		vector<ExprCtr*> ctrs = CtrGenerator().generate(*source.ctrs);

		for (vector<ExprCtr*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
			fac.add_ctr(**it); // by copy so...
			add_garbage(garbage,(*it)->e);
			delete *it;
		}
	}

	sys.init(fac);

	// add the original variables in the garbage
	// (in case some are not used)
	for (int i=0;i<vars.size(); i++)
		add_garbage(garbage, vars[i]);

	// same for constants
	for (std::vector<const char*>::const_iterator it=scopes().top().cst.begin(); it!=scopes().top().cst.end(); it++) {
		//cout << "[parser] remove cst " << *it << endl;
		add_garbage(garbage, scopes().top().get_cst(*it));
	}

	//================= set the domains =====================
	sys.box.resize(sys.nb_var);
	load(sys.box, scopes().top().var_domains());

	//================= add the external functions ===========
	sys.func.resize(source.func.size());
	int i=0;
	for (vector<Function*>::const_iterator it=source.func.begin(); it!=source.func.end(); it++) {
		sys.func.set_ref(i++,**it);
	}

	//==================== *** cleanup *** ====================
	for (IBEX_NODE_MAP(bool)::const_iterator it=garbage.begin(); it!=garbage.end(); it++) {
		delete it->first;
	}
}

} // end namespace parser
} // end namespace ibex
