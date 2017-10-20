//============================================================================
//                                  I B E X
// File        : ibex_FncKhunTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 26, 2017
// Last Update : Jul 26, 2017
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
 */

class FncKhunTucker : public Fnc {
public:

	/**
	 * \brief Build the KKT conditions function for a given box.
	 *
	 * \param df -     Symbolic gradient of the objective
	 * \param dg -     Symbolic gradient of constraints. NULL if unconstrained problem.
	 * \param box -    current box (not to be confused with the system "box", i.e., bounding constraints)
	 * \param active - (potentially) active constraints on the box.
 	 */
	FncKhunTucker(const NormalizedSystem& sys, Function* df, Function** dg, const IntervalVector& box, const BitSet& active);

	/**
	 *\see #ibex::Fnc
	 */
	virtual IntervalVector eval_vector(const IntervalVector& x_lambda, const BitSet& components) const;

	/**
	 *\see #ibex::Fnc
	 */
	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

	/**
	 * \return the matrix where the first column is the gradient of f
	 * and the other columns are the gradients of active constraints.
	 */
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

	int n; // number of original variables
	const NormalizedSystem& sys;
	Function* df;
	Function** dg;

	// TODO: put these bitsets in a backtrackable structure?
	BitSet eq;
	BitSet ineq;
	BitSet bound_left;
	BitSet bound_right;
};

} /* namespace ibex */

#endif /* __IBEX_FNC_KHUN_TUCKER_H__ */
