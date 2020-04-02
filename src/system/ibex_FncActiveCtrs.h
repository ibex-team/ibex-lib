/* ============================================================================
 * I B E X - ibex_FncActiveCtrs.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * Last update : Jul, 11 2019
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_ACTIVE_CTRS_H__
#define __IBEX_FNC_ACTIVE_CTRS_H__

#include "ibex_System.h"
#include "ibex_BitSet.h"
#include "ibex_FncProj.h"

namespace ibex {

class FncKuhnTucker;

/**
 * \ingroup system
 *
 * \brief Constraints and domain bounds that correspond to active constraints.
 */
class FncActiveCtrs : public Fnc {
public:

	class NothingActive : public Exception { };

	/**
	 * \brief Build the function.
	 *
	 * Each component corresponds to either:
	 * - an equality
	 * - a possibly active inequality.
	 * - a possibly active left or right bound of the system box
	 *
	 * \param sys        - original system
	 * \param pt         - point where constraint activity is checked
	 * \param activation_threshold
	 *                   - The inequality g(x)<=0 is considered as active if
	 *                     |g(pt)|<=activation_threshold. Same for system bounds.
	 * \param ineq_first - if true, the inequalities appear first and, then, the equalities
	 *                     (useful for KKT conditions). Otherwise, the components appear
	 *                     in their declaration order in the original system, regardless
	 *                     of their type. In both case left and right bounds follow.
	 * \param df_ctrs -    (optional) Symbolic gradient of constraints. NULL if not provided.
	 *
	 * \throw NothingActive exception -
	 *                     Thrown when nothing is active. This is necessary because a function
	 *                     cannot have a null codimension in Ibex.
	 */
	FncActiveCtrs(const System& sys, const Vector& pt, double activation_threshold=1e-6, bool ineq_first=true, const Function* df_ctrs=NULL);

	/**
	 * \brief Build the function.
	 *
	 * Each component corresponds to either:
	 * - an equality
	 * - a possibly active inequality.
	 * - a possibly active left or right bound of the system box
	 *
	 * \param sys        - original system
	 * \param box        - box where constraint activity is checked. An inequality g(x)<=0
	 *                     is considered as "possibly active" if g([box]) contains 0 (simple
	 *                     interval evaluation).
	 * \param ineq_first - if true, the inequalities appear first and, then, the equalities
	 *                     (useful for KKT conditions). Otherwise, the components appear
	 *                     in their declaration order in the original system, regardless
	 *                     of their type. In both case left and right bounds follow.
	 * \param df_ctrs -    (optional) Symbolic gradient of constraints. NULL if not provided.
	 *
	 * \throw NothingActive exception -
	 *                     Thrown when nothing is active. This is necessay because a function
	 *                     cannot have a null codimension in Ibex.
	 */
	FncActiveCtrs(const System& sys, const IntervalVector& box, bool ineq_first=true, const Function* df_ctrs=NULL);

	/**
	 * \brief Build the function.
	 *
	 * See details in the comments of the other constructors.
	 *
	 * This variant allows the user to decide which constraints is to be considered as active.
	 */
	FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet& active, bool ineq_first=true, const Function* df_ctrs=NULL);

	/**
	 * \brief Delete this.
	 */
	~FncActiveCtrs();

	/**
	 * Linear independence constraint qualification.
	 *
	 * If false, qualification is not respected
	 * (resorting to Newton is useless in this case).
	 *
	 * Two typical situations:
	 * - there are too many active inequalities (usually
	 *   results from overestimation of interval arithmetic)
	 * - the left and right bounds are simultaneously
	 *   active for the same variable (results from too
	 *   large domains)
	 *
	 * Note: this function is a fast check. It does not actually
	 *       check for linear independence of the gradients.
	 */
	bool qualified() const;

	/**
	 * \brief Function corresponding to active constraints
	 *
	 * Does not include bound constraints.
	 */
	const Fnc& f_active_ctr() const;

	/**
	 * \brief Evaluation of the function.
	 */
	virtual Interval eval(const IntervalVector& box) const;

	/**
	 * \brief Evaluation of the function.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& x, const BitSet& components) const;

	/**
	 * \brief Jacobian matrix of the function.
	 */
	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

	/**
	 * \brief First-order rejection test.
	 *
	 * \param x       : a subbox of "box" argument given to the constructor.
	 *                  (This function should not be called after the point
	 *                  variant of the constructor).
	 * \return true   : if the box does not contain a local minimum of the system
	 *                  (the test is successful)
	 *         false  : otherwise.
	 */
	bool rejection_test(const IntervalVector& x) const;

	/*
	 * Original system
	 */
	const System& sys;

	/**
	 * Whether inequalities appear first.
	 */
	const bool ineq_first;

	/**
	 * \brief Active inequality/equality constraints
	 *
	 * Does not include bound constraints.
	 */
	const BitSet active_ctr;

	/*
	 * Indices of active constraints that are equalities.
	 * (only used if ineq_first==true)
	 *
	 * E.g. if the active constraints are {3, 4, 7, 9} and constraint n°3 and 7 are equalities
	 * then eq={0,2}.
	 * So the bitset does not contain the original indices of equalities (in the system).
	 */
	const BitSet eq;

	/*
	 *  Indices of constrains that are inequalities
	 *  Same as for equalities.
	 * (only used if ineq_first==true)
	 */
	const BitSet ineq;

	/**
	 * \brief Active left bounds
	 *
	 * Indices of variables with left bound active
	 */
	const BitSet active_left_bound;

	/**
	 * \brief Active right bounds
	 *
	 * Indices of variables with right bound active
	 */
	const BitSet active_right_bound;

protected:

	FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet* active, bool ineq_first, const Function* df_ctrs=NULL);

	/*
	 * Linear independence constraint qualification.
	 * (see comments of #qualified())
	 */
	bool LICQ;

	/*
	 * Function with the active components.
	 */
	FncProj* fact;
};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, bool ineq_first, const Function* df) :
	FncActiveCtrs(sys,box,NULL,ineq_first,df) {
}

inline FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet& active, bool ineq_first, const Function* df) :
	FncActiveCtrs(sys,box,&active,ineq_first,df) {
}

inline const Fnc& FncActiveCtrs::f_active_ctr() const {
	return *fact;
}

inline bool FncActiveCtrs::qualified() const {
	return LICQ;
}

} /* namespace ibex */

#endif /* __IBEX_FNC_ACTIVE_CTRS_H__ */
