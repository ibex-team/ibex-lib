//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprExprGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_ExprGenerator.h"
#include "ibex_ConstantGenerator.h"

namespace ibex {
namespace parser {

ExprGenerator::ExprGenerator(const Scope& scope) : scope(scope) {

}

const ExprNode& ExprGenerator::generate(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {
	return ExprGenerator::copy(old_x,new_x,y);
}

void ExprGenerator::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

// (useless so far)
void ExprGenerator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprGenerator::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

// (useless so far)
void ExprGenerator::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void ExprGenerator::visit(const ExprPower& x) {
	assert(false); // only P_ExprPower appears at parse time.
}

void ExprGenerator::visit(const ExprIndex& i) {
	assert(false); // only P_ExprIndex appears at parse time.
}

void ExprGenerator::visit(const P_ExprPower& e) {
	int expon=ConstantGenerator(scope).eval_integer(e.right);
	e.deco.tmp = &(pow(e.left,expon));
}

void ExprGenerator::visit(const P_ExprIndex& e) {
	int index=ConstantGenerator(scope).eval_integer(e.right);
	e.deco.tmp = &(e.left[index]);
}

void ExprGenerator::visit(const ExprIter& x) {
	x.deco.tmp = & ExprConstant::new_scalar(scope.get_iter_value(x.name));
}


} // end namespace parser
} // end namespace ibex
