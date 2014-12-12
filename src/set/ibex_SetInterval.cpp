//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetInterval.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"
#include <stack>
#include <fstream>

using namespace std;

namespace ibex {

SetInterval::SetInterval(const IntervalVector& bounding_box, double eps, bool inner) : root(new SetLeaf(inner? __IBEX_IN__: __IBEX_UNK__)), eps(eps), bounding_box(bounding_box) {

}

SetInterval::SetInterval(const char* filename) : root(NULL), eps(-1), bounding_box(1) {
	load(filename);
}

bool SetInterval::is_empty() const {
	return root==NULL;
}

void SetInterval::sync(Separator& sep) {
	try {
		root = root->sync(bounding_box, sep, eps);
	} catch(NoSet& e) {
		delete root;
		root = NULL;
		throw e;
	}
}

void SetInterval::contract(Separator& sep) {
	root->set_in_tmp();
	root = root->inter(bounding_box, sep, eps);
	root->unset_in_tmp();
}

SetInterval& SetInterval::operator&=(const SetInterval& set) {
	root->set_in_tmp();
	root = root->inter(bounding_box, set.root, set.bounding_box, eps);
	root->unset_in_tmp();
	return *this;
}

SetInterval& SetInterval::operator|=(const SetInterval& set) {
	root = root->union_(bounding_box, set.root, set.bounding_box, eps);
	return *this;
}

void SetInterval::save(const char* filename) {
	std::stack<SetNode*> s;

	s.push(root);

	fstream os;
	os.open(filename, ios::out | ios::trunc | ios::binary);

	os << eps;

	os << (unsigned int) bounding_box.size();

	for (int i=0; i<bounding_box.size(); i++) {
		os << bounding_box[i].lb();
		os << bounding_box[i].ub();
	}

	while (!s.empty()) {
		SetNode* node=s.top();
		s.pop();
		if (node->is_leaf()) {
			os << -1; // means: leaf
			os << (unsigned int&) node->status;
		}
		else {
			SetBisect* b=(SetBisect*) node;
			os << b->var;
			os << b->pt;
			s.push(b->right);
			s.push(b->left);
		}
	}
	os.close();
}

void SetInterval::load(const char* filename) {

	std::ifstream is;
	is.open(filename, ios::in | ios::binary);

	is >> eps;
	cout << "eps=" << eps << endl;

	unsigned int n;
	is >> n;
	cout << "n=" << n << endl;

	bounding_box.resize(n);

	for (int i=0; i<bounding_box.size(); i++) {
		double lb,ub;
		is >> lb;
		is >> ub;
		bounding_box[i]=Interval(lb,ub);
	}
	cout << "bounding box=" << bounding_box << endl;

	int var;
	is >> var;

	double pt;
	unsigned int status;

	if (var==-1) {
		is >> status;
		root = new SetLeaf((NodeType&) status);
		is.close();
		return;
	}

	is >> pt;
	std::stack<SetNode*> s;
	root = new SetBisect(var, pt, NULL /* tmp */, NULL /* tmp */);
	s.push(root);

	SetBisect *node;
	SetNode* subnode;

	while (!s.empty()) {

		assert(!s.top()->is_leaf());

		node = (SetBisect*) s.top();

		// =============== backtrack ======================
		if (node->left && node->right) {
			s.pop();
			continue;
		}

		is >> var;

		if (var==-1) {
			is >> status;
			subnode = new SetLeaf((NodeType&) status);
		} else {
			is >> pt;
			subnode  =new SetBisect(var,pt,NULL,NULL);

		}

		if (node->left==NULL) node->left=subnode;
		else {
			assert(node->right==NULL);
			node->right=subnode;
		}

		if (var!=-1)
			s.push(subnode);
	}

	is.close();
}

void SetInterval::visit_leaves(SetNode::leaf_func func) const {
	root->visit_leaves(func, bounding_box);
}

std::ostream& operator<<(std::ostream& os, const SetInterval& set) {
	set.root->print(os,set.bounding_box, 0);
	return os;
}

SetInterval::~SetInterval() {
	delete root;
}

} // namespace ibex
