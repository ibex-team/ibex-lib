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
	case LT  : os << "<";  break;
	case LEQ : os << "<="; break;
	case EQ  : os <<  "="; break;
	case GEQ : os << ">="; break;
	case GT  : os << ">";  break;
	}
	return os;
}

CmpOp operator!(CmpOp op) {
	assert(op!=EQ);
	switch(op) {
	case LT :  return GEQ;
	case LEQ : return GT;
	case GEQ : return LT;
	case GT :  return LEQ;
	default :  assert(false); return EQ;
	}
}

} // end namespace ibex




