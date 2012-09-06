//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprExprGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================

#include "ibex_P_ExprGenerator.h"
#include "ibex_ConstantGenerator.h"
#include "ibex_SyntaxError.h"

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

#define LEFT  (*((const ExprNode*) e.left.deco.tmp))

void ExprGenerator::visit(const P_ExprPower& e) {
	int expon=ConstantGenerator(scope).eval_integer(e.right);
	visit(e.left);
	e.deco.tmp = &(pow(LEFT,expon));
}

void ExprGenerator::visit(const P_ExprIndex& e) {
	int index=ConstantGenerator(scope).eval_integer(e.right);
	if (index<=0) throw SyntaxError("Indices start from 1 (not 0)","",-1);
	visit(e.left);
	e.deco.tmp = &(LEFT[index-1]);
}

void ExprGenerator::visit(const ExprIter& x) {
	x.deco.tmp = & ExprConstant::new_scalar(scope.get_iter_value(x.name));
}


} // end namespace parser
} // end namespace ibex
