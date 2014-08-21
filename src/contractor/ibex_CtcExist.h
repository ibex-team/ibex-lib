//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcQuantif.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : May 7, 2014
//============================================================================

#ifndef __IBEX_CTC_EXIST_H__
#define __IBEX_CTC_EXIST_H__

#include "ibex_CtcQuantif.h"

#include <stack>
#include <list>

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Projection-union operator (for existentially-quantified constraints)
 *
 * This operator allows to contract a box [x] with respect to:
 *
 *    exists y in[y] |  c(x,y).
 *
 * where y is a vector of "parameters".
 */
class CtcExist : public CtcQuantif {
public:

	/**
	 * \brief Create the contractor for "exists y1 in [y_init] such that c(x,y)".
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,y2) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,y2,y3) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y4) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y5) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y6) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y7) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y8) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y9) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y10) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y11) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists (y1,...,y12) in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif(const NumConstraint&, const Array<const ExprSymbol&>, const IntervalVector&, double).
	 */
	CtcExist(const NumConstraint& c, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const ExprSymbol& y12, const IntervalVector& y_init, double prec);

	/**
	 * \brief Create the contractor for "exists y in [y_init] such that c(x,y)"
	 *
	 * \see #CtcQuantif::CtcQuantif(const NumConstraint& c, const Array<const ExprSymbol>& y, const IntervalVector& y_init, double prec);
	 */
	CtcExist(const NumConstraint& c, const Array<const ExprSymbol>& y, const IntervalVector& y_init, double prec);

	/**
	 * \brief Proj-union operator applied on the contractor c.
	 *
	 * \see #CtcQuantif::CtcQuantif(Ctc& c, const BitSet& vars, const IntervalVector& y_init, double prec, bool own_ctc=false);
	 */
	CtcExist(Ctc& c, const BitSet& vars, const IntervalVector& y_init, double prec, bool own_ctc=false);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& x);

private:
	/**
	 * Function call by contract to proceed a pair (x,y).
	 *
	 * The overall result x_res is updated, as well as y, and a new pair (x',y') is possibly pushed in the list "l".
	 *
	 * \param x_init:    the initial box "x" to be contracted
	 * \param x_current: the current box "x" generated by the branch & bound inside the contract(...) function
	 * \param x_res:     the current state of the overall result (proj-union). Corresponds, at the end, to the result
	 *                   of the contraction
	 * \param y:         the current box "y"
	 */
	void proceed(const IntervalVector& x_init, const IntervalVector& x_current, IntervalVector& x_res, IntervalVector& y);

	/**
	 * Stack of pairs (x,y)
	 */
	std::stack<std::pair<IntervalVector,IntervalVector> > l;

};

typedef CtcExist CtcProjUnion;

} // end namespace ibex

#endif // __IBEX_CTC_EXIST_H__
