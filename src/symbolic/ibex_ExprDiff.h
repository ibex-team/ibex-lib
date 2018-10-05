//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprDiff.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 25, 2013
// Last Update : Jan 12, 2018
//============================================================================

#ifndef __IBEX_EXPR_DIFF_H__
#define __IBEX_EXPR_DIFF_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"
#include "ibex_Function.h"

namespace ibex {


/**
 * \ingroup symbolic
 *
 * \brief Thrown when differentiation failed.
 *
 * Not all expressions are currently differentiable with Ibex.
 * This is the case for expressions with matrix/vector operations.
 */
class ExprDiffException : public Exception {
public:
	ExprDiffException(const std::string& msg);
	std::string msg;
};


std::ostream& operator<< (std::ostream& os, const ExprDiffException& e);

/**
 * \ingroup symbolic
 *
 * \brief Differentiate an expression.
 *
 */
class ExprDiff : public virtual ExprVisitor {

public:

	/**
	 * \brief Differentiation of expressions.
	 *
	 * \warning: used this way, nodes of the original expression
	 * ("y" below) that do not appear in the resulting expression are
	 * destroyed (see ExprSimplify), which may include the node y itself.
	 */
	ExprDiff();

	/**
	 * \brief Differentiation of expressions (variant with copy).
	 *
	 * Each call to diff(y,x) will produce an expression that is
	 * independent from the original one (y).
	 *
	 * \param old_symbols - The symbols of the original expressions, which
	 *                      may include "mute" symbols (that do not appear
	 *                      in a specific expression "y")
	 * \param new_symbols - The corresponding symbols in the resulting
	 *                      expression.
	 */
	ExprDiff(const Array<const ExprSymbol>& old_symbols, const Array<const ExprSymbol>& new_symbols);


	/**
	 * \brief Calculate dy/dx
	 *
	 * The differential is either a row vector for a real-valued function (the gradient)
	 * or a matrix (the Jacobian matrix) for a vector-valued function.
	 *
	 * The node in return may either be an #ExprVector or a #ExprConstant.
	 *
	 * \param y: the original expression
	 * \param x: the variables with respect to which
	 *           we are differentiating (others are considered as constants).
	 *
	 * TODO: allow the arguments x to be sub-nodes of e (not necessarily symbols).
	 * This may be useful in particular for indexed symbols.
	 */
	const ExprNode& diff(const ExprNode& y, const Array<const ExprSymbol>& x);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprGenericBinaryOp& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprGenericUnaryOp& e);
	void visit(const ExprMinus& e);
	void visit(const ExprTrans& e);
	void visit(const ExprSign& e);
	void visit(const ExprAbs& e);
	void visit(const ExprPower& e);
	void visit(const ExprSqr& e);
	void visit(const ExprSqrt& e);
	void visit(const ExprExp& e);
	void visit(const ExprLog& e);
	void visit(const ExprCos& e);
	void visit(const ExprSin& e);
	void visit(const ExprTan& e);
	void visit(const ExprCosh& e);
	void visit(const ExprSinh& e);
	void visit(const ExprTanh& e);
	void visit(const ExprAcos& e);
	void visit(const ExprAsin& e);
	void visit(const ExprAtan& e);
	void visit(const ExprAcosh& e);
	void visit(const ExprAsinh& e);
	void visit(const ExprAtanh& e);

	const ExprNode& gradient(const ExprNode& y, const Array<const ExprSymbol>& x);

	void add_grad_expr(const ExprNode& node, const ExprNode& expr);

	const Array<const ExprSymbol>* old_symbols;
	const Array<const ExprSymbol>* new_symbols;

	NodeMap<const ExprNode*> grad;

	// ======== Information for cleanup only =========
	// Roots of the expression calculated **before simplification**
	// This also includes the "grad" node of symbols w.r.t. which
	// we are calculating derivative and that do not appear in the
	// original expression (they are part of the resulting DAG) and also
	// "grad" node of constants of the original expression, or symbols
	// wrt which we are not differentiating (not in the resulting DAG).
	std::vector<const ExprNode*> groots;
};

} // end namespace ibex

#endif // __IBEX_EXPR_DIFF_H__
