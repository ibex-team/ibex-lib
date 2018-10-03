//============================================================================
//                                  I B E X
// File        : ibex_ExprOperators.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 28, 2018
// Last update : Oct 02, 2018
//============================================================================

#ifndef __IBEX_EXPR_OPERATORS_H__
#define __IBEX_EXPR_OPERATORS_H__

#include "ibex_Expr.h"
#include "ibex_SymbolMap.h"

namespace ibex {

template<const char* Name, class Input, class Output>
class UnaryOperator {
public:
	static Dim dim(const Dim& x_dim);

	static Output eval(const Input& x);

	static void bwd(Input& x, const Output& y);

	static Input diff(const Input& x);

	static const ExprNode& diff(const ExprNode& expr);
};

/**
 * \ingroup symbolic
 * \brief Unary expression
 */
class ExprGenericUnaryOp : public ExprUnaryOp {
public:
	
	static const ExprGenericUnaryOp& new_(const char* op_name, const ExprNode& expr);

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

	typedef Dim (*dim_func)(const Dim& x);

	typedef Domain (*eval_func)(const Domain& x);

	typedef void (*bwd_func)(Domain& x, const Domain& y);

	typedef Domain (*num_diff_func)(const Domain& x);

	typedef const ExprNode& (*symb_diff_func)(const ExprNode& expr);

	eval_func eval;
	bwd_func bwd;
	num_diff_func num_diff;
	symb_diff_func symb_diff;

	static eval_func get_eval(const char* name);

	/** Operator name, ex: "cos", "exp". */
	const char* name;

protected:
	typedef struct {
		dim_func dim;
		eval_func eval;
		bwd_func bwd;
		num_diff_func num_diff;
		symb_diff_func symb_diff;
	} OperatorDef;

	static OperatorDef get(const char* name);

	ExprGenericUnaryOp(const char* name, const OperatorDef& def, const ExprNode& subexpr);

	~ExprGenericUnaryOp();
};


} // end namespace

#endif /* __IBEX_EXPR_OPERATORS_H__ */
