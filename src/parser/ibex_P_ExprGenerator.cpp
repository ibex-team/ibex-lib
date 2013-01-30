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
#define RIGHT (*((const ExprNode*) e.right.deco.tmp))

void ExprGenerator::visit(const P_ExprPower& e) {

	visit(e.left);
	visit(e.right);

	if (!fold) {
		// if fold is false, even x^(1) will be transformed into exp(1*ln(x)) :-(
		e.deco.tmp = &exp(RIGHT * log(LEFT));
		return;
	}

	typedef enum { IBEX_INTEGER, IBEX_INTERVAL, IBEX_EXPRNODE } _type;
	_type right_type;
	_type left_type;

	int int_right=0;
	Interval itv_right;
	Interval itv_left;

	const ExprConstant* cr=dynamic_cast<const ExprConstant*>(&RIGHT);
	if (cr) {
		if (!cr->dim.is_scalar()) throw SyntaxError("exponent must be scalar");

		right_type=IBEX_INTERVAL;
		itv_right=cr->get_value();
		delete cr;
		// NOTE: we can delete cr right now because
		// even in the case where right_type==IBEX_INTERVAL and left_type==IBEX_EXPRNODE
		// we will recreate a ExprConstant node by multiplying itv_right (instead of RIGHT)
		// with LEFT.

		// try to see if the exponent is an integer
		if (itv_right.is_degenerated()) {
			double x=itv_right.mid();
			if (floor(x)==x) {
				right_type=IBEX_INTEGER;
				int_right=(int)floor(x);
			}
		}
	} else
		right_type=IBEX_EXPRNODE;


	const ExprConstant* cl=dynamic_cast<const ExprConstant*>(&LEFT);
	if (cl) {
		left_type=IBEX_INTERVAL;
		itv_left=cl->get_value();
		delete cl; // LEFT will no longer be used
	} else
		left_type=IBEX_EXPRNODE;


	if (left_type==IBEX_INTERVAL) {
		if (right_type==IBEX_INTEGER) {
			e.deco.tmp = &ExprConstant::new_scalar(pow(itv_left,int_right));
		} else if (right_type==IBEX_INTERVAL) {
			e.deco.tmp = &ExprConstant::new_scalar(pow(itv_left,itv_right));
		} else {
			e.deco.tmp = &exp(RIGHT * log(itv_left)); // *log(...) will create a new ExprConstant.
		}
	}  else {
		if (right_type==IBEX_INTEGER) {
			e.deco.tmp = &(pow(LEFT,int_right));
		} else if (right_type==IBEX_INTERVAL) { // do not forget this case (RIGHT does not exist anymore)
			e.deco.tmp = &exp(itv_right * log(LEFT));
		} else {
			e.deco.tmp = &exp(RIGHT * log(LEFT));
		}
	}

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
