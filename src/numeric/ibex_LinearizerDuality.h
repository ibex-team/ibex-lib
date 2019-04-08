//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerDuality.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 29, 2019
//============================================================================

#ifndef __IBEX_LINEARIZER_DUALITY_H__
#define __IBEX_LINEARIZER_DUALITY_H__

#include "ibex_Linearizer.h"
#include "ibex_NormalizedSystem.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief Duality-based linear restriction of a NLP.
 *
 * NOTE: one limitation of this implementation is that the number of variables
 *       is fixed to (m+1)*n where m is the total number of constraints, whereas
 *       m should be dynamically set to the number of active constraints only.
 *       However, with the current interface of LPSolver, the only way to change
 *       the number of variables is to delete and recreate a LPSolver object,
 *       which drastically slows down the optimizer.
 */
class LinearizerDuality : public Linearizer {
public:
	/**
	 * \brief Slope matrices computation formula.
	 *
	 * - TAYLOR: one gradient evaluation for all variables simultaneously
	 *           (less accurate but faster)
	 * - HANSEN: recursive evaluation with successive variable instantiation
	 *           (more accurate but longer)
	 */
	typedef enum  { TAYLOR, HANSEN } slope_formula;

	/**
	 * \brief Create the linear restriction.
	 */
	LinearizerDuality(const NormalizedSystem& sys, slope_formula slope=HANSEN);

	/**
	 * \brief Add the inequalities in the LP solver.
	 *
	 * The LP solver must be created with (m+1)*n variables.
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver);

	/**
	 * \brief Add the inequalities in the LP solver.
	 *
	 * The LP solver must be created with (m+1)*n variables.
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);

	/**
	 * \brief Return the point chosen in the last call to linearize(...)
	 *
	 * The size of the vector is n (number of variables of the original system).
	 *
	 * Undefined if linearize(...) has not been called yet
	 */
	const Vector& point() const;

	/**
	 * \brief The system
	 */
	const System& sys;

protected:
	/**
	 * \brief Slope matrix formula.
	 */
	slope_formula slope;

	/**
	 * \brief Chosen point
	 */
	Vector pt;
};

/*============================================ inline implementation ============================================ */

inline const Vector& LinearizerDuality::point() const {
	return pt;
}

} // namespace ibex

#endif // __IBEX_LINEARIZER_DUALITY_H__
