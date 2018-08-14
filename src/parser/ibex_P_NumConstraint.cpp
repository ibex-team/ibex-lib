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

P_OneConstraint::P_OneConstraint(const P_ExprNode* left, CmpOp op, const P_ExprNode* right) :
		//expr((right->lab->is_const() && right->lab->domain().is_zero())? *left : (const P_ExprNode&) *((*left)-(*right))), op(op) {
				expr(*((*left)-(*right))), op(op) {

//	cout << "[parser] add ctr: " << expr << op << "0" << endl;
//	if (right->lab->is_const() && right->lab->domain().is_zero()) {
//		delete right;
//	}
}

P_OneConstraint::~P_OneConstraint() {
	delete &expr;
}

P_ConstraintList::P_ConstraintList(vector<P_NumConstraint*>* _ctrs) : ctrs(*_ctrs) {

}

P_ConstraintList::~P_ConstraintList() {
	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		delete *it;
	}
	delete &ctrs;
}

P_ConstraintLoop::P_ConstraintLoop(const char* iter, const P_ExprNode* first_value, const P_ExprNode* last_value, vector<P_NumConstraint*>* ctrs) :
		 iter(strdup(iter)), first_value(*first_value), last_value(*last_value), ctrs(ctrs) {

}

P_ConstraintLoop::~P_ConstraintLoop() {
	delete &first_value;
	delete &last_value;
}

P_ThickEquality::P_ThickEquality(const P_ExprNode* expr, const Interval& d) : expr(*expr), d(d) {

}

P_ThickEquality::~P_ThickEquality() {
	delete &expr;
}


} // end namespace parser
} // end namespace ibex
