//============================================================================
//                                  I B E X                                   
// File        : ibex_CmpOp.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================


#ifndef __IBEX_CMP_OP_H__
#define __IBEX_CMP_OP_H__

#include <iostream>

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Comparison operator.
 *
 * Used, e.g., in #ibex::NumConstraint and #ibex::ExprCtr.
 */
typedef enum { LT, LEQ, EQ, GEQ, GT } CmpOp;

/**
 * \ingroup symbolic
 * \brief Return the negation of op.
 * \pre op must be different from EQ.
 */
CmpOp operator!(CmpOp op);

/**
 * \brief Display the operator.
 */
std::ostream& operator<<(std::ostream&, const CmpOp&);

} // end namespace ibex

#endif // __IBEX_CMP_OP_H__
