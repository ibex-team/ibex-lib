//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#include "ibex_ParserExpr.h"

namespace ibex {
namespace parser {

// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Domain& domain) :
				symbol(ExprSymbol::new_(name,dim)), domain(domain,false), type(VAR) {

}

// VAR is a temporary value
Entity::Entity(const char* name, const Dim& dim, const Interval& x) :
				symbol(ExprSymbol::new_(name,dim)), domain(domain,false), type(VAR) {

	switch(dim.type()) {
	case Dim::SCALAR :      domain.i()=x; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   domain.v().init(x); break;
	case Dim::MATRIX:       domain.m().init(x); break;
	case Dim::MATRIX_ARRAY: for (int i=0; i<domain.ma().size(); i++) domain.ma()[i].init(x); break;
	}

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
