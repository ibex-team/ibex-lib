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
	/**
	 * \brief Build a relaxation for a system.
	 *
	 * The system is only used by this constructor to get the number
	 * of variables and constraints.
	 */
	LinearRelax(const System& sys);

	/**
	 * \brief Build a relaxation of nb_ctr constraints on nb_var variables.
	 */
	LinearRelax(int nb_ctr, int nb_var, int goal_var=-1);

	virtual ~LinearRelax();

	/**
	 * \brief The linearization technique.
	 *
	 * It must be implemented in the subclasses.
	 *
	 * \return the number of constraints (possibly 0) or -1 if the system is
	 *         infeasible.
	 */
	virtual int linearization(const IntervalVector& box, LinearSolver& lp_solver)=0;

	/**
	 * \brief Generation of a linear approximation of the inner region and add it in the Linear Program
	 *
	 * It must be implemented in the subclasses.
	 */
	virtual int inlinearization(const IntervalVector& box, LinearSolver& lp_solver)=0;

	/**
	 * \brief Generation of a linear approximation of the linear objective function
	 *
	 * It must be implemented in the subclasses.
	 */
	virtual bool goal_linearization(const IntervalVector& box, LinearSolver& lp_solver)=0;

	/**
	 * Check if the constraint is satisfied in the box : in this case, no linear relaxation is made.
	 *
	 * TODO: is this function redundant?
	 */
	bool isInner(const IntervalVector& box, const System& sys, int j);

	/**
	 * \brief Number of variables
	 */
	int nb_var() const;

	/**
	 * \brief (Maximal?) number of constraints
	 */
	int nb_ctr() const;

	/**
	 * \brief Goal variable, if any. -1 otherwise.
	 */
	int goal_var() const;

private:
	int _nb_ctr;
	int _nb_var;
	int _goal_var;

};


/*================================== inline implementations ========================================*/

inline int LinearRelax::nb_var() const {
	return _nb_var;
}

inline int LinearRelax::nb_ctr() const {
	return _nb_ctr;
}

inline int LinearRelax::goal_var() const {
	return _goal_var;
}


} // end namespace ibex
#endif // __IBEX_LINEAR_RELAXATION_H__
