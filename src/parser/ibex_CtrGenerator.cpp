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
#include "ibex_ConstantGenerator.h"

using namespace std;

namespace ibex {
namespace parser {

void CtrGenerator::generate(const Array<const ExprSymbol>& _src_vars, const P_ConstraintList& src,
		std::vector<NumConstraint*>& dest) {
	src_vars = &_src_vars;
	ctrs = &dest;

	assert(dest.empty());

	scopes.push(Scope()); // a fresh new scope! (needed otherwise scopes.top() is NULL -> fix issue #36)

	visit(src);

	scopes.pop();
}

void CtrGenerator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void CtrGenerator::visit(const P_OneConstraint& c) {
	int n=src_vars->size();

	Array<const ExprSymbol> dest_vars(n);
	varcopy(*src_vars,dest_vars);

	const ExprNode& e=ExprGenerator(scopes.top()).generate(*src_vars, dest_vars, c.expr);

	ctrs->push_back(new NumConstraint(dest_vars, ExprCtr(e,c.op)));
}

void CtrGenerator::visit(const P_ConstraintList& list) {
	for (vector<P_NumConstraint*>::const_iterator it=list.ctrs.begin(); it!=list.ctrs.end(); it++) {
		visit(**it);
	}
}

void CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;

	int begin=ConstantGenerator(scopes.top()).eval_integer(loop.first_value); //scope.it_first_value(name);
	int end=ConstantGenerator(scopes.top()).eval_integer(loop.last_value); //scope.it_last_value(name);

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
