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
#include "ibex_ExprTools.h"

namespace ibex {

namespace parser {

P_OneConstraint::P_OneConstraint(const ExprNode& left, NumConstraint::CompOp op, const ExprNode& right) :
		expr(right.is_zero()? left : left-right), op(op) {

}

P_OneConstraint::~P_OneConstraint() {
	cleanup(expr,false); // do not delete symbols
}

P_ConstraintLoop::P_ConstraintLoop(const char* iter, int first_value, int last_value, vector<P_NumConstraint*>& ctrs) :
		 iter(strdup(iter)), first_value(first_value), last_value(last_value), ctrs(ctrs) {

}

P_ConstraintLoop::~P_ConstraintLoop() {
	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		delete *it;
	}
}

} // end namespace parser
} // end namespace ibex
