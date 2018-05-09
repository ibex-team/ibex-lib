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

#include <sstream>

using namespace std;

namespace ibex {

namespace parser {

extern stack<Scope>& scopes();

CtrGenerator::CtrGenerator() {
	// probably not anymore useful since commit:

	for (vector<const char*>::iterator it=scopes().top().cst.begin(); it!=scopes().top().cst.end(); it++) {
		s.lock.insert(scopes().top().get_cst(*it),true);
	}
}

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

	try {
		// do not simplify the expression here because some ExprNode
		// may be shared between different generated constraints
		// (use of temporary expressions + symbolic constants?)
		ExprCtr* e=new ExprCtr(c.expr.generate(),c.op);
		//cout << "[parser] generated ctr: " << *e << endl;
		ctrs.push_back(e);
	} catch(DimException& e) {
		throw SyntaxError(e.message(),NULL,c.expr.line);
	}
}

void CtrGenerator::visit(const P_ConstraintList& list) {
	for (vector<P_NumConstraint*>::const_iterator it=list.ctrs.begin(); it!=list.ctrs.end(); it++) {
		visit(**it);
	}
}

void CtrGenerator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;

	int begin=loop.first_value._2int();
	int end=loop.last_value._2int();

	scopes().push(scopes().top());

	scopes().top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes().top().set_iter_value(name,i);
		visit(loop.ctrs);
	}
	scopes().pop();
}

void CtrGenerator::visit(const P_ThickEquality& eq) {
	try {
		const ExprNode& e=eq.expr.generate();
		//cout << "[parser] generated ctr: " << *e << endl;
		ctrs.push_back(new ExprCtr(e-eq.d.lb(),GEQ));
		ctrs.push_back(new ExprCtr(e-eq.d.ub(),LEQ));
	} catch(DimException& e) {
		throw SyntaxError(e.message(),NULL,eq.expr.line);
	}
}

} // end namespace parser
} // end namespace ibex
