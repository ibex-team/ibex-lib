//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimData.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#ifndef __IBEX_OPTIM_DATA_H__
#define __IBEX_OPTIM_DATA_H__

#include "ibex_Backtrackable.h"
#include "ibex_Interval.h"
#include "ibex_EntailedCtr.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Data required for the Optimizer
 */
class OptimData : public Backtrackable {
public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	OptimData();

	/**
	 * \brief Delete *this.
	 */
	~OptimData();

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();

	/**
	 * \brief Initialize the value of "pf"
	 *
	 * \see #pf
	 */
	void compute_pf(Function& goal, const IntervalVector& box);

	/**
	 * \brief Initialize the value of "pu"
	 *
	 * \see #pu
	 */
	void compute_pu(System& sys, const IntervalVector& box, const EntailedCtr& entailed);

	/**
	 * \brief Casado criterion
	 *
	 * Image of the objective on the current box
	 */
	Interval pf;

	/**
	 * \brief Casado criterion
	 *
	 * Constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied.
	 */
	double pu;

protected:

	OptimData(const OptimData&);
};

} // end namespace ibex
#endif // __IBEX_OPTIM_DATA_H__
