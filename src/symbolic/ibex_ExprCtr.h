//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCtr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#ifndef __IBEX_EXPR_CTR_H__
#define __IBEX_EXPR_CTR_H__

#include "ibex_Expr.h"
#include "ibex_CmpOp.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Constraint expression.
 *
 * This class simply encapsulates an expression (see #ibex::ExprNode) and
 * a comparison operator, like =.
 */
class ExprCtr {
public:

	/** \brief Build a constraint expression. */
	ExprCtr(const ExprNode& e, CmpOp op);

	/** The expression. */
	const ExprNode& e;
	/** The operator. */
	const CmpOp op;
};


std::ostream& operator<<(std::ostream&, const ExprCtr&);

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline ExprCtr::ExprCtr(const ExprNode& e, CmpOp op) : e(e), op(op) {

}

inline std::ostream& operator<<(std::ostream& os, const ExprCtr& c) {
	return os << c.e << c.op << "0";
}

inline const ExprCtr& ExprNode::operator=(const ExprNode& right) const {
  return *new ExprCtr(right.is_zero()? *this : *this-right, EQ);
}

inline const ExprCtr& ExprNode::operator=(const Interval& value) const {
  return *new ExprCtr(value.lb()==0 && value.ub()==0? *this : *this-value, EQ);
}

inline const ExprCtr& ExprNode::operator=(const IntervalVector& value) const {
  return *new ExprCtr(value.is_zero()? *this : *this-value, EQ);
}

inline const ExprCtr& ExprNode::operator=(const IntervalMatrix& value) const {
  return *new ExprCtr(value.is_zero()? *this : *this-value, EQ);
}

inline const ExprCtr& ExprNode::operator<=(const ExprNode& right) const {
  return *new ExprCtr(right.is_zero()? *this : *this-right, LEQ);
}

inline const ExprCtr& ExprNode::operator<=(const Interval& value) const {
  return *new ExprCtr(value.lb()==0 && value.ub()==0? *this : *this-value, LEQ);
}

inline const ExprCtr& ExprNode::operator>=(const ExprNode& right) const {
  return *new ExprCtr(right.is_zero()? *this : *this-right, GEQ);
}

inline const ExprCtr& ExprNode::operator>=(const Interval& value) const {
  return *new ExprCtr(value.lb()==0 && value.ub()==0? *this : *this-value, GEQ);
}

inline const ExprCtr& ExprNode::operator<(const ExprNode& right) const {
  return *new ExprCtr(right.is_zero()? *this : *this-right, LT);
}

inline const ExprCtr& ExprNode::operator<(const Interval& value) const {
  return *new ExprCtr(value.lb()==0 && value.ub()==0? *this : *this-value, LT);
}

inline const ExprCtr& ExprNode::operator>(const ExprNode& right) const {
  return *new ExprCtr(right.is_zero()? *this : *this-right, GT);
}

inline const ExprCtr& ExprNode::operator>(const Interval& value) const {
  return *new ExprCtr(value.lb()==0 && value.ub()==0? *this : *this-value, GT);
}


} // end namespace ibex

#endif // __IBEX_EXPR_CTR_H__
