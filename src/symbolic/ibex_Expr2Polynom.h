//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2Polynom.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 27, 2020
// Last update : May 12, 2020
//============================================================================

#ifndef __IBEX_EXPR_2_POLYNOM_H__
#define __IBEX_EXPR_2_POLYNOM_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"
#include "ibex_ExprPolynomial.h"
#include "ibex_IntervalMatrix.h"

namespace ibex {

class ExprSimplify2;

/**
 * Get a polynomial expression from an expression.
 *
 * This class must not be used directly.
 *
 * It is to be used only through ExprSimplify2.
 *
 * \ingroup symbolic
 *
 */
class Expr2Polynom : public virtual ExprVisitor<const ExprPolynomial*> {
public:
	/**
	 * Note: the expression is assumed to have already been simplified
	 *       by ExprSimplPolynom.
	 *
	 * \param develop - Multiplication/square operator are all developed
	 * \param simp    - The simplifier is necessary for two reasons:
	 *                  * for cleanup: some nodes created by Expr2PolynomSome may not
	 *                  appear in the final expression.
	 *                  * the multiplication of monomials may require to apply a
	 *                  transposition and thus a call to simplification (but only
	 *                  the systematic part, no recursive call to Expr2Polynom).
	 */
	Expr2Polynom(ExprSimplify2& simp, bool develop=false);

	/**
	 * Delete this.
	 */
	~Expr2Polynom();

	/**
	 * Get the resulting polynomial.
	 */
	const ExprPolynomial* get(const ExprNode& e);

	/**
	 * Remove everything from the cache.
	 *
	 * Does not remove the unused nodes in #record
	 */
	void cleanup();

protected:

	const ExprPolynomial* visit(const ExprNode& e);
	const ExprPolynomial* visit(const ExprIndex& i);
	const ExprPolynomial* visit(const ExprSymbol& x);
	const ExprPolynomial* visit(const ExprConstant& c);
	const ExprPolynomial* visit(const ExprVector& e);
	const ExprPolynomial* visit(const ExprApply& e);
	const ExprPolynomial* visit(const ExprChi& e);
	const ExprPolynomial* visit(const ExprGenericBinaryOp& e);
	const ExprPolynomial* visit(const ExprAdd& e);
	const ExprPolynomial* visit(const ExprMul& e);
	const ExprPolynomial* visit(const ExprSub& e);
	const ExprPolynomial* visit(const ExprDiv& e);
	const ExprPolynomial* visit(const ExprMax& e);
	const ExprPolynomial* visit(const ExprMin& e);
	const ExprPolynomial* visit(const ExprAtan2& e);
	const ExprPolynomial* visit(const ExprGenericUnaryOp& e);
	const ExprPolynomial* visit(const ExprMinus& e);
	const ExprPolynomial* visit(const ExprTrans& e);
	const ExprPolynomial* visit(const ExprSign& e);
	const ExprPolynomial* visit(const ExprAbs& e);
	const ExprPolynomial* visit(const ExprPower& e);
	const ExprPolynomial* visit(const ExprSqr& e);
	const ExprPolynomial* visit(const ExprSqrt& e);
	const ExprPolynomial* visit(const ExprExp& e);
	const ExprPolynomial* visit(const ExprLog& e);
	const ExprPolynomial* visit(const ExprCos& e);
	const ExprPolynomial* visit(const ExprSin& e);
	const ExprPolynomial* visit(const ExprTan& e);
	const ExprPolynomial* visit(const ExprCosh& e);
	const ExprPolynomial* visit(const ExprSinh& e);
	const ExprPolynomial* visit(const ExprTanh& e);
	const ExprPolynomial* visit(const ExprAcos& e);
	const ExprPolynomial* visit(const ExprAsin& e);
	const ExprPolynomial* visit(const ExprAtan& e);
	const ExprPolynomial* visit(const ExprAcosh& e);
	const ExprPolynomial* visit(const ExprAsinh& e);
	const ExprPolynomial* visit(const ExprAtanh& e);
	const ExprPolynomial* visit(const ExprFloor& e);
	const ExprPolynomial* visit(const ExprCeil& e);
	const ExprPolynomial* visit(const ExprSaw& e);

	const ExprPolynomial* unary(const ExprUnaryOp& e, std::function<const ExprUnaryOp&(const ExprNode&)> f);

	const ExprPolynomial* binary(const ExprBinaryOp& e, std::function<const ExprBinaryOp&(const ExprNode&, const ExprNode&)> f);

	const ExprPolynomial* nary(const ExprNAryOp& e, std::function<const ExprNAryOp&(const Array<const ExprNode>&)> f);

	/*
	 * Record the node in #record for cleanup
	 */
	const ExprNode& rec(const ExprNode&);

	bool develop;

	ExprSimplify2& simp;
};

} // namespace ibex

#endif // __IBEX_EXPR_2_POLYNOM_H__
