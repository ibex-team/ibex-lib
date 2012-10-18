//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#include "ibex_P_Expr.h"
#include "ibex_P_ExprPrinter.h"

extern void ibexerror (const std::string& msg);

using namespace std;

namespace ibex {
namespace parser {

void p_print(const ExprNode& e) {
	P_ExprPrinter().print(cout,e);
}

P_ExprPower::P_ExprPower(const ExprNode& expr, const ExprNode& expon) : ExprBinaryOp(expr,expon,expr.dim) {
	if (!expr.dim.is_scalar()) ibexerror("cannot raise a non-scalar value to some power");
	if (!expon.dim.is_scalar()) ibexerror("cannot raise a value to a non-scalar power");

}

void P_ExprPower::acceptVisitor(ExprVisitor& v) const {
	P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
	if (v2) v2->visit(*this);
	else v.visit(*this);
}

P_ExprIndex::P_ExprIndex(const ExprNode& expr, const ExprNode& index, bool style) : ExprBinaryOp(expr,index,expr.dim.index_dim()),
		matlab_style(style) {

}

void P_ExprIndex::acceptVisitor(ExprVisitor& v) const {
	P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
	if (v2) v2->visit(*this);
	else v.visit(*this);
}

ExprIter::ExprIter(const char* name) : ExprLeaf(Dim()), name(strdup(name)) {

}

ExprIter::~ExprIter() {
	free((char*) name);
}

void ExprIter::acceptVisitor(ExprVisitor& v) const {
	P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
	if (v2) v2->visit(*this);
	else v.visit(*this); // as a leaf. May happen, e.g., with ExprReset called by bin_size
	// (for calculating the size of the DAG x[y])
}

ExprInfinity::ExprInfinity() : ExprLeaf(Dim::scalar()) {

}

void ExprInfinity::acceptVisitor(ExprVisitor& v) const {
	P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
	if (v2) v2->visit(*this);
	else v.visit(*this); // as a leaf.
}

ExprConstantRef::ExprConstantRef(const Domain& d) : ExprLeaf(d.dim), value(d,true) {

}

void ExprConstantRef::acceptVisitor(ExprVisitor& v) const {
	P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
	if (v2) v2->visit(*this);
	else v.visit(*this); // as a leaf.
}

//ExprEntity::ExprEntity(const Entity& e, int line) : ExprNode(0,1,e.dim), entity(e), line(line) {
//
//}

} // end namespace parser
} // end namespace ibex
