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
	result.box.resize(n);

	int i=0;
	for (int j=0; j<source.vars.size(); j++) {
		result.vars->set_ref(i,*source.vars[j]);
		result.box[i]=source.vars[j]->domain;
		i++;
	}

	for (int j=0; j<source.eprs.size(); j++) {
		result.eprs.push_back(i);
		result.vars->set_ref(i,*source.eprs[j]);
		result.box[i]=source.eprs[j]->domain;
		i++;
	}
	for (int j=0; j<source.sybs.size(); j++) {
		result.sybs.push_back(i);
		result.vars->set_ref(i,*source.sybs[j]);
		result.box[i]=source.sybs[j]->domain;
		i++;
	}
	assert(i==n);

	//================= generate the constraints =====================
	vector<pair<const ExprNode*, NumConstraint::CompOp> > ctrs;

	CtrGenerator().generate(*result.vars, source.ctrs, ctrs);

	int m=ctrs.size();

	Array<const ExprNode> image(m);
	i=0;

	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		image.set_ref(i++,*(it->first));
	}

	//================= generate the global function =====================

	Function* f=new Function(*result.vars,ExprVector::new_(image,false));

	vector<NumConstraint*> res;

	i=0;
	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		res.push_back(new NumConstraint(f[i++], it->second));
	}
}

} // end namespace parser
} // end namespace ibex
