//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCmp.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
//============================================================================

#ifndef __IBEX_EXPR_CMP_H__
#define __IBEX_EXPR_CMP_H__

#include "ibex_ExprVisitor.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Compare two expressions
 *
 * So far the two expressions must share the same symbols (TODO).
 *
 * In the case of function applications (ExprApply), the Function objects must
 * be the same (this is not a deep comparison in this case). A deep comparison
 * could also be implemented (TODO).
 */
class ExprCmp : public virtual ExprVisitor {
public:
	/**
	 * \brief True iff e1 and e2 are the same
	 */
	bool compare(const ExprNode& e1, const ExprNode& e2);

protected:

	const ExprNode* e2;
	bool are_equal;

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

	template<class T>
	void visit_nary(const T& e);

	template<class T>
	void visit_unary(const T& e);

	template<class T>
	void visit_binary(const T& e);

};

} // namespace ibex

#endif // __IBEX_EXPR_CMP_H__
