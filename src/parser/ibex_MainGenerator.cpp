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

	result.vars.resize(n);
	Array<const ExprSymbol> srcvars(n);

	Array<const Domain> domains(n);

	i=0;
	for (vector<Entity*>::const_iterator it=source.vars.begin(); it<source.vars.end(); it++) {
		const Entity& x=**it;
		// cout << "new symbol=" << x.symbol.name << endl;
		if (x.type==Entity::EPR) result.eprs.push_back(i);
		else if (x.type==Entity::SYB) result.sybs.push_back(i);
		srcvars.set_ref(i,x.symbol);
		result.vars.set_ref(i,ExprSymbol::new_(x.symbol.name, x.symbol.dim));
		domains.set_ref(i,x.domain);
		i++;
		input_size+=x.symbol.dim.size();
	}
	assert(i==n);

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
		for (int i=0; i<n; i++)
			// TODO: should we remove eprs and sybs from the objective variables?
			objvars.set_ref(i,ExprSymbol::new_(result.vars[i].name, result.vars[i].dim));

		const ExprNode& goal=ExprGenerator(Scope()).generate(srcvars, objvars, *source.goal);
		result.goal = new Function(objvars,goal,"goal");
	}

	//================= generate the global function =====================

	vector<pair<const ExprNode*, NumConstraint::CompOp> > ctrs;

	CtrGenerator().generate(srcvars, result.vars, *source.ctrs, ctrs);

	int m=ctrs.size();

	Array<const ExprNode> image(m);
	i=0;

	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		image.set_ref(i++,*(it->first));
	}

	result.f.init(result.vars,
			m>1? ExprVector::new_(image,false) : image[0]);

	//================= generate the constraints =====================
	result.ctrs.resize(m);
	i=0;
	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		result.ctrs.set_ref(i,*new NumConstraint(result.f[i], it->second));
		i++;
	}
}

} // end namespace parser
} // end namespace ibex
