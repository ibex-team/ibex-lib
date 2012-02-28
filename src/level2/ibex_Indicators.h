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
 * \ingroup level2
 * \brief Indicators (Parameters of contractors based on a dictionary structure).
 */
class Indicators {

public:

	/**
	 * Creates a new indicators structure with
	 * nothing activated (all the indicators are "off")
	 */
	Indicators();

	/**
	 * Set whether the a variable has been impacted
	 * since the last call to the contractor.
	 */
	BoolMask& impact;

	/**
	 * Set whether the a variable has to be
	 * considered by the next call to the contractor.
	 */
	BoolMask& scope;

	/**
	 * Add the impact to the indicators
	 */
	void set_impact_on();

	/**
	 * Remove the impact from the indicators
	 */
	void set_impact_off();

	/**
	 * Add the scope to the indicators
	 */
	void set_scope_on();

	/**
	 * Remove the scope from the indicators
	 */
	void set_scope_off();

	/**
	 * True if the impact is activated
	 */
	bool impact_on() const;

	/**
	 * True if the scope is activated
	 */
	bool scope_on() const;


};

} // namespace ibex
#endif // __IBEX_INDICATORS_H__
