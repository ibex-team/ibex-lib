//============================================================================
//                                  I B E X                                   
// File        : ibex_ExtendedSystem.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2013
// Last Update : Jun 26, 2013
//============================================================================

#ifndef __IBEX_EXTENDED_SYSTEM_H__
#define __IBEX_EXTENDED_SYSTEM_H__

#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief System where a goal is encoded as a constraint.
 */
class ExtendedSystem : public System {
public:

	/**
	 * \brief Normalize \a sys and encode the goal function as a constraint.
	 *
	 * The resulting system includes (n+1) variables, the n original
	 * variables x_1,...,x_n and an an additional "goal" variable y.
	 * The index of the goal variable is #goal_var(). It
	 * also includes (m+1) constraints. The index of the constraint y-goal(x)=0
	 * (where goal(x) is the goal function) is #goal_ctr(). The others constraints are
	 * normalized copy of the original ones. The name of the goal
	 * variable is #goal_name.
	 *
	 * \param eps   Transforms an equation f=0 into two inequalities: f<=eps and -f<=eps.
	 *
	 * To avoid confusion, the extended system has no goal (but it could be "y").
	 */
	ExtendedSystem(const System& sys, double eps=0);

	/**
	 * \brief Name of the goal variable ("y").
	 */
	static const char* goal_name();

	/**
	 * \return The number of the goal variable.
	 */
	int goal_var() const;

	/**
	 * \return The number of the goal constraint.
	 */
	int goal_ctr() const;

	/*
	 * \brief Goal of the original system.
	 */
	//Function original_goal;

};

/*================================== inline implementations ========================================*/

inline int ExtendedSystem::goal_var() const {
	return nb_var-1;
}

inline int ExtendedSystem::goal_ctr() const {
	return 0;
}

} // end namespace ibex

#endif // __IBEX_EXTENDED_SYSTEM_H__
