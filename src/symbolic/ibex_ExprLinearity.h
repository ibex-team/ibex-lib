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
 *
 * Note: should be faster (and more robust) than using symbolic differentiation.
 *
 */
class ExprLinearity : public virtual ExprVisitor {
public:
	/**
	 * \brief Check the linearity of y (and all its subexpressions).
	 */
	ExprLinearity(const Array<const ExprSymbol> x, const ExprNode& y);

	/**
	 * \brief True only if the expression "e" is linear.
	 *
	 */
	bool is_linear(const ExprNode& e) const;

	/**
	 * \brief Return the coefficients of a scalar subexpression "e" of "y".
	 *
	 * If e is not linear with respect to a variable x_j, the
	 * corresponding coefficient v_j in the returned vector v
	 * is set to (-oo,oo).
	 *
	 * \pre e must be scalar
	 */
	IntervalVector coeff_vector(const ExprNode& e) const;

	/**
	 * \brief Return the coefficients of a vector-valued subexpression "e" of "y".
	 *
	 * If e_i is not linear with respect to a variable x_j, the
	 * corresponding coefficient A_ij in the returned matrix A
	 * is set to (-oo,oo).
	 *
	 * \pre e must be column-vector-valued
	 */
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

	typedef enum { CONSTANT, LINEAR, NONLINEAR } nodetype;

	typedef std::pair<Array<Domain>*,nodetype> LinData;

	Array<Domain>* build_zero(const Dim& dim) const;
	LinData build_cst(const Domain& d) const;

	void binary(const ExprBinaryOp& e, Domain (*fcst)(const Domain&,const Domain&), bool linear_op);
	void unary(const ExprUnaryOp& e, Domain (*fcst)(const Domain&), bool linear_op);

	int n; // number of variables


	/**
	 * Array<Domain>* is a (n+1) array of domains.
	 * These domains are the vector of coefficients
	 * if the node is a linear expression. The
	 * last domain corresponds to the additive constant.
	 *
	 * The non-linearity of a node with respect to a variable
	 * is represented by an unbounded domain (-oo,oo).
	 *
	 * The nodetype indicates if the whole subexpression is constant
	 * linear or non-linear.
	 */
	NodeMap<LinData> _coeffs;
};

inline bool ExprLinearity::is_linear(const ExprNode& e) const {
	return _coeffs.found(e) &&
			_coeffs[e].second<NONLINEAR;
}

} /* namespace ibex */

#endif /* __IBEX_EXPR_LINEARITY_H__ */
