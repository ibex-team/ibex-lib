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

template<const char* Name, class Input1, class Input2, class Output>
class BinaryOperator {
public:
	static Dim dim(const Dim& x1_dim, const Dim& x2_dim);

	static Output eval(const Input1& x1, const Input2& x2);

	static void bwd(const Output& y, Input1& x1, Input2& x2);

	static Input1 diff1(const Input1& x1, const Input2& x2, const Output& g);

	static Input2 diff2(const Input1& x1, const Input2& x2, const Output& g);

	static const ExprNode& diff1(const ExprNode& x1, const ExprNode& x2, const ExprNode& g);

	static const ExprNode& diff2(const ExprNode& x1, const ExprNode& x2, const ExprNode& g);
};

template<const char* Name, class Input, class Output>
class UnaryOperator {
public:
	static Dim dim(const Dim& x_dim);

	static Output eval(const Input& x);

	static void bwd(const Output& y, Input& x);

	static Input diff(const Input& x, const Output& g);

	static const ExprNode& diff(const ExprNode& x, const ExprNode& g);
};

/**
 * \ingroup symbolic
 * \brief Unary expression
 */
class ExprGenericBinaryOp : public ExprBinaryOp {
public:

	static const ExprGenericBinaryOp& new_(const char* op_name, const ExprNode& left, const ExprNode& right);

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

	typedef Dim (*dim_func)(const Dim& x1, const Dim& x2);

	typedef Domain (*eval_func)(const Domain& x1, const Domain& x2);

	typedef void (*bwd_func)(const Domain& y, Domain& x1, Domain& x2);

	typedef Domain (*num_diff_func)(const Domain& x1, const Domain& x2, const Domain& g);

	typedef const ExprNode& (*symb_diff_func)(const ExprNode& expr1, const ExprNode& expr2, const ExprNode& g);

	eval_func eval;
	bwd_func bwd;
	num_diff_func num_diff1;
	num_diff_func num_diff2;
	symb_diff_func symb_diff1;
	symb_diff_func symb_diff2;

	static eval_func get_eval(const char* name);

	/** Operator name, ex: "cos", "exp". */
	const char* name;

protected:
	ExprGenericBinaryOp(const char* name, const ExprNode& left, const ExprNode& right);

	~ExprGenericBinaryOp();
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

	typedef void (*bwd_func)(const Domain& y, Domain& x);

	typedef Domain (*num_diff_func)(const Domain& x, const Domain& g);

	typedef const ExprNode& (*symb_diff_func)(const ExprNode& expr, const ExprNode& g);

	eval_func eval;
	bwd_func bwd;
	num_diff_func num_diff;
	symb_diff_func symb_diff;

	static eval_func get_eval(const char* name);

	/** Operator name, ex: "cos", "exp". */
	const char* name;

protected:
	ExprGenericUnaryOp(const char* name, const ExprNode& subexpr);

	~ExprGenericUnaryOp();
};


} // end namespace

#endif /* __IBEX_EXPR_OPERATORS_H__ */
