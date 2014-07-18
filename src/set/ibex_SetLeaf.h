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

	SetLeaf(BoolInterval status);

	virtual BoolInterval status() const;

	virtual bool is_leaf() const;

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps);

	virtual SetNode* contract_rec(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps);

	virtual void to_vibes(color_code color_func, const IntervalVector& nodebox) const;

	BoolInterval _status;

};

} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
