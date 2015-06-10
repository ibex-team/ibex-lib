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
#include "ibex_Heap.h"
#include "ibex_CellStack.h"
#include "ibex_SetConnectedComponents.cpp_"
#include <stack>
#include <fstream>

using namespace std;

namespace ibex {

SetInterval::SetInterval(const IntervalVector& bounding_box, double eps, bool inner) : root(new SetLeaf(inner? YES: MAYBE)), eps(eps), bounding_box(bounding_box) {

}

SetInterval::SetInterval(const char* filename) : root(NULL), eps(-1), bounding_box(1) {
	load(filename);
}

bool SetInterval::is_empty() const {
	return root==NULL;
}

void SetInterval::sync(Sep& sep) {
	try {
		root = root->inter(true, bounding_box, sep, eps);
	} catch(NoSet& e) {
		delete root;
		root = NULL;
		throw e;
	}
}

void SetInterval::contract(Sep& sep) {
	root = root->inter(false, bounding_box, sep, eps);
}

SetInterval& SetInterval::operator&=(const SetInterval& set) {
	root = root->inter(false, bounding_box, set.root, set.bounding_box, eps);
	return *this;
}

SetInterval& SetInterval::operator|=(const SetInterval& set) {
	root = root->union_(bounding_box, set.root, set.bounding_box, eps);
	return *this;
}

BoolInterval SetInterval::is_superset(const IntervalVector& box) const {
	if (!bounding_box.is_superset(box)) return NO;
	else return root->is_superset(bounding_box,box);
}

void SetInterval::save(const char* filename) {
	std::stack<SetNode*> s;

	s.push(root);

	fstream os;
	os.open(filename, ios::out | ios::trunc | ios::binary);

	os.write((char*) &eps, sizeof(double));

	int n=bounding_box.size();
	os.write((char*) &n, sizeof(int));

	for (int i=0; i<bounding_box.size(); i++) {
		double d; // to store double values
		d=bounding_box[i].lb();
		os.write((char*) &d,sizeof(double));
		d=bounding_box[i].ub();
		os.write((char*) &d,sizeof(double));
	}

	while (!s.empty()) {
		SetNode* node=s.top();
		s.pop();
		if (node->is_leaf()) {
			int no_var=-1; // to store "-1" (means: leaf)
			os.write((char*) &no_var, sizeof(int));
			os.write((char*) &((SetLeaf*) node)->status, sizeof(BoolInterval));
		}
		else {
			SetBisect* b=(SetBisect*) node;
			os.write((char*) &b->var, sizeof(int));
			os.write((char*) &b->pt, sizeof(double));
			s.push(b->right);
			s.push(b->left);
		}
	}
	os.close();
}

void SetInterval::load(const char* filename) {

	std::ifstream is;
	is.open(filename, ios::in | ios::binary);

	is.read((char*) &eps, sizeof(double));
	//cout << "eps=" << eps << endl;

	unsigned int n;
	is.read((char*) &n, sizeof(int));
	//cout << "n=" << n << endl;

	bounding_box.resize(n);

	for (int i=0; i<bounding_box.size(); i++) {
		double lb,ub;
		is.read((char*) &lb, sizeof(double));
		is.read((char*) &ub, sizeof(double));
		bounding_box[i]=Interval(lb,ub);
	}
	//cout << "bounding box=" << bounding_box << endl;

	int var;
	is.read((char*) &var, sizeof(int));

	double pt;
	BoolInterval status;

	if (var==-1) {
		is.read((char*) &status, sizeof(BoolInterval));
		root = new SetLeaf(status);
		is.close();
		return;
	}

	is.read((char*) &pt, sizeof(double));
	std::stack<SetNode*> s;
	root = new SetBisect(var, pt); // left and right are both set to NULL temporarily
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

		is.read((char*) &var, sizeof(int));

		if (var==-1) {
			is.read((char*) &status, sizeof(BoolInterval));
			subnode = new SetLeaf(status);
		} else {
			is.read((char*) &pt, sizeof(double));
			subnode  =new SetBisect(var,pt); // left and right are both set to NULL temporarily
		}

		if (node->left==NULL) {
			node->left=subnode;
			subnode->father = node;
		}
		else {
			assert(node->right==NULL);
			node->right=subnode;
			subnode->father = node;
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

//namespace {


class NodeAndDist : public Backtrackable {
public:
	NodeAndDist() : node(NULL), dist(-1) { }

	NodeAndDist(SetNode* _node) : node(_node), dist(-1) { }

	/**
	 * calculate the square of the distance to pt
	 * for the box of the current cell (box given in argument)
	 */
	void set_dist(const IntervalVector& box, const Vector pt) {
		assert(box.size()==pt.size());

		Interval d=Interval::ZERO;
		for (int i=0; i<pt.size(); i++) {
			d += sqr(box[i]-pt[i]);
		}
		dist=d.lb();
	}

	virtual std::pair<Backtrackable*,Backtrackable*> down() {
		assert(!node->is_leaf());

		SetBisect& b=*((SetBisect*) node);
		return std::pair<NodeAndDist*,NodeAndDist*>(new NodeAndDist(b.left),
													  new NodeAndDist(b.right));
	}

	SetNode* node;
	double dist;
};

/**
 * Cell heap where the criterion is the distance to "pt"
 */
class CellHeapDist : public CostFunc<Cell> {

public:
	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const { return c.get<NodeAndDist>().dist; }
};


double SetInterval::dist(const Vector& pt, bool inside) const {

    if (!inside && !bounding_box.contains(pt)) return 0;

	CellHeapDist costf;
	Heap<Cell> heap(costf);

	//int count=0; // for stats

	Cell* root_cell =new Cell(bounding_box);
	root_cell->add<NodeAndDist>();
	root_cell->get<NodeAndDist>().node = root;
	root_cell->get<NodeAndDist>().set_dist(bounding_box,pt);
	//count++;

	heap.push(root_cell);

	double lb = POS_INFINITY;

	while (!heap.empty()) {

		Cell* c = heap.pop();

		SetNode* node = c->get<NodeAndDist>().node;

		assert(node!=NULL);

		if (node->is_leaf() && ((SetLeaf*) node)->status==(inside? YES : NO)) {
			double d=c->get<NodeAndDist>().dist;
			if (d<lb) {
				lb=d;
				heap.contract(lb);
			}
		} else if (!node->is_leaf()) {
			SetBisect& b= *((SetBisect*) node);

			IntervalVector left=b.left_box(c->box);
			IntervalVector right=b.right_box(c->box);

			std::pair<Cell*,Cell*> p=c->bisect(left,right);

			p.first->get<NodeAndDist>().set_dist(left,pt);
			//count++;
			if (p.first->get<NodeAndDist>().dist<=lb) heap.push(p.first);

			p.second->get<NodeAndDist>().set_dist(right,pt);
			//count++;
			if (p.second->get<NodeAndDist>().dist<=lb) heap.push(p.second);
		}
		delete c;
	}
	//cout << " number of times a distance to a box has been computed: " << count << endl;
	return ::sqrt(lb);
}


IntervalVector SetInterval::node_box(const SetNode* node) const {

	// the first field is an ancestor
	// the second field is whether we are in the left or right branch of this ancestor
	list<pair<const SetBisect*,bool> > ancestors;

	// we go upward in the tree and record the side of each bisection
	for (const SetNode* ancestor=node; ancestor->father!=NULL; ancestor=ancestor->father) {
		ancestors.push_front(pair<const SetBisect*,bool>(ancestor->father, ancestor==ancestor->father->left));  // "true" means "left"
	}
	IntervalVector box=bounding_box;
	// we go backward and apply the bisections recursively from the initial bounding box
	for (list<pair<const SetBisect*,bool> >::const_iterator it=ancestors.begin(); it!=ancestors.end(); it++) {
		box = it->second ? it->first->left_box(box) : it->first->right_box(box);
	}
	return box;
}

SetInterval::~SetInterval() {
	delete root;
}

} // namespace ibex
