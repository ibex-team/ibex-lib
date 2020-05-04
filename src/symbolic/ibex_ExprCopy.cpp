//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 9, 2012
// Last Update : Apr 16, 2019
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <sstream>

#include "ibex_ExprCopy.h"
#include "ibex_Function.h"
#include "ibex_Domain.h"

using namespace std;

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

	if (!shared) cache.clean();

	for (int i=0; i<std::min(new_x.size(), old_x.size()); i++) {
		if (shared && cache.found(old_x[i])) continue;
		cache.insert(old_x[i],&new_x[i]);
	}

	return *visit(y);
}

const ExprNode* ExprCopy::visit(const ExprNode& e) {
	// ok, apply default (superclass) implementation
	return ExprVisitor<const ExprNode*>::visit(e);
}

const ExprNode* ExprCopy::visit(const ExprIndex& i) {
	return &ExprIndex::new_(*visit(i.expr),i.index);
}

const ExprNode* ExprCopy::visit(const ExprSymbol& x) {
	stringstream s;
	s << "[ExprCopy] no destination node for symbol" << x;
	ibex_error(s.str().c_str());
	return NULL;
}

const ExprNode* ExprCopy::visit(const ExprConstant& c) {
	return &c.copy();
}

const ExprNode* ExprCopy::nary(const ExprNAryOp& e, std::function<const ExprNAryOp&(const Array<const ExprNode>&)> f) {
	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,*visit(e.arg(i)));
	}
	return &f(args2);
}

const ExprNode* ExprCopy::visit(const ExprVector& e) {
	return nary(e, [&e](const Array<const ExprNode>& args)->const ExprVector& { return ExprVector::new_(args,e.orient); });
}

const ExprNode* ExprCopy::visit(const ExprApply& e) {
	return nary(e, [&e](const Array<const ExprNode>& args)->const ExprApply& { return ExprApply::new_(*e.f,args); });
}

const ExprNode* ExprCopy::visit(const ExprChi& e) {
	return nary(e, [](const Array<const ExprNode>& args)->const ExprChi& { return ExprChi::new_(args); });
}

const ExprNode* ExprCopy::visit(const ExprGenericBinaryOp& e) {
	                                       	   	   	  return &ExprGenericBinaryOp::new_(e.name,*visit(e.left),*visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprAdd& e)   { return &(*visit(e.left) + *visit(e.right));; }
const ExprNode* ExprCopy::visit(const ExprMul& e)   { return &(*visit(e.left) * *visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprSub& e)   { return &(*visit(e.left) - *visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprDiv& e)   { return &(*visit(e.left) / *visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprMax& e)   { return &max(*visit(e.left),*visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprMin& e)   { return &min(*visit(e.left),*visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprAtan2& e) { return &atan2(*visit(e.left),*visit(e.right)); }
const ExprNode* ExprCopy::visit(const ExprPower& e) { return &pow(*visit(e.expr),e.expon); }
const ExprNode* ExprCopy::visit(const ExprGenericUnaryOp& e)
													{ return &ExprGenericUnaryOp::new_(e.name,*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprMinus& e) { return &(-*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprTrans& e) { return &transpose(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSign& e)  { return &sign(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAbs& e)   { return &abs(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSqr& e)   { return &sqr(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSqrt& e)  { return &sqrt(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprExp& e)   { return &exp (*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprLog& e)   { return &log (*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprCos& e)   { return &cos (*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSin& e)   { return &sin (*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprTan& e)   { return &tan (*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprCosh& e)  { return &cosh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSinh& e)  { return &sinh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprTanh& e)  { return &tanh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAcos& e)  { return &acos(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAsin& e)  { return &asin(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAtan& e)  { return &atan(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAcosh& e) { return &acosh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAsinh& e) { return &asinh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprAtanh& e) { return &atanh(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprFloor& e) { return &floor(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprCeil& e)  { return &ceil(*visit(e.expr)); }
const ExprNode* ExprCopy::visit(const ExprSaw& e)   { return &saw (*visit(e.expr)); }

} // end ibex namespace





