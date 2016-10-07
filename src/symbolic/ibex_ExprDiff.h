//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprDiff.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 25, 2013
// Last Update : Feb 25, 2013
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
	 * \brief Return the differential
	 *
	 * The differential is either a row vector for a real-valued function (the gradient)
	 * or a matrix (the Jacobian matrix) for a vector-valued function.
	 *
	 * The node in return may either be an #ExprVector or a #ExprConstant.
	 */
	const ExprNode& diff(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
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

	const ExprNode& gradient(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y);

	void add_grad_expr(const ExprNode& node, const ExprNode& expr);

	NodeMap<const ExprNode*> grad;

	// Leaves of this expression including symbols with respect
	// to which we are calculating derivative and that do not appear
	// in the expression (assimilated to "leaves" here, although they
	// are not part of the DAG). Information for cleanup only.
	std::vector<const ExprNode*> leaves;
};

} // end namespace ibex
#endif // __IBEX_EXPR_DIFF_H__
