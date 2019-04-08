/* ============================================================================
 * I B E X - ibex_FncProj.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec, 26 2018
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_PROJECTION_H__
#define __IBEX_FNC_PROJECTION_H__

#include "ibex_System.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Project a function onto selected components
 */
class FncProj : public Fnc {
public:
	/**
	 * \brief Build the function.
	 *
	 */
	FncProj(const Fnc& fnc, const BitSet& components);

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
	 * \brief The original function
	 */
	const Fnc& fnc;

	/**
	 * \brief The components
	 */
	const BitSet components;

};

} /* namespace ibex */

#endif /* __IBEX_FNC_PROJECTION_H__ */
