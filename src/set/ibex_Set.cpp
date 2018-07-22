//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_Set.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"
#include "ibex_Heap.h"
#include "ibex_CellStack.h"
#include "ibex_SetConnectedComponents.cpp_"
#include "ibex_SepFwdBwd.h"
#include "ibex_String.h"
#include "ibex_Id.h"
#include "ibex_Bxp.h"

#include <stack>
#include <fstream>

using namespace std;

namespace ibex {


IntervalVector Set::inflate_one_float(const IntervalVector& box) {
	IntervalVector inflated=box;
	for (int i=0;i<box.size();i++) {
		double lb=box[i].lb();
		double ub=box[i].ub();
		inflated[i]=Interval(lb==NEG_INFINITY? lb : previous_float(lb), ub==POS_INFINITY? ub : next_float(ub));
	}
	return inflated;
}

Set::Set(int n, BoolInterval status) : root(new SetLeaf(status)), Rn(n) {

}

Set::Set(const IntervalVector& box, BoolInterval status) : root(NULL), Rn(box.size()) {

	if (status==YES) { // need to create a boundary around the box
		IntervalVector inflated=inflate_one_float(box);

		// create the complementary of the box
		pair<SetNode*,SetLeaf*> p=diff(Rn, inflated, NO, MAYBE, 0);

		// create the boundary around the box
		p.second->replace_with(diff(inflated, box, MAYBE, YES, 0).first);

		root = p.first;
	} else {
		pair<SetNode*,SetLeaf*> p=diff(Rn, box, NO, MAYBE, 0);

		root = p.first;
	}
}

Set::Set(Function& f, CmpOp op, double eps) : root(new SetLeaf(YES)), Rn(f.nb_var()) {
	NumConstraint ctr(f,op);
	SepFwdBwd sep(ctr);
	sep.contract(*this,eps);
}

Set::Set(NumConstraint& ctr, double eps) : root(new SetLeaf(YES)), Rn(ctr.f.nb_var()) {
	SepFwdBwd sep(ctr);
	sep.contract(*this,eps);
}

Set::Set(const System& sys, double eps) : root(new SetLeaf(YES)), Rn(sys.nb_var) {
	SepFwdBwd sep(sys);
	sep.contract(*this,eps);
}

Set::Set(const char* filename) : root(NULL), Rn(1) {
	load(filename);
}

bool Set::is_empty() const {
	return root->is_leaf() && ((SetLeaf*) root)->status==NO;
}

Set& Set::operator&=(const Set& set) {
	assert(set.Rn.size()==Rn.size());
	root = root->inter(false, Rn, set.root, Rn);
	return *this;
}

Set& Set::operator|=(const Set& set) {
	root = root->union_(Rn, set.root, Rn);
	return *this;
}

BoolInterval Set::is_superset(const IntervalVector& box) const {
	return root->is_superset(Rn,box);
}

void Set::save(const char* filename) {
	std::stack<SetNode*> s;

	s.push(root);

	fstream os;
	os.open(filename, ios::out | ios::trunc | ios::binary);

//	os.write((char*) &eps, sizeof(double));

	int n=Rn.size();
	os.write((char*) &n, sizeof(int));

//	for (int i=0; i<bounding_box.size(); i++) {
//		double d; // to store double values
//		d=bounding_box[i].lb();
//		os.write((char*) &d,sizeof(double));
//		d=bounding_box[i].ub();
//		os.write((char*) &d,sizeof(double));
//	}

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

void Set::load(const char* filename) {

	std::ifstream is;
	is.open(filename, ios::in | ios::binary);

//	is.read((char*) &eps, sizeof(double));
	//cout << "eps=" << eps << endl;

	unsigned int n;
	is.read((char*) &n, sizeof(int));
	//cout << "n=" << n << endl;

	Rn.resize(n);

//	for (int i=0; i<bounding_box.size(); i++) {
//		double lb,ub;
//		is.read((char*) &lb, sizeof(double));
//		is.read((char*) &ub, sizeof(double));
//		bounding_box[i]=Interval(lb,ub);
//	}
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

void Set::visit(SetVisitor& visitor) const {
	root->visit(Rn,visitor);
}

std::ostream& operator<<(std::ostream& os, const Set& set) {
	set.root->print(os,set.Rn, 0);
	return os;
}

class BxpNodeAndDist : public Bxp {
public:
	BxpNodeAndDist(const IntervalVector& root_box, const Vector& pt, SetNode* _node) : Bxp(next_id()), pt(pt), node(_node) {
		set_dist(root_box,pt);
	}

	BxpNodeAndDist(long id, const IntervalVector& box, const Vector& pt, SetNode* _node) : Bxp(id), pt(pt), node(_node) {
		set_dist(box,pt);
	}

	BxpNodeAndDist(const BxpNodeAndDist& p) : Bxp(p.id), pt(p.pt), node(p.node), dist(p.dist) { }

	virtual Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const {
		return new BxpNodeAndDist(*this);
	}

	virtual void update(const BoxEvent& e, const BoxProperties& p) {
		set_dist(e.box,pt); // actually never called by Set::dist(...)
	}

	const Vector& pt;
	SetNode* node;
	double dist;

protected:
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
};

/**
 * Cell heap where the criterion is the distance to "pt"
 */
class CellHeapDist : public CostFunc<Cell> {

public:
	CellHeapDist(long prop_key) : key(prop_key) { }

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const {
		return ((BxpNodeAndDist*) c.prop[key])->dist;
	}

	long key;
};


double Set::dist(const Vector& pt, bool inside) const {

	long key = next_id();


	//int count=0; // for stats

	Cell* root_cell =new Cell(Rn);
	BxpNodeAndDist* nad=new BxpNodeAndDist(Rn,pt,root);
	root_cell->prop.add(nad);

	//count++;
	CellHeapDist costf(key);

	Heap<Cell> heap(costf);
	heap.push(root_cell);

	double lb = POS_INFINITY;

	while (!heap.empty()) {

		Cell* c = heap.pop();
		BxpNodeAndDist* nad=(BxpNodeAndDist*) c->prop[key];
		SetNode* node = nad->node;

		assert(node!=NULL);

		if (node->is_leaf() && ((SetLeaf*) node)->status==(inside? YES : NO)) {
			double d=nad->dist;
			if (d<lb) {
				lb=d;
				heap.contract(lb);
			}
		} else if (!node->is_leaf()) {
			SetBisect& b= *((SetBisect*) node);

            // the box is bisected twice: could be avoided.
			std::pair<Cell*,Cell*> p=c->bisect(BisectionPoint(b.var,b.pt,false));

			BxpNodeAndDist* nad1=(BxpNodeAndDist*) p.first->prop[key];
			BxpNodeAndDist* nad2=(BxpNodeAndDist*) p.second->prop[key];

			nad1->node = b.left;
			nad2->node = b.right;

			//count++;
			if (nad1->dist<=lb) heap.push(p.first);
			//count++;
			if (nad2->dist<=lb) heap.push(p.second);
		}
		delete c;
	}
	//cout << " number of times a distance to a box has been computed: " << count << endl;
	return ::sqrt(lb);
}


IntervalVector Set::node_box(const SetNode* node) const {

	// the first field is an ancestor
	// the second field is whether we are in the left or right branch of this ancestor
	list<pair<const SetBisect*,bool> > ancestors;

	// we go upward in the tree and record the side of each bisection
	for (const SetNode* ancestor=node; ancestor->father!=NULL; ancestor=ancestor->father) {
		ancestors.push_front(pair<const SetBisect*,bool>(ancestor->father, ancestor==ancestor->father->left));  // "true" means "left"
	}
	IntervalVector box=Rn;
	// we go backward and apply the bisections recursively from the initial bounding box
	for (list<pair<const SetBisect*,bool> >::const_iterator it=ancestors.begin(); it!=ancestors.end(); it++) {
		box = it->second ? it->first->left_box(box) : it->first->right_box(box);
	}
	return box;
}

Set::~Set() {
	delete root;
}

} // namespace ibex
