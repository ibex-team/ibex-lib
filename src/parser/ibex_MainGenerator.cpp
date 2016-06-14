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

void MainGenerator::generate(const P_Source& source, System& sys) {

	SystemFactory fac;
	Array<const ExprSymbol> vars = scopes().top().var_symbols();

	//================= generate the variables & domains =====================
	fac.add_var(vars);

	//============== generate the goal function (if any) =================
	if (source.goal!=NULL) {
		fac.add_goal(source.goal->generate());
	}

	// ============== case of unconstrained optimization ===========
	if (source.ctrs!=NULL) {

		//================= generate the constraints =====================
		vector<ExprCtr*> ctrs = CtrGenerator().generate(*source.ctrs);

		for (vector<ExprCtr*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
			fac.add_ctr(**it); // by copy so...
			delete *it;        // delete the original
		}
	}

	sys.init(fac);

	// delete the original variables
	for (int i=0;i<vars.size(); i++)
		delete &vars[i];

	//================= set the domains =====================
	sys.box.resize(sys.nb_var);
	load(sys.box, scopes().top().var_domains());

	//================= add the external functions ===========
	sys.func.resize(source.func.size());
	int i=0;
	for (vector<Function*>::const_iterator it=source.func.begin(); it!=source.func.end(); it++) {
		sys.func.set_ref(i++,**it);
	}
}

} // end namespace parser
} // end namespace ibex
