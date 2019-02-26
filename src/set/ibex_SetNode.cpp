//============================================================================
//                                  I B E X                                   
// File        : ibex_SetNode.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetNode.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"

using namespace std;

namespace ibex {

SetNode::SetNode() : father(NULL) {

}

SetNode::~SetNode() {

}


namespace {

// determine if box1 has a larger diameter than box2
// return the index of the box largest dimension (either an index for box1 or box2)
pair<bool,int> largest_diameter(const IntervalVector& box1, const IntervalVector& box2) {

	int i1=box1.extr_diam_index(false);
	int i2=box2.extr_diam_index(false);

	if (box1[i1].is_unbounded()) {
		if (!box2[i2].is_unbounded())
			return pair<bool,int>(true,i1);
		else {
			double pt1=box1[i1].lb()==POS_INFINITY ? box1[i1].lb() : -box1[i1].ub();
			double pt2=box2[i2].lb()==POS_INFINITY ? box2[i2].lb() : -box2[i2].ub();

			if (pt1<pt2)
				return pair<bool,int>(true,i1);
			else
				return pair<bool,int>(false,i2);
		}
	} else {
		if (box2[i2].is_unbounded())
			return pair<bool,int>(false,i2);
		else {
			if (box1[i1].diam()>box2[i2].diam())
				return pair<bool,int>(true,i1);
			else
				return pair<bool,int>(false,i2);
		}
	}
}
}

// TODO: merge this code with union_

SetNode* SetNode::inter(bool iset, const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		//cout << "this1 bef="; this->print(cout,nodebox,0);
		//cout << "otherbox=" << otherbox << " status=" << ((SetLeaf*) other)->status << endl;
		SetNode* this1=inter(iset, nodebox, otherbox, ((SetLeaf*) other)->status);
		//cout << "this1: "; this1->print(cout,nodebox,0);
		return this1;
	} else {
		// ******************************** balancing **************************************************

		pair<bool,int> ld=largest_diameter(nodebox,otherbox);

		if (ld.first) {

			SetBisect* bis;

			if (is_leaf()) {
				int var = ld.second;
				pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
				double pt=p.first[var].ub();
				assert(nodebox[var].interior_contains(pt));

				bis = new SetBisect(var, pt);
				bis->left  = new SetLeaf(((SetLeaf *) this)->status);
				bis->right = new SetLeaf(((SetLeaf *) this)->status);
				bis->left  = bis->left->inter(iset, p.first, other, otherbox);
				bis->right = bis->right->inter(iset, p.second, other, otherbox);
				delete this;
			}
			else {
				bis = (SetBisect*) this;
				bis->left  = bis->left->inter(iset,bis->left_box(nodebox), other, otherbox);
				bis->right = bis->right->inter(iset,bis->right_box(nodebox), other, otherbox);
			}

			bis->left->father = bis;
			bis->right->father = bis;
			return bis->try_merge();

		} else {
			SetBisect* bisect_node = (SetBisect*) other;
			SetNode* this2 = inter(iset, nodebox, bisect_node->left, bisect_node->left_box(otherbox));
			//cout << "this2: "; this2->print(cout,nodebox,0);
			// warning: cannot use this anymore (use this2 instead)
			SetNode* this3 = this2->inter(iset, nodebox, bisect_node->right, bisect_node->right_box(otherbox));
			//cout << "this3: "; this3->print(cout,nodebox,0);
			return this3;
		}
	}
}

SetNode* SetNode::union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		return union_(nodebox, otherbox, ((SetLeaf*) other)->status);
	} else {

		// ******************************** balancing **************************************************

		pair<bool,int> ld=largest_diameter(nodebox,otherbox);

		if (ld.first) {

			SetBisect* bis;

			if (is_leaf()) {
				int var=ld.second;
				pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
				double pt=p.first[var].ub();
				assert(nodebox[var].interior_contains(pt));

				bis = new SetBisect(var, pt);
				bis->left  = new SetLeaf(((SetLeaf *) this)->status);
				bis->right = new SetLeaf(((SetLeaf *) this)->status);
				bis->left  = bis->left->union_(p.first, other, otherbox);
				bis->right = bis->right->union_(p.second, other, otherbox);
				delete this;
			}
			else {
				bis = (SetBisect*) this;
				bis->left  = bis->left->union_(bis->left_box(nodebox), other, otherbox);
				bis->right = bis->right->union_(bis->right_box(nodebox), other, otherbox);
			}

			bis->left->father = bis;
			bis->right->father = bis;
			return bis->try_merge();

		} else {
			SetBisect* bisect_node = (SetBisect*) other;
			SetNode* this2 = this->union_(nodebox, bisect_node->left, bisect_node->left_box(otherbox));
			// warning: cannot use this anymore (use this2 instead)
			return this2->union_(nodebox, bisect_node->right, bisect_node->right_box(otherbox));
		}

		// *********************************************************************************************

//		SetBisect* bisect_node = (SetBisect*) other;
//		SetNode* this2 = this->union_(nodebox, bisect_node->left, bisect_node->left_box(otherbox));
//		return this2->union_(nodebox, bisect_node->right, bisect_node->right_box(otherbox));
	}
}

} // namespace ibex
