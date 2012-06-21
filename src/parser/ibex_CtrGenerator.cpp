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
#include "ibex_ParserNumConstraint.h"
#include "ibex_Function.h"
#include "ibex_ExprGenerator.h"

using namespace std;

namespace ibex {
namespace parser {

void CtrGenerator::generate(const Array<const ExprSymbol> _vars,
		std::vector<P_NumConstraint*> src,
		std::vector<std::pair<const ExprNode*, NumConstraint::CompOp> > dest) {
	vars = &_vars;
	ctrs = &dest;

	assert(dest.empty());

	for (vector<P_NumConstraint*>::const_iterator it=src.begin(); it!=src.end(); it++) {
		visit(**it);
	}
}

void CtrGenerator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void CtrGenerator::visit(const P_OneConstraint& c) {
	const ExprNode& e=ExprGenerator(scopes.top()).generate(*vars,c.expr);
	ctrs->push_back(pair<const ExprNode*, NumConstraint::CompOp>(&e,c.op));
}

void CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;
	int         begin    = loop.first_value; //scope.it_first_value(name);
	int         end      = loop.last_value; //scope.it_last_value(name);

	scopes.push(scopes.top());
	scopes.top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes.top().set_iter_value(name,i);
		for (vector<P_NumConstraint*>::const_iterator it=loop.ctrs.begin(); it!=loop.ctrs.end(); it++) {
			visit(**it);
		}
	}
	scopes.pop();
}


} // end namespace parser
} // end namespace ibex
