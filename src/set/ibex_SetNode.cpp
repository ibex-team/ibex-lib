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
 #include <unistd.h>

using namespace std;

namespace ibex {

SetType operator|(SetType x, SetType y) {
	switch (x) {
	case IN : {
		switch(y) {
		case IN :        return IN;
		case OUT :
		case UNK_OUT:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_IN;
		}
	}
	break;
	case OUT : {
		switch(y) {
		case OUT :       return OUT;
		case IN :
		case UNK_IN:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_OUT;
		}
	}
	break;
	case UNK : {
		switch(y) {
		case IN :
		case UNK_IN:  return UNK_IN;
		case OUT :
		case UNK_OUT: return UNK_OUT;
		case UNK:     return UNK;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	default :
		return UNK_IN_OUT;
	}
}

bool certainly_contains_in(SetType x) {
	return x==IN || x==UNK_IN || x==UNK_IN_OUT;
}

bool certainly_contains_out(SetType x) {
	return x==OUT || x==UNK_OUT || x==UNK_IN_OUT;
}

bool possibly_contains_in(SetType x) {
	return x!=OUT;
}

bool possibly_contains_out(SetType x) {
	return x!=IN;
}

SetNode* diff(const IntervalVector& x, const IntervalVector& y, SetType x_status, SetType y_status, double eps) {

	if (y.is_empty()) {
		return new SetLeaf(x_status);
	}

	const int nn=x.size();

	typedef struct {
		int var;
		double pt;
		bool y_left; // is the part that belongs to y on the left (=true) or right (=false) side?
	} bisection;

	bisection *tmp = new bisection[2*nn]; // in the worst case, there is 2n bisections
	Interval c1, c2;
	int b=0;

	for (int var=0; var<nn; var++) {

		x[var].diff(y[var],c1,c2);
		//cout << "x[" << var << "]=" << x[var] << " y[" << var << "]=" << y[var] << " c1=" << c1 << " c2=" << c2 << endl;

		if (c1.is_empty()) continue;

		// we discard the y-part if it is not significant
		// but this is safe only if the status of y is UNK.
		if (y_status==UNK && c1.diam()<eps) continue;

		// when the set difference is close to x itself
		// we return x. But this is safe only if the
		// status of x is UNK.
		if (x_status==UNK && x[var].delta(c1)<eps) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		if (c1.lb()==x[var].lb()) {
			tmp[b].pt = c1.ub();
			tmp[b].y_left = false;
		} else {
			tmp[b].pt = c1.lb();
			tmp[b].y_left = true;
		}
		b++;

		if (c2.is_empty()) continue;
		if (y_status==UNK && c2.diam()<eps)  continue;
		if (x_status==UNK && x[var].delta(c2)<eps) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		tmp[b].pt = c2.lb();
		tmp[b].y_left = true;
		b++;
	}

	SetNode* root;

	if (b==0) {
		root = new SetLeaf(y_status);
	} else {
		SetNode* bisect=new SetLeaf(y_status);

		for (int i=b-1; i>=0; i--) {
			if (tmp[i].y_left) {
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(x_status));
			} else {
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(x_status), bisect);
			}
		}
		root = bisect;
	}
	delete[] tmp;

	return root;
}

// result of a contraction represented as a SetNode
SetNode* contract_set(const IntervalVector& box1, Ctc& c, SetType x_status, SetType y_status, double eps) {
	if (x_status==y_status) return new SetLeaf(x_status);

	SetNode* root2;
	IntervalVector box2(box1);

	try {
		c.contract(box2);
		root2 = diff(box1, box2, x_status, y_status, eps);
	} catch(EmptyBoxException&) {
		root2 = new SetLeaf(x_status);
	}
	//cout << "contract set:" << endl; root2->print(cout,box1,0);

	return root2;
}

char to_string(const SetType& status) {
	switch(status) {
	case IN : return 'Y'; break;
	case OUT : return 'N'; break;
	default : return '?'; break;
	}
}

const char* color(SetType status) {
	switch (status) {
	case IN: return "g";
	case OUT:  return "r";
	default : return "b";
	}
}

SetNode::SetNode(SetType status) : status(status) {

}

SetNode::~SetNode() {

}

SetNode* SetNode::sync(const IntervalVector& nodebox, Bracket& br, double eps) {
	// perform contraction
	//cout << "=== contract with ctc_in  =======" << endl;

	// we skip other UNK-box if this node is not a leaf. This makes no difference
	// if we are in SYNC mode but if we are in INTER mode, this prevents from
	// this node to be "absorbed" by a temporary UNK box resulting from contraction.
	SetNode* this2 = this->sync(nodebox, contract_set(nodebox, br.ctc_in, IN, UNK, eps), nodebox, eps, !is_leaf());

	//cout << " ctc_in gives: "; this2->print(cout,nodebox,0);
	//cout << endl;

	//cout << "=== contract with ctc_out =======" << endl;
	SetNode* this3 = this2->sync(nodebox, contract_set(nodebox, br.ctc_out, OUT, UNK, eps), nodebox, eps, !this2->is_leaf());
	//cout << " ctc_out gives: "; this3->print(cout,nodebox,0);
	//cout << endl;

	SetNode* this4 = this3->sync_rec(nodebox, br, eps);

	return this4;
}

SetNode* SetNode::sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps, bool skip_other_maybe) {

	if (other->is_leaf()) {
		//if (other->status<UNK) // || !skip_other_maybe)
			return sync(nodebox, otherbox, other->status, eps);
		//else
		//	return this;
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = sync(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps, skip_other_maybe);
		// warning: cannot use this anymore (use this2 instead)
		return this2->sync(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps, skip_other_maybe);
	}
}

SetNode* SetNode::inter(const IntervalVector& nodebox, Bracket& br, double eps) {
	// we skip other UNK-box if this node is not a leaf. This makes no difference
	// if we are in SYNC mode but if we are in INTER mode, this prevents from
	// this node to be "absorbed" by a temporary UNK box resulting from contraction.
	SetNode* this2 = this->inter(nodebox, contract_set(nodebox, br.ctc_in, IN, UNK, eps), nodebox, eps);
	//cout << " ctc_in gives: "; this2->print(cout,nodebox,0);
	SetNode* this3 = this2->inter(nodebox, contract_set(nodebox, br.ctc_out, OUT, UNK, eps), nodebox, eps);

	SetNode* this4 = this3->inter_rec(nodebox, br, eps);

	return this4;
}


SetNode* SetNode::inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (other->is_leaf()) {
		// we consider IN_TMP to be "UNK" until the end. Otherwise, there
		// would be problems when other->status==UNK. Indeed, if we set the status
		// of this node to UNK, we lose information ("other" may be an intermediate
		// node in the process, although it is a SetLeaf, and some sub-nodes of other
		// could be IN. But once a node is UNK it cannot be set to IN anymore...).
		// If we impose this node to be a leaf, it does not fix the problem (this node may also be an
		// intermediate node in the process: consider a plain box (a leaf) to be
		// contracted).
			return inter(nodebox, otherbox, other->status, eps);
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = inter(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
		// warning: cannot use this anymore (use this2 instead)
		return this2->inter(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}
} // namespace ibex
