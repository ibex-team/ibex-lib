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
#include "ibex_CmpOp.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Numerical constraint.
 *
 * A numerical constraint is a constraint under the form
 * f(x)=0 or f(x)<=0.
 */
class NumConstraint {

public:

	NumConstraint(Variable& x, const ExprCtr& e);

	NumConstraint(Function& f, CmpOp op=EQ);

	Function& f;
	const CmpOp op;
};

std::ostream& operator<<(std::ostream&, const NumConstraint&);


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline NumConstraint::NumConstraint(Function& f, CmpOp op) : f(f), op(op) {

}

inline std::ostream& operator<<(std::ostream& os, const NumConstraint& c) {
	return os << c.f.expr() << c.op << "0";
}

} // namespace ibex
#endif // __IBEX_NUM_CONSTRAINT_H__
