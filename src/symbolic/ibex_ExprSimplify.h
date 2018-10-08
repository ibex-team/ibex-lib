//============================================================================
//                                  I B E X
// File        : ibex_ExprSimplify.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 19, 2016
//============================================================================

#ifndef __IBEX_EXPR_SIMPLIFY_H__
#define __IBEX_EXPR_SIMPLIFY_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"
#include "ibex_DoubleIndex.h"
#include "ibex_Domain.h"

#include <vector>
#include <utility>

namespace ibex {

class ExprSimplify : public ExprVisitor {
public:

	/**
	 * \warning The function cleanup all unused nodes which
	 * may include e itself (so the node "e" may not exist
	 * after calling this function).
	 * However, it does not delete symbols.
	 */
	const ExprNode& simplify(const ExprNode& e);


	/*
	 * If some nodes should not be removed although they
	 * do not belong to the simplified expression, add
	 * them in this map. (note: this is a kind of hack,
	 * a better solution could be found).
	 */
	NodeMap<bool> lock;

protected:
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

	DoubleIndex idx;
	NodeMap<std::vector<std::pair<DoubleIndex, const ExprNode*> >*> idx_clones;

	void insert(const ExprNode& e, const ExprNode& e2);
	const ExprNode& get(const ExprNode& e, const DoubleIndex&);

	template<class N>
	void unary(const N& e, Domain (*fcst)(const Domain&));
	template<class N>
	void binary(const N& e, Domain (*fcst)(const Domain&,const Domain&));

	void visit_add_sub(const ExprBinaryOp& e, bool sign);

//	void unary_copy(const ExprUnaryOp& e, const ExprNode& (*func)(const ExprNode&));
//	void binary_copy(const ExprBinaryOp& e, const ExprNode& (*func2)(const ExprNode&, const ExprNode&));
//	bool unary_eval(const ExprUnaryOp& e, Domain (*fcst)(const Domain&));
//	bool binary_eval(const ExprBinaryOp& e, Domain (*fcst)(const Domain&, const Domain&));
};

} /* namespace ibex */

#endif /* __IBEX_EXPR_SIMPLIFY_H__ */
