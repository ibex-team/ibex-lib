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
#include "ibex_Bracket.h"

namespace ibex {

class Set {
public:

	Set(const IntervalVector& bounding_box, double eps, SetType status=UNK);

	~Set();

	Set& operator&=(const Set& set);

	bool is_empty() const;

	void contract(Bracket& br);

	void sync(Bracket& br);

	void to_vibes(SetNode::color_code color_func) const;

protected:
	friend std::ostream& operator<<(std::ostream& os, const Set& set);

	SetNode* root; // NULL means no existing set (warning: different from empty set!)

	double eps;

	IntervalVector bounding_box; // not sure it is really necessary
};

std::ostream& operator<<(std::ostream& os, const Set& set);

} // namespace ibex

#endif // __IBEX_SET_H__
