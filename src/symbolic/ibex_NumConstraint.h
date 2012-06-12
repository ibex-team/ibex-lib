/* ============================================================================
 * I B E X - Numerical constraint
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_NUM_CONSTRAINT_H__
#define __IBEX_NUM_CONSTRAINT_H__

#include "ibex_Function.h"

namespace ibex {

/** \ingroup symbolic
 * \brief Numerical constraint.
 *
 * A numerical constraint is a constraint under the form
 * f(x)=0 or f(x)<=0.
 */
class NumConstraint {

public:

	typedef enum { LT, LEQ, EQ, GEQ, GT } CompOp;

	NumConstraint(Function& f, CompOp op=EQ);

	Function& f;
	const CompOp op;
};


} // namespace ibex
#endif // __IBEX_NUM_CONSTRAINT_H__
