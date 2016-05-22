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

using namespace std;

namespace ibex {
namespace parser {

void CtrGenerator::generate(std::stack<Scope>& scopes, const P_Source& source,
		std::vector<NumConstraint*>& dest) : scopes(scopes), source(source), ctrs(dest) {

	assert(dest.empty());

	scopes.push(Scope()); // a fresh new scope! (needed otherwise scopes.top() is NULL -> fix issue #36)

	visit(*source.ctrs);
*
	scopes.pop();
}

void CtrGenerator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void CtrGenerator::visit(const P_OneConstraint& c) {

	// create the nodes for the variables
	int n=source.vars.size();
	Array<const ExprSymbol> vars(n);
	for (int i=0; i<n; i++) {
		vars.set_ref(i,ExprSymbol::new_(source.vars[i], scopes.top().get_var(source.vars[i]).second->dim));
	}

	const ExprNode& e=ExprGenerator(scopes.top()).generate(vars, c.expr);

//	Array<const ExprSymbol> dest_vars2(n);
//	varcopy(vars,dest_vars2);
//
//	const ExprNode& e2=Expr2DAG().transform(vars, (const Array<const ExprNode>&) dest_vars2, e);
//
//	cleanup(e,true);

//	ctrs->push_back(new NumConstraint(dest_vars2, ExprCtr(e2,c.op)));

	ctrs.push_back(new NumConstraint(vars, ExprCtr(e,c.op)));
}

void CtrGenerator::visit(const P_ConstraintList& list) {
	for (vector<P_NumConstraint*>::const_iterator it=list.ctrs.begin(); it!=list.ctrs.end(); it++) {
		visit(**it);
	}
}

void CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;

	int begin=ExprGenerator(scopes.top()).generate_int(loop.first_value);
	int end=ExprGenerator(scopes.top()).generate_int(loop.last_value);

	if (!scopes.empty())
		scopes.push(scopes.top());
	else
		scopes.push(Scope());

	scopes.top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes.top().set_iter_value(name,i);
		visit(loop.ctrs);
	}
	scopes.pop();
}


} // end namespace parser
} // end namespace ibex
