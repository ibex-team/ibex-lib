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
#include "ibex_Exception.h"

namespace ibex {
namespace parser {

ExprGenerator::ExprGenerator(const Scope& scope) : scope(scope) {

}

const ExprNode& ExprGenerator::generate(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {
	return ExprGenerator::copy(old_x,new_x,y,true);
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

	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&LEFT);
		if (c) {
			e.deco.tmp = &ExprConstant::new_(pow(c->get(),expon));
			delete c;
			return;
		}
	}
	e.deco.tmp = &(pow(LEFT,expon));
}

void ExprGenerator::visit(const P_ExprIndex& e) {
	int index=ConstantGenerator(scope).eval_integer(e.right);

	visit(e.left);

	int real_index=e.matlab_style? index-1 : index;

	if (real_index<0)
		throw SyntaxError("negative index. Note: indices in Matlab-style (using parenthesis like in \"x(i)\") start from 1 (not 0).");
	if (real_index>LEFT.dim.max_index())
		throw SyntaxError("index out of bounds. Note: indices in C-style (using square brackets like in \"x[i]\") start from 0 (not 1).");

	if (fold) {
		const ExprConstant* c=dynamic_cast<const ExprConstant*>(&LEFT);
		if (c) {
			e.deco.tmp = &ExprConstant::new_(c->get()[real_index]);
			delete c;
			return;
		}
	}

	const ExprConstantRef* s=dynamic_cast<const ExprConstantRef*>(&LEFT);
	if (s) {
		if (dynamic_cast<const P_ExprIndex*>(e.father)) {
			e.deco.tmp = new ExprConstantRef(s->value[real_index]);
		} else {
			// "last time": we cannot keep reference anymore.
			e.deco.tmp = &ExprConstant::new_(s->value[real_index]);
		}
		delete s;
		return;
	}

	e.deco.tmp = &(LEFT[real_index]);
}

void ExprGenerator::visit(const ExprConstantRef& c) {
	if (dynamic_cast<const P_ExprIndex*>(c.father)) {
		// temporary copy (with domain passed by reference).
		// will be replaced by a ExprConstant at the
		// last time (in P_ExprIndex)
		c.deco.tmp = new ExprConstantRef(c.value);
	} else {
		c.deco.tmp = & ExprConstant::new_(c.value); //ExprConstSymbol(c.value);
	}
}

void ExprGenerator::visit(const ExprIter& x) {
	x.deco.tmp = & ExprConstant::new_scalar(scope.get_iter_value(x.name));
}

void ExprGenerator::visit(const ExprInfinity& x) {
	ibex_warning("infinity value \"oo\" inside an expression means the empty interval");
	x.deco.tmp = & ExprConstant::new_scalar(Interval::EMPTY_SET);
}

} // end namespace parser
} // end namespace ibex
