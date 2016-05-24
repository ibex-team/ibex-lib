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

extern stack<ibex::parser::Scope>& scopes();

namespace ibex {

namespace parser {

void MainGenerator::generate(const P_Source& source, System& sys) {

	SystemFactory fac;

	//================= generate the variables & domains =====================
	int n=scopes().top().vars.size();
	int input_size=0;

	fac.add_var(scopes().top().var_symbols());

	//============== generate the goal function (if any) =================
	if (source.goal!=NULL) {
		fac.add_goal(source.goal->generate());
	}

	// ============== case of unconstrained optimization ===========
	if (source.ctrs!=NULL) {

		//================= generate the constraints =====================
		// we cannot generate first the global function f and
		// then each constraint with (f[i] op 0) because
		// a constraint can be vector or matrix valued.
		// so we do the contrary: we generate first the constraints,
		// and build f with the components of all constraints' functions.
		// ----> SystemFactory

		vector<ExprCtr*> ctrs = CtrGenerator().generate(*source.ctrs);

		for (vector<ExprCtr*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
			fac.add_ctr(**it);
		}
	}

	sys.init(fac);

	//================= set the domains =====================
	sys.box.resize(input_size);
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
