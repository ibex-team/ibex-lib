//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSize.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_EXPR_SIZE_H__
#define __IBEX_EXPR_SIZE_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * Return the size of the DAG (left op right)
 */
int bin_size(const ExprNode& left, const ExprNode& right);

/**
 * Return the size of the DAG (arg[0] op ... op arg[n-1])
 */
int nary_size(const Array<const ExprNode>& args);

/**
 * \brief Calculate the size of a DAG
 */
class ExprSize : public virtual ExprVisitor {
public:
	/** For binary expressions (BinOpExpr). */
	ExprSize(const ExprNode& l, const ExprNode& r);

	/** For n-ary expressions (ExprApply, ExprVector,...). */
	ExprSize(const Array<const ExprNode>& args);

	/** The size of the DAG */
	int size;

protected:
	 void visit(const ExprNode& e);
	 void visit(const ExprIndex& e);
	 void visit(const ExprLeaf& e);
	 void visit(const ExprNAryOp& e);
	 void visit(const ExprBinaryOp& e);
	 void visit(const ExprUnaryOp& e);

	 NodeMap<bool> map;
};

} // end namespace ibex

#endif // __IBEX_EXPR_SIZE_H__
