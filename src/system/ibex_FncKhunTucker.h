//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : May 17, 2019
//============================================================================

#ifndef __IBEX_FNC_KHUN_TUCKER_H__
#define __IBEX_FNC_KHUN_TUCKER_H__

#include "ibex_NormalizedSystem.h"

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
	 * \param df -     Symbolic gradient of the objective
	 * \param dg -     Symbolic gradient of constraints. NULL if unconstrained problem.
	 * \param box -    current box (not to be confused with the system "box", i.e., bounding constraints).
	 *                 The current box is precisely used to determine active bound constraints.
	 * \param active - (potentially) active inequalities on the box. Must be indices of the
	 *                 components of sys.f_ctrs.
 	 */
	FncKhunTucker(const NormalizedSystem& sys, Function& df, Function** dg, const IntervalVector& box, const BitSet& active);

	/**
	 * \see #ibex::Fnc
	 *
	 * The vector x_lambda contains both original variables domains and multipliers domains,
	 * organized as follows:
	 *
	 *  - original variables
	 *  - multiplier 'l0' of the objective
	 *  - multipliers 'mu*' of the inequalities (respecting their order in the original system)
	 *  - multipliers 'l*' of the equalities (idem)
	 *  - multipliers 'of the left bounds activated (respecting the order of variables)
	 *  - multipliers of the right bounds activated (respecting the order of variables)
	 *
	 * The "x" component of the box must be a subbox of the box given in argument to the constructor.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& x_lambda, const BitSet& components) const;

	/**
	 *\see #ibex::Fnc
	 */
	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

	/**
	 * \return the matrix where the first column is the gradient of f
	 * and the other columns are the gradients of active constraints.
	 *
	 * "x" must be a subbox of the box given in argument to the constructor.
	 **/
	IntervalMatrix gradients(const IntervalVector& x) const;

	/**
	 * \return The multiplier initial domain
	 *
	 * The domain of an inequality multiplier is [-1,1] and
	 * [0,1] for an equality multiplier.
	 */
	IntervalVector multiplier_domain() const;

	/**
	 * Total number of multipliers.
	 */
	const int nb_mult;

//protected: // temporary public

	int n;                         // number of variables in the original system
	const NormalizedSystem& sys;   // original system
	Function& df;                  // gradient of objective function
	Array<Function> dg;            // gradients of active constraints
	BitSet active;                 // bitset of active constraints

	// TODO: put these bitsets in a Bxp?

	/*
	 * Indices of active constraints that are equalities.
	 *
	 * E.g. if the active constraints are {3, 4, 7, 9} and constraint n°3 and 7 are equalities
	 * then eq={0,2}.
	 * So the bitset does not contain the original indices of equalities (in the system).
	 */
	BitSet eq;

	/*
	 *  Indices of constrains that are inequalities
	 *  Same as for equalities.
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
};

} /* namespace ibex */

#endif /* __IBEX_FNC_KHUN_TUCKER_H__ */
