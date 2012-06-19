//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 25, 2012
// Last Update : May 25, 2012
//============================================================================

#ifndef __IBEX_PARSER_EXPR_H__
#define __IBEX_PARSER_EXPR_H__

#include <vector>
#include "ibex_Expr.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_ParserExprVisitor.h"

namespace ibex {

namespace parser {

/*
 * Power at parse time.
 */
class P_ExprPower : public ExprBinaryOp {
	P_ExprPower(const ExprNode& expr, const ExprNode& expon) : ExprBinaryOp(expr,expon,Dim(1,1,1)) { }

	virtual void acceptVisitor(ExprVisitor& v) const {
		// never called
	}

	/** Accept an #ibex::parser::P_ExprVisitor visitor. */
	virtual void acceptVisitor(P_ExprVisitor& v) const {
		v.visit(*this);
	}

};

/**
 * An indexed expression, like x[i+1], at parse time
 */
class P_ExprIndex : public ExprBinaryOp {
public:
	P_ExprIndex(const ExprNode& expr, const ExprNode& index) : ExprBinaryOp(expr,index,expr.dim.index_dim()) { }

	virtual void acceptVisitor(ExprVisitor& v) const {
		// never called
	}

	/** Accept an #ibex::parser::P_ExprVisitor visitor. */
	virtual void acceptVisitor(P_ExprVisitor& v) const {
		v.visit(*this);
	}
};

/**
 * A symbol + a domain.
 */
class P_ExprSymbol: public ExprSymbol {
public:
	P_ExprSymbol(const char* name, const Dim& dim, const Interval& domain) : ExprSymbol(name,dim), domain(domain) { }

	virtual void acceptVisitor(ExprVisitor& v) const {
		// never called
	}

	/** Accept an #ibex::parser::P_ExprVisitor visitor. */
	virtual void acceptVisitor(P_ExprVisitor& v) const {
		v.visit(*this);
	}

	Interval domain;
};

} // end namespace parser

} // end namespace ibex
#endif // __IBEX_PARSER_EXPR_H__
