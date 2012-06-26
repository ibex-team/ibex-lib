//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSize.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_ExprSize.h"
#include "ibex_Expr.h"

namespace ibex {

int bin_size(const ExprNode& left, const ExprNode& right) {
	return ExprSize(left,right).size+1;
}

int nary_size(const ExprNode** args, int n) {
	return ExprSize(args,n).size+1;
}

ExprSize::ExprSize(const ExprNode& l, const ExprNode& r) : size(0) {
	l.reset_visited();
	r.reset_visited();
	visit(l);
	visit(r);
}

ExprSize::ExprSize(const ExprNode** args, int n) : size(0) {
	for (int i=0; i<n; i++) args[i]->reset_visited();
	for (int i=0; i<n; i++) visit(*args[i]);
}

void ExprSize::visit(const ExprNode& e) {
	if (!e.deco.visited) {
		e.deco.visited=true;
		size++;
		e.acceptVisitor(*this);
	}
}

void ExprSize::visit(const ExprIndex& e)    { visit(e.expr); }
void ExprSize::visit(const ExprLeaf& e)     { }
void ExprSize::visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
void ExprSize::visit(const ExprBinaryOp& e) { visit(e.left); visit(e.right); }
void ExprSize::visit(const ExprUnaryOp& e)  { visit(e.expr); }

} // end namespace ibex
