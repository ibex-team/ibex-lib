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

	SetBisect(int var, double pt, Set* left, Set* right);

	virtual ~SetBisect();

	virtual BoolInterval status() const;

	virtual bool is_leaf() const;

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps);

	virtual SetNode* contract_rec(const IntervalVector& nodebox, Ctc& c, BoolInterval c_status, double eps);

	IntervalVector left_box(const IntervalVector& nodebox) const;

	IntervalVector right_box(const IntervalVector& nodebox) const;

	virtual void to_vibes(color_code color_func, const IntervalVector& nodebox) const;

protected:

	SetNode* try_merge() const;

	int var;
	double pt;
	SetNode* left;
	SetNode* right;
};

} // namespace ibex

#endif // __IBEX_SET_BISECT_H__
