//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 9, 2012
// Last Update : Apr 9, 2012
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include "ibex_ExprCopy.h"
#include "ibex_Expr.h"

namespace ibex {


const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	y.reset_visited();

	assert(new_x.size()>=old_x.size());

	for (int i=0; i<old_x.size(); i++)
		old_x[i].deco.tmp=&new_x[i]; // must be done *after* y.reset_visited()

	visit(y);

	return *(const ExprNode*) y.deco.tmp;
}

void ExprCopy::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

void ExprCopy::visit(const ExprIndex& i) {
	visit(i.expr);
	i.deco.tmp =& (*((const ExprNode*) i.expr.deco.tmp))[i.index];
}

void ExprCopy::visit(const ExprSymbol& x) {

}

void ExprCopy::visit(const ExprConstant& c) {
	c.deco.tmp = &c.copy();
}

// (useless so far)
void ExprCopy::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void ExprCopy::visit(const ExprLeaf& e) {
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

void ExprCopy::visit(const ExprVector& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprVector::new_(args2, e.nb_args, e.row_vector());
}

void ExprCopy::visit(const ExprApply& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprApply::new_(e.func, args2);
}

#define LEFT  (*((const ExprNode*) e.left.deco.tmp))
#define RIGHT (*((const ExprNode*) e.right.deco.tmp))
#define EXPR  (*((const ExprNode*) e.expr.deco.tmp))

void ExprCopy::visit(const ExprAdd& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT + RIGHT); }
void ExprCopy::visit(const ExprMul& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT * RIGHT); }
void ExprCopy::visit(const ExprSub& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT - RIGHT); }
void ExprCopy::visit(const ExprDiv& e)   { visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT / RIGHT); }
void ExprCopy::visit(const ExprMax& e)   { visit(e.left); visit(e.right); e.deco.tmp = & max  (LEFT, RIGHT); }
void ExprCopy::visit(const ExprMin& e)   { visit(e.left); visit(e.right); e.deco.tmp = & min  (LEFT, RIGHT); }
void ExprCopy::visit(const ExprAtan2& e) { visit(e.left); visit(e.right); e.deco.tmp = & atan2(LEFT, RIGHT); }

void ExprCopy::visit(const ExprMinus& e) { visit(e.expr); e.deco.tmp = & -    (EXPR); }
void ExprCopy::visit(const ExprTrans& e) { visit(e.expr); e.deco.tmp = & transpose(EXPR); }
void ExprCopy::visit(const ExprSign& e)  { visit(e.expr); e.deco.tmp = & sign (EXPR); }
void ExprCopy::visit(const ExprAbs& e)   { visit(e.expr); e.deco.tmp = & abs  (EXPR); }
void ExprCopy::visit(const ExprPower& e) { visit(e.expr); e.deco.tmp = & pow  (EXPR,e.expon); }
void ExprCopy::visit(const ExprSqr& e)   { visit(e.expr); e.deco.tmp = & sqr  (EXPR); }
void ExprCopy::visit(const ExprSqrt& e)  { visit(e.expr); e.deco.tmp = & sqrt (EXPR); }
void ExprCopy::visit(const ExprExp& e)   { visit(e.expr); e.deco.tmp = & exp  (EXPR); }
void ExprCopy::visit(const ExprLog& e)   { visit(e.expr); e.deco.tmp = & log  (EXPR); }
void ExprCopy::visit(const ExprCos& e)   { visit(e.expr); e.deco.tmp = & cos  (EXPR); }
void ExprCopy::visit(const ExprSin& e)   { visit(e.expr); e.deco.tmp = & sin  (EXPR); }
void ExprCopy::visit(const ExprTan& e)   { visit(e.expr); e.deco.tmp = & tan  (EXPR); }
void ExprCopy::visit(const ExprCosh& e)  { visit(e.expr); e.deco.tmp = & cosh (EXPR); }
void ExprCopy::visit(const ExprSinh& e)  { visit(e.expr); e.deco.tmp = & sinh (EXPR); }
void ExprCopy::visit(const ExprTanh& e)  { visit(e.expr); e.deco.tmp = & tanh (EXPR); }
void ExprCopy::visit(const ExprAcos& e)  { visit(e.expr); e.deco.tmp = & acos (EXPR); }
void ExprCopy::visit(const ExprAsin& e)  { visit(e.expr); e.deco.tmp = & asin (EXPR); }
void ExprCopy::visit(const ExprAtan& e)  { visit(e.expr); e.deco.tmp = & atan (EXPR); }
void ExprCopy::visit(const ExprAcosh& e) { visit(e.expr); e.deco.tmp = & acosh(EXPR); }
void ExprCopy::visit(const ExprAsinh& e) { visit(e.expr); e.deco.tmp = & asinh(EXPR); }
void ExprCopy::visit(const ExprAtanh& e) { visit(e.expr); e.deco.tmp = & atanh(EXPR); }

} // end ibex namespace





