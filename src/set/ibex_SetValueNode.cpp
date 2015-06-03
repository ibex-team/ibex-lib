//============================================================================
//                                  I B E X                                   
// File        : ibex_SetValueNode.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 28, 2015
//============================================================================

#include "ibex_SetValueNode.h"

namespace ibex {

SetValueNode::SetValueNode(int var, Interval value) : var(var), value(value) {

}

SetValueNode::~SetValueNode() {
	// TODO Auto-generated destructor stub
}

} // namespace ibex
