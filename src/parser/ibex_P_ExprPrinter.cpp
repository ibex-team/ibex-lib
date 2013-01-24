//============================================================================
//                                  I B E X                                   
// File        : ibex_P_ExprPrinter.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_P_ExprPrinter.h"
#include "ibex_P_Expr.h"

namespace ibex {

namespace parser {

void P_ExprPrinter::print(std::ostream& os, const ExprNode& e) {
	ExprPrinter::print(os,e);
}

void P_ExprPrinter::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void P_ExprPrinter::visit(const P_ExprPower& e) {
	(*os) << "(";
	visit(e.left);
	(*os) << "^";
	visit(e.right);
	(*os ) << ")";
}

void P_ExprPrinter::visit(const P_ExprIndex& e) {
	visit(e.left);
	(*os) << (e.matlab_style? '(' : '[');
	visit(e.right);
	(*os ) << (e.matlab_style? ')' : ']');
}

void P_ExprPrinter::visit(const ExprConstantRef& e) {
	(*os) << "<cst>"; //e.name;
}

void P_ExprPrinter::visit(const ExprIter& e) {
	(*os) << e.name;
}

void P_ExprPrinter::visit(const ExprInfinity& e) {
	(*os) << "oo";
}

} // end namespace parser

} // end namespace ibex
