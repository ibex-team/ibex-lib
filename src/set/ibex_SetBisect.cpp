//============================================================================
//                                  I B E X                                   
// File        : ibex_SetBisect.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetBisect.h"
#include "ibex_SetLeaf.h"
#include <stack>
#include <utility>

using namespace std;

// =========== shortcuts ==================
#define IN         __IBEX_IN__
#define OUT        __IBEX_OUT__
#define UNK        __IBEX_UNK__
#define UNK_IN     __IBEX_UNK_IN__
#define UNK_OUT    __IBEX_UNK_OUT__
#define UNK_IN_OUT __IBEX_UNK_IN_OUT__
#define IN_TMP     __IBEX_IN_TMP__
// ========================================

namespace ibex {

SetBisect::SetBisect(int var, double pt, SetNode* left, SetNode* right) : SetNode(UNK_IN_OUT,NULL), var(var), pt(pt), left(left), right(right) {
	
}

SetBisect::SetBisect(int var, double pt, SetNode* left, SetNode* right,SetNode* father) : SetNode(UNK_IN_OUT,father), var(var), pt(pt), left(left), right(right) {
	
}

SetBisect::SetBisect(int var, double pt) : SetNode(UNK_IN_OUT), var(var), pt(pt), left(NULL), right(NULL) {

}

SetBisect::~SetBisect() {
	delete left;
	delete right;
}

bool SetBisect::is_leaf() const {
    return false;
}

SetNode * SetBisect::copy() const {
	SetBisect * n = new SetBisect(this->var,this->pt,this->left->copy(),this->right->copy());
	n->left->father = n;
	n->right->father = n;
	return n;
}


void SetBisect::inter(NodeType x) {

	left->inter(x); // apply inter to left and right until reach the leaves
	right->inter(x);
}

void SetBisect::_union(NodeType x) {
	left->_union(x);
	right->_union(x);
}

void SetBisect::oper(SetNode * node,bool op) {
	if(node->is_leaf()) // apply status to leaves of the subtree which have this as root
	{
		if(op && node->status != IN) 
		{
			left->inter(node->status);
			right->inter(node->status);
		}
		else if(!op && node->status!= OUT)
		{
			left->_union(node->status);
			right->_union(node->status);
		}
	}
	else
	{
		SetBisect * other = (SetBisect*) node;  // node is assume to be either a leaf or a bisect, able to cast node in bisect as it ain't a leaf
		left->oper(other->left,op);
		right->oper(other->right,op);
	}
}

void SetBisect::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType val,bool op, double eps)
{

	if(box.is_subset(subbox)) // compute intersection of leaves of this root with val
	{
		if(op && val != IN)
			this->inter(val);
		else if(!op && val != OUT)
			this->_union(val);
	}
	else
	{
		if(this->left_box(box).overlaps(subbox)) //  need to apply function on left only if leftbox ovelaps subbox
			left->operator_ir(this->left_box(box),subbox,val,op,eps);
		if(this->right_box(box).overlaps(subbox)) //  need to apply function on right only if rightbox overlaps subbox
			right->operator_ir(this->right_box(box),subbox,val,op,eps);
	}
}

void SetBisect::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType valin,NodeType valout,bool op, double eps)
{

	if(box.is_subset(subbox)) // apply valin on the leaves of this root
	{
		if(op && valin != IN)
			this->inter(valin);
		else if(!op && valin != OUT)
			this->_union(valin);
	}
	else if(box.overlaps(subbox))
	{
		if(this->left_box(box).overlaps(subbox))
			left->operator_ir(this->left_box(box),subbox,valin,valout,op,eps);
		else {								// leftbox is disjoint of subbox, apply valout on the leaves of this->left root
			if(op && valout != IN)
				left->inter(valout);
			else if (!op && valout != OUT)
				left->_union(valout);}
		if(this->right_box(box).overlaps(subbox))
			right->operator_ir(this->right_box(box),subbox,valin,valout,op,eps);
		else{								// rightbox is disjoint of subbox, apply valout on the leaves of this->right root
			if(op && valout != IN)
				right->inter(valout);
			else if (!op && valout != OUT)
				right->_union(valout);}
	}
	else{   								// this is disjoint of subbox, apply valout on the leaves of this root
		if(op && valout != IN)
			this->inter(valout);
		else if (!op && valout != OUT)
			this->_union(valout);}
}

// Uncomment this function and comment the one above to use fakeBranch method
/*void SetBisect::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType val,bool op, double eps)
{

	if(left == NULL)
	{
		assert(right == NULL);
		left = this->fakeLeaf(left_box(box),subbox,val,op,eps);
		left->father = this;
		right = this->fakeLeaf(right_box(box),subbox,val,op,eps);
		right->father = this;
		status = UNK_IN_OUT;
	}
	else
	{
		if(box.is_subset(subbox))
		{
			if(op)
				this->inter(val);
			else
				this->_union(val);
		}
		else
		{
			if(this->left_box(box).overlaps(subbox))
				left->operator_ir(this->left_box(box),subbox,val,op,eps);
			else
				return;
			if(this->right_box(box).overlaps(subbox))
				right->operator_ir(this->right_box(box),subbox,val,op,eps);
			else
				return;
		}
	}
}*/

SetNode * SetBisect::fakeLeaf(const IntervalVector& box,const IntervalVector& subbox,NodeType val,bool op, double eps) {
    //assert(left == NULL && right == NULL);
	if(box.is_subset(subbox))
		if(op)
			return new SetLeaf(inte(status,val));
		else
			return new SetLeaf(uni(status,val));
	else if(box.overlaps(subbox))
	{
		if(box.max_diam()<=eps)
			return new SetLeaf(inte_in(status,val));
		else
		{
			int var = box.extr_diam_index(false); 
			double pt = box[var].mid();
			SetBisect * node = new SetBisect(var,pt,NULL,NULL);
			node->status  = status;
			node->left = node->fakeLeaf(node->left_box(box),subbox,val,op,eps);
			node->right = node->fakeLeaf(node->right_box(box),subbox,val,op,eps);
			node->left->father = node;
			node->right->father = node;
			node->status = UNK_IN_OUT;
			return node;
		}
	}
	else
		return new SetLeaf(status);
}
void SetBisect::cleave(const IntervalVector& box, Sep& sep, double eps) {

	IntervalVector box1(box);
	IntervalVector box2(box);

	sep.separate(box1,box2);
	if(box1.is_disjoint(box2)) // in and out are disjoint, can contract on box1 and box2
	{
		if(!box1.is_empty())
			this->operator_ir(box,box1,IN,true,eps);
		if(!box2.is_empty())
			this->operator_ir(box,box2,OUT,true,eps);
	}
	else // continu until box1 and box2 are disjoint
	{
		left->cleave(left_box(box),sep,eps);
		right->cleave(right_box(box),sep,eps);
	}
}
void SetBisect::gather(bool go_up) {
		if(((left->is_leaf()&&right->is_leaf()) || go_up)&&left->status==right->status) // leaves are reached, or climbing up the tree toward the root
		{
			status = left->status;  // this bisect get a leaf status IN OUT or UNK
			if(father != NULL) // check if father is not the root of SetInterval
				father->gather(true);
		}
		else if(!go_up) // go down the tree to reach the leaves
		{
			left->gather(false);
			right->gather(false);
		}
}

void SetBisect::cutDeadBranch() {
	if(left->status ==right->status && left->status < 3) // if status of left and right are equal and has got a leaf status, meaning all leaves have the same status
	{
		SetBisect* bfather = (SetBisect*) father;
		if(bfather->left == this)
			bfather->left = new SetLeaf(status,father); // this is now a leaf
		else
			bfather->right = new SetLeaf(status,father); // this is now a leaf
		delete this;  // delete former this which was a bisect
	}
	else // go down the tree until reach leaves or bisect that can be set a leaf
	{
		left->cutDeadBranch();
		right->cutDeadBranch();
	}
}


void SetBisect::checkFat() {
	if(father == NULL)
		cout<<"issue, invalid father"<<endl;
	left->checkFat();
	right->checkFat();
}
void SetBisect::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	left->visit_leaves(func, left_box(nodebox));
	right->visit_leaves(func, right_box(nodebox));
}

void SetBisect::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os << "* " << nodebox << endl;
	left->print(os, left_box(nodebox), shift+2);
	right->print(os, right_box(nodebox), shift+2);
}

void SetBisect::setFathers() {
	left->father = this;
	right->father = this;
	left->setFathers();
	right->setFathers();
}


IntervalVector SetBisect::left_box(const IntervalVector& nodebox) const {
	IntervalVector leftbox(nodebox);
	assert (nodebox[var].contains(pt));
	leftbox[var] =Interval(nodebox[var].lb(),pt);
	return leftbox;
}

IntervalVector SetBisect::right_box(const IntervalVector& nodebox) const {
	IntervalVector rightbox(nodebox);
	assert (nodebox[var].contains(pt));
	rightbox[var]=Interval(pt,nodebox[var].ub());
	return rightbox;
}


} // namespace ibex
