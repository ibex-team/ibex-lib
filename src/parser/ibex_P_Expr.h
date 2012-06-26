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
#include "ibex_P_ExprVisitor.h"
#include "ibex_Entity.h"

namespace ibex {

namespace parser {

/*
 * Power at parse time.
 */
class P_ExprPower : public ExprBinaryOp {
public:
	P_ExprPower(const ExprNode& expr, const ExprNode& expon);

	virtual void acceptVisitor(ExprVisitor& v) const {
		P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
		if (v2) v2->visit(*this);
		else v.visit(*this);
	}
};

/**
 * An indexed expression, like x[i+1], at parse time
 */
class P_ExprIndex : public ExprBinaryOp {
public:
	P_ExprIndex(const ExprNode& expr, const ExprNode& index);

	virtual void acceptVisitor(ExprVisitor& v) const {
		P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
		if (v2) v2->visit(*this);
		else v.visit(*this);
	}
};

/**
 * An iterator symbol in an expression
 */
class ExprIter : public ExprNode {
public:
	ExprIter(const char* name);

	~ExprIter();

	virtual void acceptVisitor(ExprVisitor& v) const {
		P_ExprVisitor* v2=dynamic_cast<P_ExprVisitor*>(&v);
		assert(v2);
		v2->visit(*this);
	}

	const char* name;
};

void p_print(const ExprNode& e);

/**
 * An entity symbol in an expression
 */
//class ExprEntity: public ExprNode {
//public:
//	ExprEntity(const Entity& e, int lineno);
//
//	virtual void acceptVisitor(ExprVisitor& v) const {
//		// never called
//	}
//
//	/** Accept an #ibex::parser::P_ExprVisitor visitor. */
//	virtual void acceptVisitor(P_ExprVisitor& v) const {
//		v.visit(*this);
//	}
//
//	const Entity& entity;
//	int line;  // line in the program
//};


} // end namespace parser

} // end namespace ibex
#endif // __IBEX_PARSER_EXPR_H__
