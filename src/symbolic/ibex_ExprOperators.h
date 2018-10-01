//============================================================================
//                                  I B E X
// File        : ibex_ExprOperators.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 28, 2018
//============================================================================

#ifndef __IBEX_EXPR_OPERATORS_H__
#define __IBEX_EXPR_OPERATORS_H__

#include "ibex_Expr.h"
#include "ibex_SymbolMap.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Unary expression
 */
class ExprGenericUnaryOp : public ExprUnaryOp {
public:
	
	static const ExprGenericUnaryOp& new_(std::string op_name, const ExprNode& expr, const Dim& dim);

	/** Create an equality constraint op(expr)=expr. */
	const ExprCtr& operator=(const ExprNode& expr) const { return ((ExprNode&) *this)=expr; }

	/** Create an equality constraint op(expr)=value. */
	const ExprCtr& operator=(const Interval& value) const  { return ((ExprNode&) *this)=value; }

	/** Create an equality constraint op(expr)=value. */
	const ExprCtr& operator=(const IntervalVector& value) const  { return ((ExprNode&) *this)=value; }

	/** Create an equality constraint op(expr)=value. */
	const ExprCtr& operator=(const IntervalMatrix& value) const  { return ((ExprNode&) *this)=value; }

	/** Accept an #ibex::ExprVisitor visitor. */
	virtual void acceptVisitor(ExprVisitor& v) const { v.visit(*this); };

	/**
	 * Create a new node with a given expression (dynamic variant).
	 */
	//virtual const ExprGenericUnaryOp& new__(const ExprNode& expr) const=0;

	void (*fwd)(const Domain& x, Domain& y);

	void (*bwd)(Domain& x, const Domain& y);

	void (*num_diff)(const Domain& x, Domain& g);

	const ExprNode& (*symb_diff)(const ExprNode& expr);

	/** Operator name, ex: "cos", "exp". */
	const char* name;

	/** Forward evaluation.
	 *  Set y to f(x). */
	//Domain eval(const Domain& x) const;

protected:

	ExprGenericUnaryOp(const char* name, const ExprNode& subexpr, const Dim& dim);

	~ExprGenericUnaryOp();
};


} // end namespace

#endif /* __IBEX_EXPR_OPERATORS_H__ */
