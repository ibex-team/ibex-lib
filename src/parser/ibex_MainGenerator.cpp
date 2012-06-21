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
#include "ibex_ParserSource.h"
#include "ibex_ParserResult.h"
#include <utility>

using namespace std;

namespace ibex {

namespace parser {

void MainGenerator::generate(const P_Source& source, P_Result& result) {

	//================= generate the functions =====================
	for (vector<Function*>::const_iterator it=source.func.begin(); it!=source.func.end(); it++) {
		result.func.push_back(*it);
	}

	//================= generate the variables & domains =====================
	int n=source.vars.size()+source.eprs.size()+source.sybs.size();

	result.vars = new Array<const ExprSymbol>(n);

	Array<const Domain> domains(n);

	int i=0;
	for (vector<const P_ExprSymbol*>::const_iterator it=source.vars.begin(); it<source.vars.end(); it++) {
		const P_ExprSymbol& x=**it;
		result.vars->set_ref(i,ExprSymbol::new_(x.name,x.dim));
		domains.set_ref(i,*x.domain);
		i++;
	}

	for (vector<const P_ExprSymbol*>::const_iterator it=source.eprs.begin(); it!=source.eprs.end(); it++) {
		const P_ExprSymbol& x=**it;
		result.eprs.push_back(i);
		result.vars->set_ref(i,ExprSymbol::new_(x.name,x.dim));
		domains.set_ref(i,*x.domain);
		i++;
	}
	for (vector<const P_ExprSymbol*>::const_iterator it=source.sybs.begin(); it!=source.sybs.end(); it++) {
		const P_ExprSymbol& x=**it;
		result.sybs.push_back(i);
		result.vars->set_ref(i,ExprSymbol::new_(x.name,x.dim));
		domains.set_ref(i,*x.domain);
		i++;
	}
	assert(i==n);

	//================= generate the domain =====================
	result.box.resize(n);
	load(result.box, domains);

	//================= generate the global function =====================

	vector<pair<const ExprNode*, NumConstraint::CompOp> > ctrs;

	CtrGenerator().generate(*result.vars, source.ctrs, ctrs);

	int m=ctrs.size();

	Array<const ExprNode> image(m);
	i=0;

	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		image.set_ref(i++,*(it->first));
	}

	result.f = new Function(*result.vars,ExprVector::new_(image,false));

	//================= generate the constraints =====================

	i=0;
	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		result.ctrs.push_back(new NumConstraint((*result.f)[i++], it->second));
	}
}

} // end namespace parser
} // end namespace ibex
