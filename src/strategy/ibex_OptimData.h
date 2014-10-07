//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimData.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#ifndef __IBEX_OPTIMDATA_H__
#define __IBEX_OPTIMDATA_H__

#include "ibex_Backtrackable.h"
#include "ibex_IntervalVector.h"

namespace ibex {

/** \ingroup strategy
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

	/** for the Casado criteria */
	/** the image of the objective on the current box */
	Interval pf;
	/** the constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied */
	double pu;

protected:

	OptimData(const OptimData&);
};

} // end namespace ibex
#endif // __IBEX_OPTIMDATA_H__
