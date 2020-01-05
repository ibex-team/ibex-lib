//============================================================================
//                                  I B E X                                   
// File        : ibex_P_NumConstraint.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : May 22, 2019
//============================================================================

#include "ibex_P_NumConstraint.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

namespace parser {

P_OneConstraint::P_OneConstraint(const P_ExprNode* left, CmpOp op, const P_ExprNode* right) :
		//expr((right->lab->is_const() && right->lab->domain().is_zero())? *left : (const P_ExprNode&) *((*left)-(*right))), op(op) {
				expr(right ? *((*left)-(*right)) : *left), op(op) {

//	cout << "[parser] add ctr: " << expr << op << "0" << endl;
//	if (right->lab->is_const() && right->lab->domain().is_zero()) {
//		delete right;
//	}
}

P_OneConstraint::~P_OneConstraint() {
	delete &expr;
}

P_TmpSymbolDecl::P_TmpSymbolDecl(const char* symbol, const P_ExprNode* expr) : symbol(strdup(symbol)), expr(*expr) {

}

void P_TmpSymbolDecl::print(std::ostream& os) const {
	os << symbol << ":=" << expr << endl;
}

P_TmpSymbolDecl::~P_TmpSymbolDecl() {
	free((char*) symbol);
	delete &expr;
}

void P_OneConstraint::print(ostream& os) const {
	os << expr << op << '0';
}

P_ConstraintList::P_ConstraintList(vector<P_NumConstraint*>* _ctrs) : ctrs(*_ctrs) {

}

P_ConstraintList::~P_ConstraintList() {
	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); ++it) {
		delete *it;
	}
	delete &ctrs;
}

void P_ConstraintList::print(ostream& os) const {
	for (vector<P_NumConstraint*>::const_iterator it=ctrs.begin(); it!=ctrs.end(); ++it) {
		os << **it << endl;
	}
}

P_ConstraintLoop::P_ConstraintLoop(const char* iter, const P_ExprNode* first_value, const P_ExprNode* last_value, vector<P_NumConstraint*>* ctrs) :
		 iter(strdup(iter)), first_value(*first_value), last_value(*last_value), ctrs(ctrs) {

}

P_ConstraintLoop::~P_ConstraintLoop() {
	free((char*) iter);
	delete &first_value;
	delete &last_value;
}

void P_ConstraintLoop::print(ostream& os) const {
	os << "for " << iter << "=" << first_value << ":" << last_value << endl;
	os << ctrs;
	os << "end" << endl;
}

P_ThickEquality::P_ThickEquality(const P_ExprNode* expr, const Interval& d) : expr(*expr), d(d) {

}

P_ThickEquality::~P_ThickEquality() {
	delete &expr;
}

void P_ThickEquality::print(ostream& os) const {
	os << expr << " in " << d;
}

} // end namespace parser

} // end namespace ibex
