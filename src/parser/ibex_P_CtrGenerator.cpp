//============================================================================
//                                  I B E X                                   
// File        : ibex_P_CtrGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : May 22, 2019
//============================================================================

#include "ibex_P_CtrGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_P_NumConstraint.h"
#include "ibex_Function.h"
#include "ibex_P_ExprGenerator.h"
#include "ibex_Expr2DAG.h"
#include "ibex_P_Source.h"
#include "ibex_P_Scope.h"

#include <sstream>

using namespace std;

namespace ibex {

namespace parser {

P_CtrGenerator::P_CtrGenerator(P_Scope& scopes) : scopes(scopes) {
	// probably not anymore useful since commit:

}

std::vector<ExprCtr*> P_CtrGenerator::generate(const P_ConstraintList& ctrs) {

	visit(ctrs);

	return this->ctrs;
}

void P_CtrGenerator::visit(const P_NumConstraint& c) {
	c.accept_visitor(*this);
}

void P_CtrGenerator::visit(const P_OneConstraint& c) {

//	Array<const ExprSymbol> dest_vars2(n);
//	varcopy(vars,dest_vars2);
//
//	const ExprNode& e2=Expr2DAG().transform(vars, (const Array<const ExprNode>&) dest_vars2, e);
//
//	cleanup(e,true);

//	ctrs->push_back(new NumConstraint(dest_vars2, ExprCtr(e2,c.op)));

	try {
		// do not simplify the expression here because some ExprNode
		// may be shared between different generated constraints
		// (use of temporary expressions or constants)
		// The simplification will be done by the factory.
		ExprCtr* e=new ExprCtr(ExprGenerator(scopes).generate(c.expr),c.op);
		//cout << "[parser] generated ctr: " << *e << endl;
		ctrs.push_back(e);
	} catch(DimException& e) {
		throw SyntaxError(e.message(),NULL,c.expr.line);
	}
}

void P_CtrGenerator::visit(const P_TmpSymbolDecl& tmp) {
	// TODO: if the temporary symbol is never used,
	// the generated expression is a memory leak
	scopes.add_expr_tmp_symbol(tmp.symbol, &ExprGenerator(scopes).generate(tmp.expr));
}

void P_CtrGenerator::visit(const P_ConstraintList& list) {
	for (vector<P_NumConstraint*>::const_iterator it=list.ctrs.begin(); it!=list.ctrs.end(); it++) {
		visit(**it);
	}
}

void P_CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;

	int begin=loop.first_value._2int();
	int end=loop.last_value._2int();

	for (int i=begin; i<=end; i++) {
		scopes.push();
		scopes.add_iterator(name);
		scopes.set_iter_value(name,i);
		visit(loop.ctrs);
		scopes.pop();
	}
}

void P_CtrGenerator::visit(const P_ThickEquality& eq) {
	try {
		const ExprNode& e=ExprGenerator(scopes).generate(eq.expr);
		//cout << "[parser] generated ctr: " << *e << endl;
		ctrs.push_back(new ExprCtr(e-eq.d.lb(),GEQ));
		ctrs.push_back(new ExprCtr(e-eq.d.ub(),LEQ));
	} catch(DimException& e) {
		throw SyntaxError(e.message(),NULL,eq.expr.line);
	}
}

} // end namespace parser
} // end namespace ibex
