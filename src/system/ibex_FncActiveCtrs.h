/* ============================================================================
 * I B E X - ibex_FncActiveCtrs.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * Last update : Jun, 28 2019
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_ACTIVE_CTRS_H__
#define __IBEX_FNC_ACTIVE_CTRS_H__

#include "ibex_System.h"
#include "ibex_BitSet.h"
#include "ibex_FncProj.h"

namespace ibex {

class FncKhunTucker;

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
	 * Each component corresponds to an equality or an (almost) activate inequality.
	 * The inequality g(x)<=0 is considered as activate if |g(x)|<=activation_threshold.
	 *
	 * \param ineq_first - if true, the inequalities appear first and, then, the equalities
	 *                     (useful for KKT conditions). Otherwise, the components appear
	 *                     in their declaration order in the original system, regardless
	 *                     of their type.
	 *
	 * \throw NothingActive exception - This is because a function cannot have a null codimension in Ibex.
	 */
	FncActiveCtrs(const System& sys, const Vector& pt, double activation_threshold=1e-6, bool ineq_first=true);

	FncActiveCtrs(const System& sys, const IntervalVector& box, bool ineq_first=true);

	/**
	 * This variant allows to decide which constraints has to be activated.
	 *
	 * \throw NothingActive exception - This is because a function cannot have a null codimension in Ibex.
	 */
	FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet& active, bool ineq_first=true);

	/**
	 * \brief Delete this.
	 */
	~FncActiveCtrs();

	/**
	 * \brief Get the active inequality/equality constraints
	 *
	 * Does not include bound constraints.
	 */
	const BitSet& activated() const;

	/**
	 * \brief Get the active left bounds
	 */
	const BitSet& active_left_bound() const;

	/**
	 * \brief Get the active right bounds
	 */
	const BitSet& active_right_bound() const;

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

//protected:

	void init(const System& sys, const BitSet& active);

	const System& sys;   // original system

	bool ineq_first;

	/*
	 * Bitset of active constraints
	 */
	BitSet active;

	/*
	 * Indices of active constraints that are equalities.
	 * (only used if ineq_first==true)
	 *
	 * E.g. if the active constraints are {3, 4, 7, 9} and constraint n°3 and 7 are equalities
	 * then eq={0,2}.
	 * So the bitset does not contain the original indices of equalities (in the system).
	 */
	BitSet eq;

	/*
	 *  Indices of constrains that are inequalities
	 *  Same as for equalities.
	 * (only used if ineq_first==true)
	 */
	BitSet ineq;

	/*
	 * Indices of variables with left bound active
	 */
	BitSet bound_left;

	/*
	 * Indices of variables with right bound active
	 */
	BitSet bound_right;

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
	 */
	bool LICQ;

	protected:

	friend class FncKhunTucker;

	/**
	 * Function with the active components.
	 */
	FncProj* fact;

	FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet* active, bool ineq_first);

};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, bool ineq_first) :
	FncActiveCtrs(sys,box,NULL,ineq_first) {
}

inline FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet& active, bool ineq_first) :
		FncActiveCtrs(sys,box,&active,ineq_first) {
}

} /* namespace ibex */

#endif /* __IBEX_FNC_ACTIVE_CTRS_H__ */
