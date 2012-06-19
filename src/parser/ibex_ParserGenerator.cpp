//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_ParserGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_ParserNumConstraint.h"
#include "ibex_ParserSource.h"
#include "ibex_ParserResult.h"
#include "ibex_Function.h"
#include "ibex_ConstantGenerator.h"

using namespace std;

namespace ibex {
namespace parser {

Generator::Generator(const P_Source& source, P_Result& result) : result(result) {

	assert(ctrs.empty()); //clear(); // cleanup

	for (vector<P_NumConstraint*>::const_iterator it=source.ctrs.begin(); it!=source.ctrs.end(); it++) {
		visit(**it);
	}

	int m=ctrs.size();

	Array<const ExprNode> image(m);
	int i=0;

	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		image.set_ref(i++,*(it->first));
	}

	Array<const ExprSymbol> all_vars(source.vars.size()+source.eprs.size()+source.sybs.size());

	i=0;
	for (int j=0; j<source.vars.size(); j++) all_vars.set_ref(i++,*source.vars[j]);
	for (int j=0; j<source.eprs.size(); j++) all_vars.set_ref(i++,*source.eprs[j]);
	for (int j=0; j<source.sybs.size(); j++) all_vars.set_ref(i++,*source.sybs[j]);

	Function* f=new Function(all_vars,ExprVector::new_(image,false));

	vector<NumConstraint*> res;

	i=0;
	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		res.push_back(new NumConstraint(f[i++], it->second));
	}
}

void Generator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void Generator::visit(const P_OneConstraint& c) {
	const ExprNode& e=generate(c.expr);
	ctrs.push_back(pair<const ExprNode*, NumConstraint::CompOp>(&e,c.op));
}

void Generator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;
	int         begin    = loop.first_value; //scope.it_first_value(name);
	int         end      = loop.last_value; //scope.it_last_value(name);

	scopes.push(scopes.top());
	scopes.top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes.top().set_iter_value(name,i);
		for (int j=0; j<=loop.ctrs.size(); j++) {
			visit(*loop.ctrs[j]);
		}
	}
	scopes.pop();
}

const ExprNode& Generator::generate(const ExprNode& expr) {
	expr.reset_visited();
	visit(expr);
	return *((const ExprNode*) expr.deco.tmp);
}

Generator::~Generator() {

}

void Generator::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

// (useless so far)
void ExprCopy::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprCopy::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

// (useless so far)
void ExprCopy::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void Generator::visit(const ExprPower& x) {
	assert(false); // only P_ExprPower appears at parse time.
}

void Generator::visit(const ExprIndex& i) {
	assert(false); // only P_ExprIndex appears at parse time.
}

void Generator::visit(const ExprSymbol& x) {
	assert(false); // only P_ExprSymbol appears at parse time.
}

void Generator::visit(const P_ExprPower& e) {
	int expon=ConstantEvaluator(scopes.top()).eval_integer(e.right);
	e.deco.tmp = &(pow(e.left,expon));
}


void Generator::visit(const P_ExprIndex& e) {
	int index=ConstantEvaluator(scopes.top()).eval_integer(e.right);
	e.deco.tmp = &(e.left[*index]);
}

void Generator::visit(const P_ExprSymbol& x) {
	if (scopes.top().is_iter_symbol(x.name)) {
		x.deco.tmp = & ExprConstant::new_scalar(scopes.top().get_iter_value(x.name));
	} else if (scopes.top().is_cst_symbol(x.name)) {
		x.deco.tmp = & ExprConstant::new_(scopes.top().get_cst(x.name));
	} else {
		x.deco.tmp = & ExprSymbol::new_(x.name,x.dim);
		result.domains.push_back(x.domain);
	}
}


} // end namespace parser
} // end namespace ibex
