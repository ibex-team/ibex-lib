//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprReset.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_EXPR_RESET_H__
#define __IBEX_EXPR_RESET_H__

#include "ibex_ExprVisitor.h"

namespace ibex {

/**
 * \brief Reset all the "visited" flags of an expression
 */
class ExprReset : public virtual ExprVisitor {
public:
	void reset(const ExprNode& e);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprLeaf& f);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
};

} // end namespace ibex
#endif // __IBEX_EXPR_RESET_H_
