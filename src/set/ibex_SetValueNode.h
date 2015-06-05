//============================================================================
//                                  I B E X                                   
// File        : ibex_SetValueNode.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 28, 2015
//============================================================================

#ifndef __IBEX_SET_VALUE_NODE_H__
#define __IBEX_SET_VALUE_NODE_H__


#include "ibex_SetNode.h"


namespace ibex {

class SetValueNode : public SetNode {
public:

	SetValueNode(int var, Interval value);

	virtual ~SetValueNode();

	int var;
	Interval value;
};

} // namespace ibex

#endif // __IBEX_SET_VALUE_NODE_H__
