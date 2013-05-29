//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprNodes.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_ExprSubNodes.h"
#include "ibex_Expr.h"
#include <algorithm>

using std::vector;

namespace ibex {

namespace {

class ExprNodes : public virtual ExprVisitor {
public:
	vector<const ExprNode*> nodes;
	NodeMap<const ExprNode*> map;

	ExprNodes() {
		//map.clean();
	}

	void visit(const ExprIndex& i)    { visit(i.expr); }
	void visit(const ExprLeaf& e)     { }
	void visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
	void visit(const ExprBinaryOp& b) { visit(b.left); visit(b.right); }
	void visit(const ExprUnaryOp& u)  {	visit(u.expr); }

	void visit(const ExprNode& e)     {
		if (!map.found(e)) {
			map.insert(e,&e);
			nodes.push_back(&e);
			e.acceptVisitor(*this);
		}
	}
};

bool compare(const ExprNode* x, const ExprNode* y) { return (x->height>y->height); }

} // end anonymous namespace

SubNodes::SubNodes() : tab(NULL) {

}

SubNodes::SubNodes(const ExprNode& e) {
	init(e);
}

void SubNodes::init(const ExprNode& e) {

	ExprNodes en;
	en.visit(e);

	tab = new const ExprNode*[e.size];
	int i=0;
	//for (IBEX_NODE_MAP(const ExprNode*)::const_iterator it=en.map.begin(); it!=en.map.end(); it++) {
	for (vector<const ExprNode*>::iterator it=en.nodes.begin(); it!=en.nodes.end(); it++) {
		assert(i<e.size);
		tab[i++]=*it;
	}
	assert(i==e.size);

	// Sort the nodes by decreasing height
	std::sort(tab,tab+e.size,compare);

	for (int i=0; i<e.size; i++) {
		map.insert(*tab[i],i);
	}
}

SubNodes::~SubNodes() {
	if (tab) delete[] tab;
	// it may happen tab==NULL e.g. with an unconstrained system
	// (the main function is not initialized and so are the "compiled function"
	// and the nodes inside)
}

} // end namespace ibex
