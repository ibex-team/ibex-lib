//============================================================================
//                                  I B E X
// File        : ibex_FncKuhnTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : Jul 11, 2019
//============================================================================

#ifndef __IBEX_FNC_KUHN_TUCKER_H__
#define __IBEX_FNC_KUHN_TUCKER_H__

#include "ibex_NormalizedSystem.h"
#include "ibex_FncActiveCtrs.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Function corresponding to KKT conditions.
 *
 * This function is basically the gradient of the lagrangian
 * of the system:
 *
 *  l0*grad(f) + l1*grad(g1) + ...
 *
 * and it has to be zero for all critical points
 * of the NLP.
 *
 * It is designed to be dynamically built, that is, for a given box.
 * This way, inactive constraints are discarded and computations are
 * more efficient. So the dimensions of the function (number of variables
 * and co-dimension) may change with the input box.
 *
 * The function is more precisely built as follows.
 *
 * Given:
 *  - n variables
 *  - ni active inequalities
 *  - ne equalities
 *  - nl active left bound constraints, x[j_1]>=l_1,....,x[j_nl]>=l_nl
 *  - nr active right bound constraints X[k_1]<=r_1,....,x[k_nr]<=r_nr
 *
 * The n first components of f(x,l) are:
 *
 *     l[0]*grad(f)
 *   + l[1]*grad(g_1)       + ... + l[ni]*grad(g_ni)
 *   + l[ni+1]*grad(h_1)    + ... + l[ni+ne]*grad(h_ne)
 *   - l[ni+ne+1]*e(j_1)    - ... - l[ni+ne+nl]*e(j_nl)    where e(i)=(0,..0,1,0,...,0), (e(i)[i]=1)
 *   + r[ni+ne+nl+1]*e(k_1) + ... + r[ni+ne+nl+nr]*e(k_nr)
 *
 * The next component is for the normalization equation of multipliers:
 *
 *    l[0]
 *   + l[1]        + ... + l[ni]
 *   + l[ni+1]^2   + ... + l[ni+ne]^2
 *   + l[ni+ne+1]  + ... + l[ni+ne+nl]
 *   + l[ni+ne+nl] + ... + l[ni+ne+nl+nr]
 *   - 1
 *
 * The ni following components are
 *     l[1]*g_1(x),
 *     ...,
 *     l[ni]*g_ni(x)
 *
 * The ne following components are
 *     h_1(x),
 *     ...
 *     h_ne(x)
 *
 * The nl following components are
 *     l[ni+ne+1]*(r_1-x[j_1])
 *     ...
 *     l[ni+ne+nl]*(r_nl-x[j_nl])
 *
 * The nr last components are
 *     r[ni+ne+nl+1]*(x[k_1]-r_1)
 *     ...
 *     r[ni+ne+nl+nr]*(x[k_nr]-r_nr)
 */

class FncKuhnTucker : public Fnc {
public:

	/**
	 * \brief Build the KKT conditions function for a given box.
	 *
	 * \param sys -    Original NLP
	 * \param df -     Symbolic gradient of the objective
	 * \param dg -     Symbolic gradient of constraints. NULL if unconstrained problem.
	 * 			       Note: we ask an array of functions and not just a single matrix-valued
	 *                 function, because Ibex does not handle differentiation of matrix function
	 *                 (would require tensors)
	 * \param box -    current box (not to be confused with the system "box", i.e., bounding constraints).
	 *                 The current box is precisely used to determine active constraints.
 	 */
	FncKuhnTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box);

	/**
	 * \brief Build the KKT conditions function for a given box.
	 *
	 * This variant allows to decide which constraints has to be activated.
	 *
	 * \param sys -    see other constructor.
	 * \param df -     see other constructor.
	 * \param dg -     see other constructor.
	 * \param box -    current box (not to be confused with the system "box", i.e., bounding constraints).
	 *                 The current box is used to determine active bound constraints (the active inequalities
	 *                 being given in argument).
	 * \param active - (potentially) active inequalities on the box. Must be indices of the
	 *                 components of sys.f_ctrs.
 	 */
	FncKuhnTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet& active);

	/**
	 * \brief Delete this.
	 */
	~FncKuhnTucker();

	/**
	 * \brief Evaluation
	 *
	 * \see #ibex::Fnc
	 *
	 * The vector x_lambda contains both original variables domains and multipliers domains,
	 * organized as follows:
	 *
	 *  - original variables
	 *  - multiplier 'l0' of the objective
	 *  - multipliers 'mu*' of the inequalities (respecting their order in the original system)
	 *  - multipliers 'l*' of the equalities (idem)
	 *  - multipliers of the active left bounds of the system box (respecting the order of variables)
	 *  - multipliers of the active right bounds of the system box  (respecting the order of variables)
	 *
	 * The "x" component of the box must be a subbox of the box given in argument to the constructor.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& x_lambda, const BitSet& components) const;

	/**
	 * \brief Jacobian matrix
	 *
	 * \see #ibex::Fnc
	 */
	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

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
	 * NOTE: this field does not account for linear independence
	 * of constraints gradients. Only a count of potentially
	 * active constraints is done.
	 */
	bool qualified() const;

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

	/**
	 * \brief Multiplier initial domain
	 *
	 * The domain of an inequality multiplier is [-1,1] and
	 * [0,1] for an equality multiplier.
	 */
	IntervalVector multiplier_domain() const;

	/*
	 * Bitset of active constraints
	 */
	const BitSet& active() const;

	/*
	 * Indices of active constraints that are equalities.
	 *
	 * E.g. if the active constraints are {3, 4, 7, 9} and constraint n°3 and 7 are equalities
	 * then eq={0,2}.
	 * So the bitset does not contain the original indices of equalities (in the system).
	 */
	const BitSet& eq() const;

	/*
	 *  Indices of constrains that are inequalities
	 *  Same as for equalities.
	 */
	const BitSet& ineq() const;

	/*
	 * Indices of variables with left bound active
	 */
	const BitSet& left_bound() const;

	/*
	 * Indices of variables with right bound active
	 */
	const BitSet& right_bound() const;

	/**
	 * \brief Original system
	 */
	const NormalizedSystem& sys;

	/**
	 * \brief Number of variables in the original system
	 */
	const int n;

	/**
	 * \brief Total number of multipliers.
	 */
	const int nb_mult;

protected:
	FncKuhnTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet* active);

	FncActiveCtrs* act;            // function of active constraints

	Function& df;                  // gradient of objective function

	Array<Function> dg;            // gradients of active (in)equalities

	BitSet nothing;                // for the case where nothing is active.
};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline FncKuhnTucker::FncKuhnTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet& active) :
		FncKuhnTucker(sys,df,dg,box,&active) {
}

inline FncKuhnTucker::FncKuhnTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box) :
		FncKuhnTucker(sys,df,dg,box,NULL) {
}

inline bool FncKuhnTucker::qualified() const {
	return act? act->qualified() : false;
}

inline bool FncKuhnTucker::rejection_test(const IntervalVector& x) const {
	return act? act->rejection_test(x) : false;
}

inline const BitSet& FncKuhnTucker::active() const      { return act? act->active_ctr : nothing; }

inline const BitSet& FncKuhnTucker::eq() const          { return act? act->eq : nothing; }

inline const BitSet& FncKuhnTucker::ineq() const        { return act? act->ineq : nothing; }

inline const BitSet& FncKuhnTucker::left_bound() const  { return act? act->active_left_bound : nothing; }

inline const BitSet& FncKuhnTucker::right_bound() const { return act? act->active_right_bound : nothing; }



} /* namespace ibex */

#endif /* __IBEX_FNC_KUHN_TUCKER_H__ */
