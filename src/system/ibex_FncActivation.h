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
#include "ibex_FncProj.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Components of a system function that correspond to activate constraints.
 */
class FncActivation : public FncProj {
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
	 * \brief Get the activated constraints
	 */
	const BitSet& activated() const;

protected:
	//const System& sys;
};

inline const BitSet& FncActivation::activated() const {
	return components;
}

} /* namespace ibex */

#endif /* __IBEX_FNC_ACTIVATION_H__ */
