//============================================================================
//                                  I B E X
// File        : ibex_ExprOccCounter.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 08, 2020
// Last update : May 12, 2020
//============================================================================

#ifndef __IBEX_EXPR_OCC_COUNTER_H__
#define __IBEX_EXPR_OCC_COUNTER_H__

#include "ibex_ExprVisitor.h"

namespace ibex {

/**
 * \brief Count symbol occurrences in an expression.
 *
 * The total number of occurrences of the syntax tree
 * (not the DAG). Hence, in the expression x+x, there are two
 * occurrences, even if there is only a single node "x".
 *
 * Mutable constants do not count for symbols.
 */
class ExprOccCounter : public ExprVisitor<Matrix*> {
public:
	/**
	 * Return the count as a matrix, each
	 * component for each corresponding component
	 * of the expression.
	 *
	 * The returned matrix is actually a matrix
	 * of integers.
	 */
	const Matrix& count(const ExprNode&) const;

	/**
	 * Total of occurrence for an expression
	 * (gathering all components), which
	 * counting is stored in m.
	 */
	unsigned int total(const Matrix& m) const;

	/**
	 * Occurrence count for the product of two
	 * expression (which counting is stored in
	 * resp. m1 and m2).
	 */
	Matrix occ_product(const Matrix& m1, const Matrix& m2) const;

protected:
	Matrix* visit(const ExprNode& e);
	Matrix* visit(const ExprIndex& i);
	/*
	 * Default behavior, overridden
	 * in the case of ExprVector
	 */
	Matrix* visit(const ExprNAryOp& e);
	/*
	 * Default behavior, overridden
	 * in the case of +,-,* and generic binary op.
	 */
	Matrix* visit(const ExprBinaryOp& b);
	/*
	 * Default behavior, overridden
	 * in the case of -,transpose and generic unary op.
	 */
	Matrix* visit(const ExprUnaryOp& u);
	Matrix* visit(const ExprSymbol& x);
	Matrix* visit(const ExprConstant& c);
	Matrix* visit(const ExprVector& e);
	Matrix* visit(const ExprGenericBinaryOp& e);
	Matrix* visit(const ExprAdd& e);
	Matrix* visit(const ExprMul& e);
	Matrix* visit(const ExprSub& e);
	Matrix* visit(const ExprGenericUnaryOp& e);
	Matrix* visit(const ExprMinus& e);
	Matrix* visit(const ExprTrans& e);
	unsigned int total(const ExprNode& e) const;
};

} /* namespace ibex */

#endif /* __IBEX_EXPR_OCC_COUNTER_H__ */
