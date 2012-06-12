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
 * Integer, double and Interval constants at parse time.
 *
 * remark: private inheritance to avoid conflits between the
 * operator= between 2 constants and 2 expressions.
 */
class P_ExprConstant : public ExprNode {
public:
	P_ExprConstant(int);
	P_ExprConstant(double);
	P_ExprConstant(const Interval&);
	P_ExprConstant(const IntervalVector&);
	P_ExprConstant(const IntervalMatrix&);
	P_ExprConstant(const std::vector<P_ExprConstant>&);

	P_ExprConstant& operator=(const P_ExprConstant& c);

	~P_ExprConstant();

	virtual void acceptVisitor(ExprVisitor& v) const {
		// never called
	}

	/** Accept an #ibex::parser::P_ExprVisitor visitor. */
	virtual void acceptVisitor(P_ExprVisitor& v) const {
		v.visit(*this);
	}

	P_ExprConstant operator[](int i) const;

	const ExprConstant& to_cst() const;

	const ExprNode& _2expr() const { return *this; }

	virtual bool is_zero() const;

	bool is_scalar() const;

	/* Conversions */
	int _2int() const;
	double _2double() const;
	Interval _2itv() const;
	IntervalVector _2itvv() const;
	IntervalMatrix _2itvM() const;

	int type;
	int n;
	double r;
	Interval x;
	IntervalVector v;
	IntervalMatrix M;

	/*  int* intv;
  double* doublev;
  int size; */
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

} // end namespace parser

} // end namespace ibex
#endif // __IBEX_PARSER_EXPR_H__
