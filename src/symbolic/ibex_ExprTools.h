//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprTools.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#ifndef __IBEX_EXPR_TOOLS_H__
#define __IBEX_EXPR_TOOLS_H__

namespace ibex {

/**
 * Delete all the DAG represented by expr, including expr.
 *
 * \param delete_symbols if false, symbols are not deleted.
 */
void cleanup(const ExprNode& expr, bool delete_symbols);

/**
 * Return the size of the DAG (left op right)
 */
int bin_size(const ExprNode& left, const ExprNode& right);

/**
 * Return the size of the DAG (arg[0] op ... op arg[n-1])
 */
int nary_size(const ExprNode** args, int n);

} // end namespace

#endif // __IBEX_EXPR_TOOLS_H__
