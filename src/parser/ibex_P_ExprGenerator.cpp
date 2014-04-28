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
	if (!clone.found(e)) {
		e.acceptVisitor(*this);
	}
}

#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])

void ExprGenerator::visit(const P_ExprPower& e) {

	visit(e.left);
	visit(e.right);

	if (!fold) {
		// TODO: if fold is false, even x^(1) will be transformed into exp(1*ln(x)) :-(
		mark(e.left);
		mark(e.right);
		clone.insert(e, &exp(RIGHT * log(LEFT)));
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
		//delete cr; // not now (see comment in ExprCopy.h)
		// NOTE: we can delete cr because
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
		//delete cl; // LEFT will no longer be used // not now (see comment in ExprCopy.h)
	} else
		left_type=IBEX_EXPRNODE;


	if (left_type==IBEX_INTERVAL) {
		if (right_type==IBEX_INTEGER) {
			clone.insert(e, &ExprConstant::new_scalar(pow(itv_left,int_right)));
		} else if (right_type==IBEX_INTERVAL) {
			clone.insert(e, &ExprConstant::new_scalar(pow(itv_left,itv_right)));
		} else {
			mark(e.right);
			clone.insert(e, &exp(RIGHT * log(itv_left))); // *log(...) will create a new ExprConstant.
		}
	}  else {
		mark(e.left);
		if (right_type==IBEX_INTEGER) {
			clone.insert(e, &(pow(LEFT,int_right)));
		} else if (right_type==IBEX_INTERVAL) { // do not forget this case (RIGHT does not exist anymore)
			clone.insert(e, &exp(itv_right * log(LEFT)));
		} else {
			mark(e.right);
			clone.insert(e, &exp(RIGHT * log(LEFT)));
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
			clone.insert(e, &ExprConstant::new_(c->get()[real_index]));
			//delete c; // not now (see comment in ExprCopy.h)
			return;
		}
	}

	const ExprConstantRef* s=dynamic_cast<const ExprConstantRef*>(&LEFT);
	if (s) {
		assert(e.fathers.size()==1);
		if (dynamic_cast<const P_ExprIndex*>(&e.fathers[0])) {
			clone.insert(e, new ExprConstantRef(s->value[real_index]));
		} else {
			// "last time": we cannot keep reference anymore.
			clone.insert(e, &ExprConstant::new_(s->value[real_index]));
		}
		//delete s; // not now: there may be inside a DAG (via a function) (see comment in ExprCopy.h)
		return;
	}

	mark(e.left);
	clone.insert(e, &(LEFT[real_index]));
}

void ExprGenerator::visit(const ExprConstantRef& c) {
	assert(c.fathers.size()==1);
	if (dynamic_cast<const P_ExprIndex*>(&c.fathers[0])) {
		// temporary copy (with domain passed by reference).
		// will be replaced by a ExprConstant at the
		// last time (in P_ExprIndex)
		clone.insert(c, new ExprConstantRef(c.value));
	} else {
		clone.insert(c, & ExprConstant::new_(c.value)); //ExprConstSymbol(c.value));
	}
}

void ExprGenerator::visit(const ExprIter& x) {
	clone.insert(x, & ExprConstant::new_scalar(scope.get_iter_value(x.name)));
}

void ExprGenerator::visit(const ExprInfinity& x) {
	ibex_warning("infinity value \"oo\" inside an expression means the empty interval");
	clone.insert(x, & ExprConstant::new_scalar(Interval::EMPTY_SET));
}

} // end namespace parser
} // end namespace ibex
