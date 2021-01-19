//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSize.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : May 04, 2020
//============================================================================

#ifndef __IBEX_EXPR_SIZE_H__
#define __IBEX_EXPR_SIZE_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * \brief Calculate the size of a DAG
 */
class ExprSize : public ExprVisitor<int> {
public:
	/**
	 * Return the size of the DAG (left op right)
	 *
	 * For binary expressions (BinOpExpr).
	 */
	int bin_size(const ExprNode& l, const ExprNode& r);

	/**
	 * Return the size of the DAG (arg[0] op ... op arg[n-1])
	 *
	 * For n-ary expressions (ExprApply, ExprVector,...).
	 */
	int nary_size(const Array<const ExprNode>& args);

protected:
	virtual int visit(const ExprNode& e);
	virtual int visit(const ExprIndex& e);
	virtual int visit(const ExprLeaf& e);
	virtual int visit(const ExprNAryOp& e);
	virtual int visit(const ExprBinaryOp& e);
	virtual int visit(const ExprUnaryOp& e);
};

} // end namespace ibex

#endif // __IBEX_EXPR_SIZE_H__
