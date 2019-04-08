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
#include "ibex_Id.h"

namespace ibex {

class System;

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
	NumConstraint(const Function& f, CmpOp op=EQ, bool own_f=false);

	/**
	 * \brief Build a constraint c(x).
	 *
	 * If c=(expr,op), the constraint is f(x) op 0 with f:x->expr.
	 */
	NumConstraint(const ExprSymbol& x1, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprCtr& c);

	/**
	 * \brief Build the constraint c(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20).
	 */
	NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprSymbol& x20, const ExprCtr& c);

	/**
	 * \brief Creates the constraint (f(x1,...,xn) op 0).
	 */
	NumConstraint(const Array<const ExprSymbol>& x, const ExprCtr& c);

	/**
	 * \brief Create a constraint from a Minibex input file.
	 */
	NumConstraint(const char* filename);

	/**
	 * \brief Creates a constraint c(x1).
	 *
	 * The parameter x1 may not only contain the name
	 * of the constraint argument but also the dimension of the symbol
	 * (like for the arguments of a Minibex Function). E.g.:
	 *
	 * NumConstraint c("x[2]","x(1)<=x(2)");
	 *
	 * \warning: Note that the Minibex convention (not C++) is applied for indices!
	 *
	 */
	NumConstraint(const char* x1, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3,x4).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3,x4,x5).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3,x4,x5,x6).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3,x4,x5,x6,x7).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* c);

	/**
	 * \brief Creates a constraint c(x1,x2,x3,x4,x5,x6,x7,x8).
	 *
	 * \see NumConstraint(const char* x1, const char* c).
	 */
	NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* c);

	/**
	 * \brief Build a constraint c(x)
	 */
	NumConstraint(const Array<const char*>& x, const char* c);

	/**
	 * \bried Delete *this.
	 */
	~NumConstraint();

	/**
	 * \brief The domain "d" such that the constraint is equivalent to "f in d".
	 */
	Domain right_hand_side() const;

	/**
	 * \brief Identifying number.
	 */
	const long id;

	/**
	 * \brief The function.
	 */
	const Function& f;

	/**
	 * \brief The comparison operator.
	 */
	const CmpOp op;

	/*
	 * \brief Check if this constraint is a thick equality.
	 *
	 * A thick equality is a constraint under the form g(x)=[a,b]
	 * or [a,b]=g(x). In this case, we have f(x)=+/-(g(x)-[a,b]).
	 *
	 * This function returns a pair containing pointers to
	 * the subexpression g(x) and the constant [a,b], if f
	 * can be decomposed in this way. Otherwise, it returns
	 * the pair <NULL,NULL>.
	 *
	 * Note: an inequality g(x)<=[a,b] is not considered as
	 * a thick equality, although it could...
	 *
	 * TODO: extend to vector/matrix constraints.
	 */
//	std::pair<const ExprNode*, const Interval*> is_thick_equality() const;

private:

	bool own_f;

	void build_from_string(const Array<const char*>& x, const char* c);

	void build_from_system(const System& sys);
};

std::ostream& operator<<(std::ostream&, const NumConstraint&);


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline NumConstraint::NumConstraint(const Function& f, CmpOp op, bool own_f) : id(next_id()), f(f), op(op), own_f(own_f) { }

inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprCtr& c) : id(next_id()), f(*new Function(x1,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprCtr& c): id(next_id()), f(*new Function(x1,x2,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprCtr& c): id(next_id()), f(*new Function(x1,x2,x3,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprCtr& c): id(next_id()), f(*new Function(x1,x2,x3,x4,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprCtr& c): id(next_id()), f(*new Function(x1,x2,x3,x4,x5,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprCtr& c): id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,c.e)), op(c.op), own_f(true) { }
inline NumConstraint::NumConstraint(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprSymbol& x20, const ExprCtr& c):
		id(next_id()), f(*new Function(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::NumConstraint(const Array<const ExprSymbol>& x, const ExprCtr& c): id(next_id()), f(*new Function(x,c.e)), op(c.op), own_f(true) { }

inline NumConstraint::~NumConstraint() {
	if (own_f) delete &f;
}

inline std::ostream& operator<<(std::ostream& os, const NumConstraint& c) {
	return os << c.f.expr() << c.op << "0";
}

} // namespace ibex
#endif // __IBEX_NUM_CONSTRAINT_H__
