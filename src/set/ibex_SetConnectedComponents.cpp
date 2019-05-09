//============================================================================
//                                  I B E X                                   
// File        : ibex_SetConnectedComponents.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 26, 2015
//============================================================================

#include <stdlib.h>
#include <cassert>
#include <stack>
#include <list>

#include "ibex_SetBisect.h"
#include "ibex_SetLeaf.h"
#include "ibex_Set.h"

using namespace std;

namespace ibex {

namespace {

class ExtSetNode {
public:
	ExtSetNode(SetNode* node, const IntervalVector& box);
	~ExtSetNode();
	SetNode* node;      // corresponding node.
	int component;
	IntervalVector box;
	list<ExtSetNode*> adj; // adjacency
	ExtSetNode* left;
	ExtSetNode* right;
};


ExtSetNode::ExtSetNode(SetNode* node, const IntervalVector& box) : node(node), component(-1), box(box), adj(), left(NULL), right(NULL) {

}

ExtSetNode::~ExtSetNode() {
	if (left) {
		delete left;
		assert(right);
		delete right;
	}
}

/*
 * Build the extended set (and, in particular, calculate the adjacency list of every leaf).
 */
void init(ExtSetNode* n) {

	SetBisect* b=dynamic_cast<SetBisect*>(n->node);
	if (b) {
		IntervalVector lbox=b->left_box(n->box);
		IntervalVector rbox=b->right_box(n->box);
		n->left  = new ExtSetNode(b->left, lbox);
		n->right = new ExtSetNode(b->right, rbox);

		for (list<ExtSetNode*>::iterator it=n->adj.begin(); it!=n->adj.end(); it++) {
			ExtSetNode* n2=*it;
			list<ExtSetNode*>::iterator it2=n2->adj.begin();
			while ((*it2)!=n && it2!=n2->adj.end()) it2++;
			assert(it2!=n2->adj.end());
			n2->adj.erase(it2);

			if (n2->box.intersects(lbox)) { n->left->adj.push_back(n2); n2->adj.push_back(n->left); }
			if (n2->box.intersects(rbox)) { n->right->adj.push_back(n2); n2->adj.push_back(n->right); }

		}
		n->adj.clear(); // unused anymore (we don't need the adjacency of intermediate nodes)
		n->left->adj.push_back(n->right);
		n->right->adj.push_back(n->left);

		init(n->left);
		init(n->right);
	}
}

/**
 * Fill the stack "s" with all the leaves in the extended set rooted at "n".
 */
void fill_with_leaves(stack<ExtSetNode*>& s, ExtSetNode* n) {
	if (n->left) {
		fill_with_leaves(s,n->left);
		fill_with_leaves(s,n->right);
	} else {
		s.push(n);
	}
}

/**
 * Add into "comp" all the nodes connected to "n".
 * "num" is the component number associated to "comp".
 */
void fill_connected_component(vector<SetLeaf*>& comp, ExtSetNode* n, int num) {
	n->component = num;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(n->node);
	assert(leaf);

	comp.push_back(leaf);

	for (list<ExtSetNode*>::iterator it=n->adj.begin(); it!=n->adj.end(); it++) {
		ExtSetNode* n2=*it;

		SetLeaf* leaf2=dynamic_cast<SetLeaf*>(n2->node);
		assert(leaf2);

		if (n2->component==-1) {
			if (leaf2->status == leaf->status)
				fill_connected_component(comp, n2, num);
		}
		else {
			assert(n2->component==num || leaf2->status!=leaf->status);
		}
	}
}

} // end anonymous namespace

// =================================================================================================================================================

vector<vector<SetLeaf*> > Set::connected_components() {
	vector<vector<SetLeaf*> > result;

	ExtSetNode* ext_root = new ExtSetNode(root,Rn);
	init(ext_root);

	stack<ExtSetNode*> s;

	fill_with_leaves(s,ext_root);

	while (!s.empty()) {
		ExtSetNode* n = s.top();
		s.pop();
		if (n->component!=-1) continue;

		result.push_back(vector<SetLeaf*>());
		fill_connected_component(result.back(), n, result.size()-1);
	}

	delete ext_root;
	return result;
}

} // namespace ibex
