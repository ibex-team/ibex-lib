//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserNumConstraint.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_ParserNumConstraint.h"

namespace ibex {

namespace parser {

P_OneConstraint::P_OneConstraint(const ExprNode& expr, NumConstraint::CompOp op) : expr(expr), op(op) {

}

P_ConstraintLoop::P_ConstraintLoop(const char* iter, int first_value, int last_value, const P_NumConstraint& ctr) :
		 iter(strdup(iter)), first_value(first_value), last_value(last_value), ctr(ctr) {

}

} // end namespace parser
} // end namespace ibex
