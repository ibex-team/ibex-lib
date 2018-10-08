//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2DAG.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
//============================================================================

#ifndef __IBEX_EXPR_2_DAG_H__
#define __IBEX_EXPR_2_DAG_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"

namespace ibex {


/**
 *  \ingroup symbolic
 *
 * \brief Transform an expression to a minimal-size DAG
 *
 * The expression can be a tree or, partially, a DAG.
 *
 */
class Expr2DAG : public virtual ExprVisitor {
public:
	/**
	 * \brief Transform y to a DAG
	 */
	const ExprNode& transform(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprNAryOp& e);
	void visit(const ExprLeaf& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
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

	NodeMap<const ExprNode*> peer;

	Array<const ExprNode> comps(const ExprNAryOp& e);

	template<class T>
	void visit_unary(const T& e);

	template<class T>
	void visit_binary(const T& e);
};

} // namespace ibex

#endif // __IBEX_EXPR_2_DAG_H__
