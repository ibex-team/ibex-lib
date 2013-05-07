//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFritzJohn.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 13, 2013
// Last Update : May 03, 2013
//============================================================================

#ifndef __IBEX_CTC_FRITZ_JOHN_H__
#define __IBEX_CTC_FRITZ_JOHN_H__

#include "ibex_Ctc.h"
#include "ibex_CtcHC4.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \brief Contractor based on Fritz-John conditions.
 *
 */
class CtcFritzJohn : public Ctc {
public:

	/**
	 * \brief Build the contractor for an optimization problem.
	 */
	CtcFritzJohn(const System& sys);

	~CtcFritzJohn();

	virtual void contract(IntervalVector& box);

	/**
	 * \brief Add Lagrange multipliers.
	 *
	 * See #ibex::Multipliers.
	 */
	void add_backtrackable(Cell& root);

	/** Fritz-John conditions. */
	System fritz;

	/** Contractor associated to \a "fritz". */
	CtcHC4 ctc;

	/** Number of basic variables */
	const int n;

	/** Number of constraints */
	const int m;

	/** Number of bound constraints */
	const int K;

	/** The current extended box, including multipliers. */
	IntervalVector ext_box;

	/** bound_ctr is an array representing the bounds of the initial box of the system
	 * that are less (in absolute value) than #ACTIVE_BOUND_CEIL.
	* bound_ctr[i][0] is the number of the variable and bound_ctr[i] is
	* 0 (resp 1) is this is the lower (resp. upper) bound.
	*/
	int (*bound_ctr)[2];

	/** */
	static const double ACTIVE_BOUND_CEIL;
};

} // end namespace ibex
#endif // __IBEX_CTC_FRITZ_JOHN_H__
