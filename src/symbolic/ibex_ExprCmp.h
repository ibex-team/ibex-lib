//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCmp.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
// Last update : Mar 27, 2020
//============================================================================

#ifndef __IBEX_EXPR_CMP_H__
#define __IBEX_EXPR_CMP_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Compare two expressions
 *
 * So far the two expressions must share the same symbols (TODO).
 *
 */
class ExprCmp : public virtual ExprVisitor<int,const ExprNode&> {
public:
	/**
	 * \brief Compare e1 and e2.
	 *
	 * \¶eturn -1 if e1 < e2
	 *          0 if e1 ==e2
	 *          1 if e1 > e2.
	 */
	int compare(const ExprNode& e1, const ExprNode& e2);

	/**
	 * \brief Comparison of two intervals
	 */
	static int compare(const Interval& x1, const Interval& x2);

	/**
	 * \brief Comparison of two interval vectors
	 */
	static int compare(const IntervalVector& x1, const IntervalVector& x2);

	/**
	 * \brief Comparison of two interval matrices
	 */
	static int compare(const IntervalMatrix& m1, const IntervalMatrix& m2);

	/**
	 * \brief Comparison of two interval vectors (TODO)
	 */
	static int compare(const Vector& x1, const Vector& x2);

	/**
	 * \brief Comparison of two interval matrices (TODO)
	 */
	static int compare(const Matrix& m1, const Matrix& m2);

protected:
	int visit(const ExprNode& e, const ExprNode&);
	int visit(const ExprIndex& i, const ExprNode&);
	int visit(const ExprSymbol& x, const ExprNode&);
	int visit(const ExprConstant& c, const ExprNode&);
	int visit(const ExprVector& e, const ExprNode&);
	int visit(const ExprApply& e, const ExprNode&);
	int visit(const ExprChi& e, const ExprNode&);
	int visit(const ExprGenericBinaryOp& e, const ExprNode&);
	int visit(const ExprAdd& e, const ExprNode&);
	int visit(const ExprMul& e, const ExprNode&);
	int visit(const ExprSub& e, const ExprNode&);
	int visit(const ExprDiv& e, const ExprNode&);
	int visit(const ExprMax& e, const ExprNode&);
	int visit(const ExprMin& e, const ExprNode&);
	int visit(const ExprAtan2& e, const ExprNode&);
	int visit(const ExprGenericUnaryOp& e, const ExprNode&);
	int visit(const ExprMinus& e, const ExprNode&);
	int visit(const ExprTrans& e, const ExprNode&);
	int visit(const ExprSign& e, const ExprNode&);
	int visit(const ExprAbs& e, const ExprNode&);
	int visit(const ExprPower& e, const ExprNode&);
	int visit(const ExprSqr& e, const ExprNode&);
	int visit(const ExprSqrt& e, const ExprNode&);
	int visit(const ExprExp& e, const ExprNode&);
	int visit(const ExprLog& e, const ExprNode&);
	int visit(const ExprCos& e, const ExprNode&);
	int visit(const ExprSin& e, const ExprNode&);
	int visit(const ExprTan& e, const ExprNode&);
	int visit(const ExprCosh& e, const ExprNode&);
	int visit(const ExprSinh& e, const ExprNode&);
	int visit(const ExprTanh& e, const ExprNode&);
	int visit(const ExprAcos& e, const ExprNode&);
	int visit(const ExprAsin& e, const ExprNode&);
	int visit(const ExprAtan& e, const ExprNode&);
	int visit(const ExprAcosh& e, const ExprNode&);
	int visit(const ExprAsinh& e, const ExprNode&);
	int visit(const ExprAtanh& e, const ExprNode&);
	int visit(const ExprFloor& e, const ExprNode&);
	int visit(const ExprCeil& e, const ExprNode&);
	int visit(const ExprSaw& e, const ExprNode&);

	template<class T>
	int visit_nary(const T& e, const ExprNode&);

	template<class T>
	int visit_unary(const T& e, const ExprNode&);

	template<class T>
	int visit_binary(const T& e, const ExprNode&);

	/*
	 * Cache.
	 * Order: The first node is the one with lesser id.
	 */
	NodeMap<NodeMap<int> > cache;

};

} // namespace ibex

#endif // __IBEX_EXPR_CMP_H__
