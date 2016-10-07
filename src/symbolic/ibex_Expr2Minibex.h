//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2Minibex.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 7, 2015
//============================================================================

#ifndef __IBEX_EXPR_2_MINIBEX_H__
#define __IBEX_EXPR_2_MINIBEX_H__

#include "ibex_ExprPrinter.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Get the Minibex code of an expression
 *
 * \note The root expression is preceded with "return" keyword.
 */
class Expr2Minibex : private ExprPrinter {
public:
	/**
	 * \brief Print the Minibex code of an expression on a given output stream.
	 *
	 * \param human : see #ExprPrinter::print(std::ostream& os, const ExprNode&, bool).
	 */
	void print(std::ostream& os, const ExprNode&, bool human=true);

protected:
	void visit(const ExprNode& e);

	// Associate a subexpression to a temporary symbol number
	NodeMap<int> map;
};

} // namespace ibex

#endif // __IBEX_EXPR_2_MINIBEX_H__
