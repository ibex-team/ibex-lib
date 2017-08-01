/* ============================================================================
 * I B E X - ibex_FncActivation.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_ACTIVATION_H__
#define __IBEX_FNC_ACTIVATION_H__

#include "ibex_System.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Components of a system function that correspond to activate constraints.
 */
class FncActivation : public Fnc {
public:
	/**
	 * \brief Build the function.
	 *
	 * Each component corresponds to an equality or an (almost) activate inequality.
	 * The inequality g(x)<=0 is considered as activate if |g(x)|<=activation_threshold.
	 *
	 * \param trace - if true, activation of inequalities is displayed
	 */
	FncActivation(const System& sys, const Vector& pt, double activation_threshold=1e-6, bool trace=false);

	/**
	 * \brief Delete this.
	 */
	virtual ~FncActivation();

	/**
	 * \brief Evaluation of the function.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& x, const BitSet& components) const;

	/**
	 * \brief Jacobian matrix of the function.
	 */
	virtual void jacobian(const IntervalVector& x_lambda, IntervalMatrix& J, const BitSet& components, int v) const;

	/**
	 * \brief Get the activated constraints
	 */
	const BitSet& activated() const;

protected:
	const System& sys;
	BitSet _activated;
};

inline const BitSet& FncActivation::activated() const {
	return _activated;
}

} /* namespace ibex */

#endif /* __IBEX_FNC_ACTIVATION_H__ */
