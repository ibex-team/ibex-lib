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
#include "ibex_ExprCtr.h"

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Numerical constraint.
 *
 * A numerical constraint is a constraint under the form
 * f(x)<0, f(x)<=0, f(x)=0, f(x)>=0 or f(x)>0 where
 * f is a #ibex::Function.
 */
class NumConstraint {

public:

	/**
	 * \brief Build a constraint.
	 */
	NumConstraint(Function& f, CmpOp op=EQ);

	/**
	 * \brief Build a constraint (f(x) op 0).
	 *
	 * If c=(expr,op), the constraint is f(x) op 0 with f:x->expr.
	 */
	NumConstraint(const ExprSymbol& x, const ExprCtr& c);

	/**
	 * \brief Build a constraint (f(x1,x2) op 0).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprCtr& c);

	/**
	 * \brief Build a constraint (f(x1,x2,x3) op 0).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprCtr& c);


	/**
	 * \brief Build a constraint (f(x1,x2,x3,x4) op 0).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprCtr& c);


	/**
	 * \brief Build a constraint (f(x1,x2,x3,x4,x5) op 0).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprCtr& c);


	/**
	 * \brief Build a constraint (f(x1,x2,x3,x4,x5,x6) op 0).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprCtr& c);

	/**
	 * \brief Creates a constraint (f(x1,...,xn) op 0).
	 */
	NumConstraint(const Array<const ExprSymbol>& x, const ExprCtr& c);

	/**
	 * \bried Delete *this.
	 */
	~NumConstraint();

	/**
	 * \brief The function.
	 */
	Function& f;

	/**
	 * \brief The comparison operator.
	 */
	const CmpOp op;

protected:
	bool own_f;
};

std::ostream& operator<<(std::ostream&, const NumConstraint&);


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline NumConstraint::NumConstraint(Function& f, CmpOp op) : f(f), op(op), own_f(false) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x, const ExprCtr& c) : f(*new Function(x,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprCtr& c): f(*new Function(x1,x2,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprCtr& c): f(*new Function(x1,x2,x3,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprCtr& c): f(*new Function(x1,x2,x3,x4,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprCtr& c): f(*new Function(x1,x2,x3,x4,x5,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprCtr& c): f(*new Function(x1,x2,x3,x4,x5,x6,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const Array<const ExprSymbol>& x, const ExprCtr& c): f(*new Function(x,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::~NumConstraint() {
	if (own_f) delete &f;
}

inline std::ostream& operator<<(std::ostream& os, const NumConstraint& c) {
	return os << c.f.expr() << c.op << "0";
}

} // namespace ibex
#endif // __IBEX_NUM_CONSTRAINT_H__
