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

using namespace std;

namespace ibex {
namespace parser {

void p_print(const ExprNode& e) {
	P_ExprPrinter().print(cout,e);
}

P_ExprPower::P_ExprPower(const ExprNode& expr, const ExprNode& expon) : ExprBinaryOp(expr,expon,expr.dim) {

}


P_ExprIndex::P_ExprIndex(const ExprNode& expr, const ExprNode& index) : ExprBinaryOp(expr,index,expr.dim.index_dim()) {

}

ExprIter::ExprIter(const char* name) : ExprNode(0,1,Dim()), name(strdup(name)) {

}

ExprIter::~ExprIter() {
	free((char*) name);
}

//ExprEntity::ExprEntity(const Entity& e, int line) : ExprNode(0,1,e.dim), entity(e), line(line) {
//
//}

} // end namespace parser
} // end namespace ibex
