//============================================================================
//                                  I B E X                                   
// File        : ibex_CmpOp.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#include "ibex_CmpOp.h"
#include <cassert>

namespace ibex {

std::ostream& operator<<(std::ostream& os, const CmpOp& op) {
	switch (op) {
	case IBEX_LT  : os << "<";  break;
	case IBEX_LEQ : os << "<="; break;
	case IBEX_EQ  : os <<  "="; break;
	case IBEX_GEQ : os << ">="; break;
	case IBEX_GT  : os << ">";  break;
	}
	return os;
}

CmpOp operator!(CmpOp op) {
	assert(op!=IBEX_EQ);
	switch(op) {
	case IBEX_LT :  return IBEX_GEQ;
	case IBEX_LEQ : return IBEX_GT;
	case IBEX_GEQ : return IBEX_LT;
	case IBEX_GT :  return IBEX_LEQ;
	default :  assert(false); return IBEX_EQ;
	}
}

} // end namespace ibex




