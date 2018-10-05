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
#include "ibex_Function.h"
#include "ibex_Domain.h"

namespace ibex {

void varcopy(const Array<const ExprSymbol>& src, Array<const ExprSymbol>& dest) {
	assert(src.size()<=dest.size());
	for (int i=0; i<src.size(); i++) {
		dest.set_ref(i,ExprSymbol::new_(src[i].name, src[i].dim));
	}
}

bool varequals(const Array<const ExprSymbol>& arg1, Array<const ExprSymbol>& arg2) {
	if (arg1.size()!=arg2.size()) return false;
	for (int j=0; j<arg1.size(); j++)
		if (arg1[j].dim!=arg2[j].dim) return false;
	return true;
}

const ExprNode& var_component(const Array<const ExprSymbol>& args, int i) {

	int n=0; // count the symbols
	int j=0; // count the number of components up to symbol n;
	while (n<args.size() && j<=i) {
		const ExprNode& x=args[n];
		if (j+x.dim.size() > i) { // we have found the right symbol
			switch (x.dim.type()) {
			case Dim::SCALAR :      return x;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   return x[i-j];
			case Dim::MATRIX:       return x[(i-j) / x.dim.nb_cols()][(i-j) % x.dim.nb_cols()];
			}
		} else {
			j+=x.dim.size();
			n++;
		}
	}
	ibex_error("var_component: index exceeds size.");
}

const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, bool shared) {

	if (!shared) clone.clean();

	assert(new_x.size()>=old_x.size());

	for (int i=0; i<old_x.size(); i++) {
		if (shared && clone.found(old_x[i])) continue;
		clone.insert(old_x[i],&new_x[i]);
	}

	visit(y);

	return *clone[y];
}

void ExprCopy::visit(const ExprNode& e) {
	if (!clone.found(e)) {
		e.acceptVisitor(*this);
	}
}

void ExprCopy::visit(const ExprIndex& i) {
	visit(i.expr);
	const ExprNode& node=*clone[i.expr];
	clone.insert(i, &(node[i.index]));
}

void ExprCopy::visit(const ExprSymbol& x) {

}

void ExprCopy::visit(const ExprConstant& c) {
	clone.insert(c, &c.copy());
}


#define ARG(i) (*clone[e.arg(i)])
#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])
#define EXPR   (*clone[e.expr])

void ExprCopy::visit(const ExprVector& e) {
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		args2.set_ref(i,ARG(i));
	}
	clone.insert(e, &ExprVector::new_(args2,e.orient));
}

void ExprCopy::visit(const ExprApply& e) {
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		args2.set_ref(i,ARG(i));
	}

	clone.insert(e, &ExprApply::new_(e.func, args2));
}

void ExprCopy::visit(const ExprChi& e) {
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		args2.set_ref(i,ARG(i));
	}
	clone.insert(e, &ExprChi::new_(args2));
}

void ExprCopy::visit(const ExprGenericBinaryOp& e) {
	                                       visit(e.left); visit(e.right); clone.insert(e,&ExprGenericBinaryOp::new_(e.name,LEFT,RIGHT)); }
void ExprCopy::visit(const ExprAdd& e)   { visit(e.left); visit(e.right); clone.insert(e, &(LEFT+RIGHT));; }
void ExprCopy::visit(const ExprMul& e)   { visit(e.left); visit(e.right); clone.insert(e, &(LEFT*RIGHT)); }
void ExprCopy::visit(const ExprSub& e)   { visit(e.left); visit(e.right); clone.insert(e, &(LEFT-RIGHT)); }
void ExprCopy::visit(const ExprDiv& e)   { visit(e.left); visit(e.right); clone.insert(e, &(LEFT/RIGHT)); }
void ExprCopy::visit(const ExprMax& e)   { visit(e.left); visit(e.right); clone.insert(e, &max(LEFT,RIGHT)); }
void ExprCopy::visit(const ExprMin& e)   { visit(e.left); visit(e.right); clone.insert(e, &min(LEFT,RIGHT)); }
void ExprCopy::visit(const ExprAtan2& e) { visit(e.left); visit(e.right); clone.insert(e, &atan2(LEFT,RIGHT)); }
void ExprCopy::visit(const ExprPower& e) { visit(e.expr); clone.insert(e, &pow(EXPR,e.expon)); }
void ExprCopy::visit(const ExprGenericUnaryOp& e) {
	                                       visit(e.expr); clone.insert(e,&ExprGenericUnaryOp::new_(e.name,EXPR)); }
void ExprCopy::visit(const ExprMinus& e) { visit(e.expr); clone.insert(e,&(-EXPR)); }
void ExprCopy::visit(const ExprTrans& e) { visit(e.expr); clone.insert(e,&transpose (EXPR)); }
void ExprCopy::visit(const ExprSign& e)  { visit(e.expr); clone.insert(e,&sign (EXPR)); }
void ExprCopy::visit(const ExprAbs& e)   { visit(e.expr); clone.insert(e,&abs (EXPR)); }
void ExprCopy::visit(const ExprSqr& e)   { visit(e.expr); clone.insert(e,&sqr (EXPR)); }
void ExprCopy::visit(const ExprSqrt& e)  { visit(e.expr); clone.insert(e,&sqrt (EXPR)); }
void ExprCopy::visit(const ExprExp& e)   { visit(e.expr); clone.insert(e,&exp  (EXPR)); }
void ExprCopy::visit(const ExprLog& e)   { visit(e.expr); clone.insert(e,&log  (EXPR)); }
void ExprCopy::visit(const ExprCos& e)   { visit(e.expr); clone.insert(e,&cos  (EXPR)); }
void ExprCopy::visit(const ExprSin& e)   { visit(e.expr); clone.insert(e,&sin  (EXPR)); }
void ExprCopy::visit(const ExprTan& e)   { visit(e.expr); clone.insert(e,&tan  (EXPR)); }
void ExprCopy::visit(const ExprCosh& e)  { visit(e.expr); clone.insert(e,&cosh (EXPR)); }
void ExprCopy::visit(const ExprSinh& e)  { visit(e.expr); clone.insert(e,&sinh (EXPR)); }
void ExprCopy::visit(const ExprTanh& e)  { visit(e.expr); clone.insert(e,&tanh (EXPR)); }
void ExprCopy::visit(const ExprAcos& e)  { visit(e.expr); clone.insert(e,&acos (EXPR)); }
void ExprCopy::visit(const ExprAsin& e)  { visit(e.expr); clone.insert(e,&asin (EXPR)); }
void ExprCopy::visit(const ExprAtan& e)  { visit(e.expr); clone.insert(e,&atan (EXPR)); }
void ExprCopy::visit(const ExprAcosh& e) { visit(e.expr); clone.insert(e,&acosh(EXPR)); }
void ExprCopy::visit(const ExprAsinh& e) { visit(e.expr); clone.insert(e,&asinh(EXPR)); }
void ExprCopy::visit(const ExprAtanh& e) { visit(e.expr); clone.insert(e,&atanh(EXPR)); }

} // end ibex namespace





