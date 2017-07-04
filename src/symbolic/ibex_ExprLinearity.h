/* ============================================================================
 * I B E X - Expression linearity check
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : July 3rd, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_EXPR_LINEARITY_H__
#define __IBEX_EXPR_LINEARITY_H__

#include "ibex_ExprVisitor.h"
#include "ibex_Array.h"
#include "ibex_NodeMap.h"

#include <utility>

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Linearity Test for Expressions
 */
class ExprLinearity : public virtual ExprVisitor {
public:
	/**
	 * \brief Check the linearity of y (and all its subexpressions).
	 */
	ExprLinearity(const Array<const ExprSymbol> x, const ExprNode& y);

	/**
	 * \brief Return the coefficients of a subexpression "e" of "y".
	 *
	 * If e is not linear (or if it is not a subexpression of y), the
	 * results is an empty interval vector.
	 */
	IntervalVector coeff_vector(const ExprNode& e) const;

	IntervalMatrix coeff_matrix(const ExprNode& e) const;

	/**
	 * \brief Delete this.
	 */
	virtual ~ExprLinearity();

protected:
	void visit(const ExprSymbol& e, int k);

	void visit(const ExprNode& e);
	void visit(const ExprIndex& e);
	void visit(const ExprSymbol& e);
	void visit(const ExprConstant& e);
	void visit(const ExprVector& e);
	void visit(const ExprApply& a);
	void visit(const ExprChi& a);
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

	Array<Domain>* build_zero(const Dim& dim) const;
	std::pair<Array<Domain>*, bool> build_cst(const Domain& d) const;

	void binary(const ExprBinaryOp& e, Domain (*fcst)(const Domain&,const Domain&), bool linear_op);
	void unary(const ExprUnaryOp& e, Domain (*fcst)(const Domain&), bool linear_op);

	int n; // number of variables

	/**
	 * Domain** is a (n+1) array of domains.
	 * These domains are the vector of coefficients
	 * if the node is a linear expression. The
	 * last domain corresponds to the additive constant.
	 *
	 * coeffs[e] is NULL if e is not a
	 * linear expression.
	 *
	 * The boolean indicates if the expression is constant
	 * or not.
	 */
	NodeMap<std::pair<Array<Domain>*, bool> > _coeffs;
};

} /* namespace ibex */

#endif /* __IBEX_EXPR_LINEARITY_H__ */
