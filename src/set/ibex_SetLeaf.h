//============================================================================
//                                  I B E X                                   
// File        : ibex_SetLeaf.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_LEAF_H__
#define __IBEX_SET_LEAF_H__

#include "ibex_SetNode.h"

namespace ibex {

class SetLeaf : public SetNode {

public:

	SetLeaf(SetType status);

	virtual ~SetLeaf();

	virtual bool is_leaf() const;

	//virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps, Mode mode);

	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps);

	virtual SetNode* sync_rec(const IntervalVector& nodebox, Bracket& br, double eps);

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps);

	virtual SetNode* inter_rec(const IntervalVector& nodebox, Bracket& br, double eps);

	virtual void to_vibes(color_code color_func, const IntervalVector& nodebox) const;

	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	virtual void set_in_tmp();

	virtual void unset_in_tmp();

};

} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
