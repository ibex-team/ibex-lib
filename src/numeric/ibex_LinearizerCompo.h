//============================================================================
//                                  I B E X
// File        : ibex_LinearizerCompo.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 17, 2019
//============================================================================

#ifndef __IBEX_LINEARIZER_COMPO_H__
#define __IBEX_LINEARIZER_COMPO_H__

#include "ibex_Linearizer.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief Composition of linearizations (logical AND).
 *
 * Allows to merge two linear programs in a single one.
 */
class LinearizerCompo : public Linearizer {
public:
	/**
	 * \brief Build a composition of two linearizers.
	 */
	LinearizerCompo(Linearizer& l1, Linearizer& l2);

	/**
	 * \brief Add constraints in a LP solver.
	 *
	 * Add the constraints of underlying linearizers.
	 *
	 * \return the number of constraints (possibly 0) or -1 if the linear system is
	 *         infeasible.
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver);

	/**
	 * \brief Add constraints in a LP solver.
	 *
	 * By default, call linearize(box, lp_solver).
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);

	/**
	 * \brief Add properties of underlying linearizers.
	 *
	 *
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	/**
	 * \brief Delete this.
	 */
	virtual ~LinearizerCompo();

protected:

	/**
	 * Linearizer n°1
	 */
	Linearizer& l1;

	/**
	 * Linearizer n°2
	 */
	Linearizer& l2;
};

} /* namespace ibex */

#endif /* __IBEX_LINEARIZER_COMPO_H__ */
