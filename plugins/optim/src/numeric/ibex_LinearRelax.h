//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelax.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 30, 2013
// Last Update : Jun 22, 2017
//============================================================================

#ifndef __IBEX_LINEAR_RELAXATION_H__
#define __IBEX_LINEAR_RELAXATION_H__

#include "ibex_LinearFactory.h"

namespace ibex {

/**
 * \brief Linear relaxation
 *
 * This in an abstract class for all linear relaxation algorithm
 * of a nonlinear system.
 */

class LinearRelax : public LinearFactory {
public:

	/**
	 * Return -1 if the underlying system is
	 * infeasible.
	 */
	LinearRelax(int nb_var) : LinearFactory(nb_var) { }
};


//class LinearRelax {
//public:
//	/**
//	 * \brief Build a relaxation for a system.
//	 *
//	 * The system is only used by this constructor to get the number
//	 * of variables and constraints.
//	 */
//	LinearRelax(const System& sys);
//
//	/**
//	 * \brief Build a relaxation of nb_ctr constraints on nb_var variables.
//	 */
//	LinearRelax(int nb_ctr, int nb_var, int goal_var=-1);
//
//	virtual ~LinearRelax();
//
//	/**
//	 * \brief The linearization technique.
//	 *
//	 * It must be implemented in the subclasses.
//	 *
//	 * \return the number of constraints (possibly 0) or -1 if the system is
//	 *         infeasible.
//	 */
//	virtual int linearization(const IntervalVector& box, LinearSolver& lp_solver)=0;
//
//	/**
//	 * \brief Generation of a linear approximation of the inner region and add it in the Linear Program
//	 *
//	 * It must be implemented in the subclasses.
//	 */
//	virtual int inlinearization(const IntervalVector& box, LinearSolver& lp_solver)=0;
//
//	/**
//	 * \brief Generation of a linear approximation of the linear objective function
//	 *
//	 * It must be implemented in the subclasses.
//	 */
//	//virtual bool goal_linearization(const IntervalVector& box, LinearSolver& lp_solver)=0;
//

//
//	/**
//	 * \brief (Maximal?) number of constraints
//	 */
//	int nb_ctr() const;
//
//	/**
//	 * \brief Goal variable, if any. -1 otherwise.
//	 */
//	int goal_var() const;

//};
//

//inline int LinearRelax::nb_ctr() const {
//	return _nb_ctr;
//}
//
//inline int LinearRelax::goal_var() const {
//	return _goal_var;
//}


} // end namespace ibex
#endif // __IBEX_LINEAR_RELAXATION_H__
