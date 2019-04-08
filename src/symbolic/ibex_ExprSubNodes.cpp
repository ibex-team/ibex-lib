//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSubNodes.cpp
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
	NodeMap<bool> visited;

	ExprNodes(const Array<const ExprSymbol>* args) {
		//visited.clean();
		if (args)
			// We want to keep the control on the insertion order
			// of arguments so we set the arguments as "visited"
			for (int i=0; i<args->size(); i++) {
				visited.insert((*args)[i],true);
				// We don't insert the arguments right now in "nodes"
				// just to let constants be placed before in the vector
				// (so that constants will appear before variables in
				// the final ordering).

			}
	}

	void visit(const ExprIndex& i)    { visit(i.expr); }
	void visit(const ExprLeaf& e)     { }
	void visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
	void visit(const ExprBinaryOp& b) { visit(b.left); visit(b.right); }
	void visit(const ExprUnaryOp& u)  {	visit(u.expr); }

	void visit(const ExprNode& e)     {
		if (!visited.found(e)) {
			e.acceptVisitor(*this);
			nodes.push_back(&e);
			visited.insert(e,true);
		}
	}
};

bool compare(const ExprNode* x, const ExprNode* y) { return (x->height>y->height); }

} // end anonymous namespace

ExprSubNodes::ExprSubNodes() : tab(NULL), _size(0) {

}

ExprSubNodes::ExprSubNodes(const ExprNode& e) {
	init(NULL, e, true);
}

ExprSubNodes::ExprSubNodes(Array<const ExprNode> exprs) {
	init(NULL, exprs, false);
}

ExprSubNodes::ExprSubNodes(const Array<const ExprSymbol>& args, const ExprNode& e, bool var_DFS) {
	init(&args, e, var_DFS);
}

void ExprSubNodes::init(const Array<const ExprSymbol>* args, const Array<const ExprNode>& e, bool symb_DFS) {

	ExprNodes en(symb_DFS? NULL : args);

	for (int i=0; i<e.size(); i++)
		en.visit(e[i]);

	if (args) {
		for (int i=0; i<args->size(); i++) {
			if (!symb_DFS || (symb_DFS && !en.visited.found((*args)[i]))) {
				en.nodes.push_back(&(*args)[i]);
			}
		}
	}

	_size=en.nodes.size();

	// only true for a single expr:
	//assert((args && e.size<=_size) || (!args && e.size==_size));

	tab = new const ExprNode*[_size];
	int i=0;

	// ============ 1st method ============
	// It appears that with the problem bearing.bch, the following order
	// gives much better results... but we loses
	// reproducibility (because, by copying an expression,
	// the ids hence the order changes)
//	for (IBEX_NODE_MAP(const ExprNode*)::const_iterator it=en.map.begin(); it!=en.map.end(); it++) {
//		assert(i<e.size);
//		tab[i++]=it->second;
//	}

	// ============ 2nd method ============
	for (vector<const ExprNode*>::iterator it=en.nodes.begin(); it!=en.nodes.end(); it++) {
		tab[i++]=*it;
	}

	// Sort the nodes by decreasing height
	// stable_sort (versus sort) will maintain current ordering between elements of equal height
	std::stable_sort(tab,tab+_size,compare);

	for (int i=0; i<_size; i++) {
		map.insert(*tab[i],i);
	}
}

ExprSubNodes::~ExprSubNodes() {
	if (tab) delete[] tab;
	// it may happen tab==NULL e.g. with an unconstrained system
	// (the main function is not initialized and so are the "compiled function"
	// and the nodes inside)
}

} // end namespace ibex
