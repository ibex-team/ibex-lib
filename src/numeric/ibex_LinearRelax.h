//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxation.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 30, 2013
// Last Update : Oct 30, 2013
//============================================================================

#ifndef __IBEX_LINEAR_RELAXATION_H__
#define __IBEX_LINEAR_RELAXATION_H__

#include "ibex_System.h"
#include "ibex_LinearSolver.h"

namespace ibex {

/**
 * \brief Linear relaxation
 *
 * This in an abstract class for all linear relaxation algorithm
 * of a nonlinear system.
 */
class LinearRelax {
public:
	LinearRelax(const System& sys);

	virtual ~LinearRelax();

	/**
	 * \brief The linearization technique.
	 *
	 * It must be implemented in the subclasses.
	 */
	virtual int linearization(IntervalVector& box, LinearSolver *mysolver) =0;

	/**
	 * Check if the constraint is satisfied in the box : in this case, no linear relaxation is made.
	 *
	 * TODO: is this function redundant?
	 */
	bool isInner(IntervalVector & box, const System& sys, int j);

	/**
	 * \brief The system linearized
	 */
	const System& sys;
};

} // end namespace ibex
#endif // __IBEX_LINEAR_RELAXATION_H__
