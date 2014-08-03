//============================================================================
//                                  I B E X                                   
// File        : ibex_SetBisect.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_BISECT_H__
#define __IBEX_SET_BISECT_H__

#include "ibex_SetNode.h"

namespace ibex {

class SetBisect : public SetNode {

public:

	SetBisect(int var, double pt, SetNode* left, SetNode* right);

	virtual ~SetBisect();

	virtual bool is_leaf() const;

	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps);

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps);

	virtual SetNode* sync_rec(const IntervalVector& nodebox, Bracket& br, double eps);

	virtual SetNode* inter_rec(const IntervalVector& nodebox, Bracket& br, double eps);

	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const;

	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	virtual void set_in_tmp();

	virtual void unset_in_tmp();

	IntervalVector left_box(const IntervalVector& nodebox) const;

	IntervalVector right_box(const IntervalVector& nodebox) const;

protected:
	friend class SetNode;

	SetNode* try_merge();

	int var;
	double pt;
	SetNode* left;
	SetNode* right;
};

} // namespace ibex

#endif // __IBEX_SET_BISECT_H__
