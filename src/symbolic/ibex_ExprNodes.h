//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprNodes.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_EXPR_NODES_H__
#define __IBEX_EXPR_NODES_H__

#include "ibex_ExprVisitor.h"

namespace ibex {

/**
 * \brief Return all the (sub)nodes of an expression (including itself)
 */
class ExprNodes : public virtual ExprVisitor {
public:

	/**
	 * \brief Return the (sub)nodes of e.
	 *
	 * \warning The result array must be freed by the caller
	 */
	const ExprNode** nodes(const ExprNode& e);

protected:
	const ExprNode** subnodes;
	int j;
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprLeaf& e);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
};

} // end namespace ibex
#endif // __IBEX_EXPR_NODES_H__
