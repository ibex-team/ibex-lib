//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAll.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Aug 21, 2014
//============================================================================

#ifndef __IBEX_CTC_FORALL_H__
#define __IBEX_CTC_FORALL_H__

#include "ibex_CtcQuantif.h"

#include <stack>

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Projection-union operator (for universally-quantified constraints)
 *
 * This operator allows to contract a box [x] with respect to:
 *
 *    y in[y] =>  c(x,y).
 *
 * where y is a vector of "parameters".
 */
class CtcForAll : public CtcQuantif {
public:

	/**
	 * \brief Create the contractor for "For all y1 in [y_init], c(x,y)".
	 *
	 * \see #CtcQuantif(const NumConstraint&, const VarSet&, const IntervalVector&, double).
	 */
	CtcForAll(const NumConstraint& c, const ExprNode& y1, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "For all y in [y_init], c(x,y)"
	 *
	 * \see #CtcQuantif::CtcQuantif(const NumConstraint& c, const VarSet&, const IntervalVector& y_init, double prec);
	 */
	CtcForAll(const NumConstraint& c, const VarSet& y, const IntervalVector& y_init, double prec);

	/**
	 * \brief Proj-union operator applied on the contractor c.
	 *
	 * \see #CtcQuantif::CtcQuantif(Ctc& c, const VarSet&, const IntervalVector& y_init, double prec, bool own_ctc=false);
	 */
	CtcForAll(Ctc& c, const BitSet& vars, const IntervalVector& y_init, double prec, bool own_ctc=false);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& x, ContractContext& context);

private:
	/**
	 * Function call by contract to proceed a pair (x,y).
	 *
	 * The overall result x is updated and, if large enough, y is pushed in the list "l".
	 *
	 * \param x:   the current box "x". Corresponds, at the end, to the result of the contraction
	 * \param y:   the current box "y"
	 */
	void proceed(IntervalVector& x, const IntervalVector& y, bool& is_inactive);

	/**
	 * Stack of y
	 */
	std::stack<IntervalVector> l;

};

typedef CtcForAll CtcProjInter;

} // end namespace ibex


#endif // __IBEX_CTC_FORALL_H__
