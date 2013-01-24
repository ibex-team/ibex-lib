//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprNodes.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_ExprNodes.h"
#include "ibex_Expr.h"

namespace ibex {

const ExprNode** ExprNodes::nodes(const ExprNode& e) {
	subnodes = new const ExprNode*[e.size];
	e.reset_visited();
	j=0;
	visit(e);
	return subnodes;
}

void ExprNodes::visit(const ExprNode& e) {
	if (!e.deco.visited) {
		e.deco.visited=true;
		subnodes[j++]=&e;
		e.acceptVisitor(*this);
	}
}

void ExprNodes::visit(const ExprIndex& i)    { visit(i.expr); }
void ExprNodes::visit(const ExprLeaf& e)     { }
void ExprNodes::visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
void ExprNodes::visit(const ExprBinaryOp& b) { visit(b.left); visit(b.right); }
void ExprNodes::visit(const ExprUnaryOp& u)  {	visit(u.expr); }

} // end namespace ibex
