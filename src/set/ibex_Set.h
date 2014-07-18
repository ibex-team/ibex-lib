//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_H__
#define __IBEX_SET_H__

#include "ibex_SetNode.h"

namespace ibex {

class Set {
public:

	Set(const IntervalVector& bounding_box);

	virtual ~Set();

	Set& operator&=(const Set& set);

	void contract(Ctc& ctc, BoolInterval c_status, double eps);

	void inter(const Set& s2, double eps);

	void to_vibes(SetNode::color_code color_func) const;

protected:
	SetNode* root;

	IntervalVector bounding_box; // not sure it is really necessary
};

} // namespace ibex

#endif // __IBEX_SET_H__
