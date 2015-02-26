//============================================================================
//                                  I B E X                                   
// File        : ibex_SetLeaf.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"

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

SetLeaf::SetLeaf(NodeType status) : SetNode(status) {
	if (status>UNK && status!=IN_TMP) {
		ibex_error("cannot set multiple status to SetLeaf");
	}
}

SetLeaf::SetLeaf(NodeType status,SetNode* father): SetNode(status,father) {

} 

SetLeaf::~SetLeaf() {

}

bool SetLeaf::is_leaf() const {
	return true;
}

 SetNode * SetLeaf::copy() const {
	return new SetLeaf(this->status); 
}

void SetLeaf::inter(NodeType x_status) {
	status = inte(status,x_status); 
}

void SetLeaf::_union(NodeType x) {
	status = uni(status,x);
}

void SetLeaf::oper(SetNode * node,bool op) {
	if(node->is_leaf()) // if node is leaf, apply its value to leaves of this
	{
		if(op)
			status = inte(status,node->status);
		else
			status = uni(status,node->status);
	}
	else{ // if this is leaf and node isn't
		SetBisect * bfather = (SetBisect*) father;
		if(bfather->left == this)
		{
			bfather->left = node->copy(); // copy the structure of node
			bfather->left->father = father;
			if(op && status != IN)
				bfather->left->inter(status);  // apply status to leaves
			else if(!op && status!= OUT)
				bfather->left->_union(status);
		}
		else
		{
			bfather->right = node->copy();
			bfather->right->father = father;
			if(op && status != IN)
				bfather->right->inter(status);
			else if(!op && status!= OUT)
				bfather->right->_union(status);
		}
		delete this; // delete this as it is now a setBisect and no more a leaf

	}
}

void SetLeaf::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType val, bool op,double eps) {
    if(box.is_subset(subbox))
	{
		if(op && val != IN)
			status = inte(status,val);
		else if(!op && val!=OUT)
			status = uni(status,val);
	}
	else if(box.max_diam()>eps)
	{
		SetBisect * bfather = (SetBisect*) father;
		int var = box.extr_diam_index(false); // indice of the maximal diameter
		double pt = box[var].mid();
		SetLeaf * lnode = new SetLeaf(status,NULL); // create future right node
		SetLeaf * rnode = new SetLeaf(status,NULL); // create future left node
        if(bfather->left == this) // this is father->left
		{
			bfather->left = new SetBisect(var,pt,lnode,rnode,father); // create SetBisect to replace leaf
			lnode->father = bfather->left;  // set fathers
			rnode->father = bfather->left;
            bfather->left->operator_ir(box,subbox,val,op,eps);
		}
		else // this is father->right
		{
			bfather->right = new SetBisect(var,pt,lnode,rnode,father);
			lnode->father = bfather->right;
			rnode->father = bfather->right;
            bfather->right->operator_ir(box,subbox,val,op,eps);
		}
		delete this; // delete former father->left or father->right
	}
	else // if leaf reach minimum precision, compute partial intersection or union
	{
		if(op)
			status = inte_in(status,val);
		else
			status = uni_in(status,val);
    }
}

// Uncomment this function and comment the one above to use fakeBranch method
/*void SetLeaf::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType val, bool op,bool is_left,double eps) {
	if(box.is_subset(subbox))
	{
		if(op)
			status = inte(status,val);
		else
			status = uni(status,val);
	}
	else if(box.overlaps(subbox))
	{
		if(box.max_diam()>eps)
		{
			SetBisect * bfather = (SetBisect*) father;
			int var = box.extr_diam_index(false); // indice of the maximal diameter
			double pt = box[var].mid();
			if(bfather->left == this)
			{
				bfather->left = new SetBisect(var,pt,NULL,NULL,father);
				bfather->left->status = status; // create new set bissect with nodetype IN OUT or UNK, that it will transmit this value to its left and right
				bfather->left->operator_ir(box,subbox,val,op,eps);
			}
			else
			{
				bfather->right = new SetBisect(var,pt,NULL,NULL,father);
				bfather->right->status = status; // create new set bissect with nodetype IN OUT or UNK, that it will transmit this value to its left and right
				bfather->right->operator_ir(box,subbox,val,op,eps);
			}
			delete this;
		}
		else
			if(op)
				status = inte_in(status,val);
			else
				status = uni_in(status,val);
	}
	else
		return;

}*/

void SetLeaf::operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType valin,NodeType valout, bool op,double eps) {
    if(box.is_subset(subbox))
	{
		if(op && valin!=IN)
			status = inte(status,valin);
		else if(!op && valin!=OUT)
			status = uni(status,valin);
	}
	else if(box.max_diam()>eps)
	{
		SetBisect * bfather = (SetBisect*) father;
		int var = box.extr_diam_index(false); // indice of the maximal diameter
		double pt = box[var].mid();
		SetLeaf * lnode = new SetLeaf(status,NULL); // create future right node
		SetLeaf * rnode = new SetLeaf(status,NULL); // create future left node
        if(bfather->left == this) // this is father->left
		{
			bfather->left = new SetBisect(var,pt,lnode,rnode,father); // create SetBisect to replace leaf
			lnode->father = bfather->left;  // set fathers
			rnode->father = bfather->left;
            bfather->left->operator_ir(box,subbox,valin,valout,op,eps);
		}
		else // this is father->right
		{
			bfather->right = new SetBisect(var,pt,lnode,rnode,father);
			lnode->father = bfather->right;
			rnode->father = bfather->right;
            bfather->right->operator_ir(box,subbox,valin,valout,op,eps);
		}
		delete this; // delete former father->left or father->right
	}
	else // if leaf reach minimum precision, compute partial intersection or union
	{
		if(op)
			status = inte_in(status,UNK);
		else
			status = uni_in(status,UNK);
    }
}

SetNode * SetLeaf::fakeLeaf(const IntervalVector& box,const IntervalVector& subbox,NodeType val,bool op, double eps) {
	assert(!this->is_leaf());
	return NULL;
}

void SetLeaf::cleave(const IntervalVector& box, Sep& sep,double eps) {

	IntervalVector box1(box);
	IntervalVector box2(box);
	sep.separate(box1,box2);
	if(box1.is_disjoint(box2)) // if box are disjoint, contract on box1 and box2
	{
		if(!box1.is_empty())
			this->operator_ir(box,box1,IN,true,eps);
		if(!box2.is_empty())
			this->operator_ir(box,box2,OUT,true,eps);
		
	}
	else if(box.max_diam()>eps)
	{
		SetBisect * bfather = (SetBisect*) father;
		int var = box.extr_diam_index(false); // indice of the maximal diameter
		double pt = box[var].mid();
		SetLeaf * lnode = new SetLeaf(status,NULL); // create future right node
		SetLeaf * rnode = new SetLeaf(status,NULL); // create future left node
        if(bfather->left == this) // this is father->left
		{
			bfather->left = new SetBisect(var,pt,lnode,rnode,father); // create SetBisect to replace leaf
			lnode->father = bfather->left;  // set fathers
			rnode->father = bfather->left;
            bfather->left->cleave(box,sep,eps);
		}
		else // this is father->right
		{
			bfather->right = new SetBisect(var,pt,lnode,rnode,father);
			lnode->father = bfather->right;
			rnode->father = bfather->right;
            bfather->right->cleave(box,sep,eps);
		}
		delete this; // delete former father->left or father->right
	}
	else
		status = inte(status,UNK);

}

void SetLeaf::gather(bool go_up) {
	return; // nothing to do
}

void SetLeaf::cutDeadBranch() {
	return; // nothing to do
}
void SetLeaf::checkFat() {
	if (father == NULL)
		cout<<"issue, invalid father"<<endl;
}


void SetLeaf::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	func(nodebox, status==IN? YES : (status==OUT? NO : MAYBE));
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << to_string(status) << endl;
}

void SetLeaf::setFathers() {
	return; // nothing to do here
}
} // namespace ibex
