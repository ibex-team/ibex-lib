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
#include <algorithm>

namespace ibex {

namespace {
bool compare(const ExprNode* x, const ExprNode* y) { return (x->height>y->height); }
}

const ExprNode** ExprNodes::nodes(const ExprNode& e) {
	map.clean();

	visit(e);

	const ExprNode** subnodes = new const ExprNode*[e.size];
	int i=0;
	for (IBEX_NODE_MAP(const ExprNode*)::const_iterator it=map.begin(); it!=map.end(); it++) {
		subnodes[i++]=it->second;
	}
	// Sort the nodes by decreasing height
	std::sort(subnodes,subnodes+e.size,compare);

	return subnodes;
}

void ExprNodes::visit(const ExprNode& e) {
	if (!map.found(e)) {
		map.insert(e,&e);
		e.acceptVisitor(*this);
	}
}

void ExprNodes::visit(const ExprIndex& i)    { visit(i.expr); }
void ExprNodes::visit(const ExprLeaf& e)     { }
void ExprNodes::visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
void ExprNodes::visit(const ExprBinaryOp& b) { visit(b.left); visit(b.right); }
void ExprNodes::visit(const ExprUnaryOp& u)  {	visit(u.expr); }

} // end namespace ibex
