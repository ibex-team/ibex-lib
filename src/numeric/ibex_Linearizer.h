//============================================================================
//                                  I B E X
// File        : ibex_Linearizer.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2017
//============================================================================

#ifndef __IBEX_LINEARIZER_H__
#define __IBEX_LINEARIZER_H__

#include "ibex_IntervalVector.h"
#include "ibex_LPSolver.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief Linearization method.
 *
 * This in an abstract class for all linear approximations
 * (relaxations or restrictions) of nonlinear systems.
 */
class Linearizer {
public:
	/**
	 * \brief Build a linearizer on nb_var variables.
	 */
	Linearizer(int nb_var);

	/**
	 * \brief Add constraints in a LP solver.
	 *
	 * The constraints correspond to a linearization of the
	 * underlying system, on the given box.
	 *
	 * This function must be implemented in the subclasses.
	 *
	 * \return the number of constraints (possibly 0) or -1 if the linear system is
	 *         infeasible.
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver)=0;

	/**
	 * \brief Delete this.
	 */
	virtual ~Linearizer();

	/**
	 * \brief Number of variables
	 */
	int nb_var() const;

protected:

	/**
	 * Number of variables
	 */
	const int n;
};

/*================================== inline implementations ========================================*/

inline int Linearizer::nb_var() const {
	return n;
}

} /* namespace ibex */

#endif /* __IBEX_LINEARIZER_H__ */
