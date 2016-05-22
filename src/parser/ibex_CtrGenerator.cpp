//============================================================================
//                                  I B E X                                   
// File        : ibex_CtrGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_CtrGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_P_NumConstraint.h"
#include "ibex_Function.h"
#include "ibex_P_ExprGenerator.h"
#include "ibex_Expr2DAG.h"
#include "ibex_P_Source.h"
#include "ibex_Scope.h"

using namespace std;

extern stack<ibex::parser::Scope>& scopes();

namespace ibex {
namespace parser {

std::vector<ExprCtr*> CtrGenerator::generate(const P_ConstraintList& ctrs) {

	visit(ctrs);

	return this->ctrs;
}

void CtrGenerator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void CtrGenerator::visit(const P_OneConstraint& c) {

//	Array<const ExprSymbol> dest_vars2(n);
//	varcopy(vars,dest_vars2);
//
//	const ExprNode& e2=Expr2DAG().transform(vars, (const Array<const ExprNode>&) dest_vars2, e);
//
//	cleanup(e,true);

//	ctrs->push_back(new NumConstraint(dest_vars2, ExprCtr(e2,c.op)));

	ctrs.push_back(new ExprCtr(c.expr.generate(),c.op));
}

void CtrGenerator::visit(const P_ConstraintList& list) {
	for (vector<P_NumConstraint*>::const_iterator it=list.ctrs.begin(); it!=list.ctrs.end(); it++) {
		visit(**it);
	}
}

void CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;

	int begin=_2int(loop.first_value);
	int end=_2int(loop.last_value);

	scopes().push(scopes().top());

	scopes().top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes().top().set_iter_value(name,i);
		visit(loop.ctrs);
	}
	scopes().pop();
}


} // end namespace parser
} // end namespace ibex
