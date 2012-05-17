/* ============================================================================
 * I B E X - Indicators (contraction parameters)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_INDICATORS_H__
#define __IBEX_INDICATORS_H__

#include "ibex_BoolMask.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Indicators (Parameters of contractors based on a dictionary structure).
 */
class Indicators {

public:

	/**
	 * \brief Creates a new indicators structure.
	 *
	 * The indicators are all "off" (not activated) and
	 * the bits of each indicators are all "false" (unset).
	 */
	Indicators(int n);

	/**
	 * \brief Set whether a variable has been impacted
	 * since the last call to the contractor.
	 */
	BoolMask impact;

	/**
	 * \brief Set whether a variable has to be
	 * considered by the next call to the contractor.
	 */
	BoolMask scope;

	/**
	 * \brief Whether the impact is activated (on).
	 */
	bool impact_on;

	/**
	 * \brief Whether the scope is activated (on).
	 */
	bool scope_on;
};

/*================================== inline implementations ========================================*/

inline Indicators::Indicators(int n) : impact(n), scope(n), impact_on(false), scope_on(false) {

}

} // namespace ibex
#endif // __IBEX_INDICATORS_H__
