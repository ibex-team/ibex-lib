//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : Jul 11, 2019
//============================================================================

#ifndef __IBEX_FNC_KHUN_TUCKER_H__
#define __IBEX_FNC_KHUN_TUCKER_H__

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

class FncKhunTucker : public Fnc {
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
	FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box);

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
	FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet& active);

	/**
	 * \brief Delete this.
	 */
	~FncKhunTucker();

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
	 * \brief Multiplier initial domain
	 *
	 * The domain of an inequality multiplier is [-1,1] and
	 * [0,1] for an equality multiplier.
	 */
	IntervalVector multiplier_domain() const;

	/*
	 * \brief Function of active constraints
	 *
	 * Useful to apply a quick qualification check
	 * or a first-order rejection test.
	 */
	const FncActiveCtrs& f_active() const;

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
	FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet* active);

	FncActiveCtrs* act;            // function of active constraints

	Function& df;                  // gradient of objective function

	Array<Function> dg;            // gradients of active (in)equalities

	BitSet nothing;                // for the case where nothing is active.
};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline FncKhunTucker::FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet& active) :
		FncKhunTucker(sys,df,dg,box,&active) {
}

inline FncKhunTucker::FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box) :
		FncKhunTucker(sys,df,dg,box,NULL) {
}

inline const FncActiveCtrs& FncKhunTucker::f_active() const {
	return *act;
}

} /* namespace ibex */

#endif /* __IBEX_FNC_KHUN_TUCKER_H__ */
