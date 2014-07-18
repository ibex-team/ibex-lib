//============================================================================
//                                  I B E X                                   
// File        : ibex_SetNode.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_NODE_H__
#define __IBEX_SET_NODE_H__

#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"
#include "ibex_BoolInterval.h"

namespace ibex {

class SetNode {
public:
	typedef const char* (*color_code) (BoolInterval);

	virtual ~SetNode();

	virtual BoolInterval status() const=0;

	virtual bool is_leaf() const=0;

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps)=0;

	virtual SetNode* contract_rec(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps)=0;

	virtual void to_vibes(color_code color_func, const IntervalVector& nodebox) const=0;

	SetNode* inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	SetNode* contract(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps);
};

SetNode* diff(const IntervalVector& x, const IntervalVector& y, BoolInterval x_status, BoolInterval y_status, double eps);

SetNode* contract_set(const IntervalVector& box1, Ctc& c, BoolInterval c_status, double eps);

} // namespace ibex

#endif // __IBEX_SET_NODE_H__
