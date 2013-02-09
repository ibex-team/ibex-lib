//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserNumConstraint.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_P_NumConstraint.h"

using namespace std;

namespace ibex {

namespace parser {

P_OneConstraint::P_OneConstraint(const ExprNode& left, CmpOp op, const ExprNode& right) :
		expr(right.is_zero()? left : left-right), op(op) {

	if (right.is_zero()) {
		delete &right;
	}
}

P_OneConstraint::~P_OneConstraint() {
	cleanup(expr,false); // will not delete symbols
}

P_ConstraintList::P_ConstraintList(vector<P_NumConstraint*>& ctrs) : ctrs(ctrs) {

}

P_ConstraintList::~P_ConstraintList() {
	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		delete *it;
	}
	delete &ctrs;
}

P_ConstraintLoop::P_ConstraintLoop(const char* iter, const ExprNode& first_value, const ExprNode& last_value, vector<P_NumConstraint*>& ctrs) :
		 iter(strdup(iter)), first_value(first_value), last_value(last_value), ctrs(ctrs) {

}

P_ConstraintLoop::~P_ConstraintLoop() {
	cleanup(first_value,true); // false or true (there is no symbols)
	cleanup(last_value,true);
}

} // end namespace parser
} // end namespace ibex
