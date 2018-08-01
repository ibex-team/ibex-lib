//============================================================================
//                                  I B E X                                   
// File        : ibex_ExtendedSystem.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2013
// Last update : Aug 01, 2018
//============================================================================

#ifndef __IBEX_EXTENDED_SYSTEM_H__
#define __IBEX_EXTENDED_SYSTEM_H__

#include "ibex_NormalizedSystem.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief System where a goal is encoded as a constraint.
 *
 * <ul> The extended system contains:
 * <li> (n+1) variables, x_1,...x_n,y. The index of y is #goal_var (==n).
 * <li> A (m+1)-valued function f:(x,y)->(y-f(x),g_0(x),...,g_m(x))
 * <li> (m+1) constraints: y>=f(x), g_1(x)<=0, ..., g_m(x)<=0.
 * </ul>
 */
class ExtendedSystem : public NormalizedSystem {
public:

	/**
	 * \brief Normalize \a sys and encode the goal function as a constraint.
	 *
	 * The resulting system includes (n+1) variables, the n original
	 * variables x_1,...,x_n and an an additional "goal" variable y.
	 * The index of the goal variable is #goal_var(). It
	 * also includes (m+1) constraints. The index of the constraint y-goal(x)=0
	 * (where goal(x) is the goaoriginal_sys_id(sys.id), l function) is #goal_ctr(). The others constraints are
	 * normalized copy of the original ones. The name of the goal
	 * variable is #goal_name.
	 *),
	 * \param eps   Transforms an equation f=0 into two inequalities: f<=eps and -f<=eps.
	 *
	 * The goal of the extended system is "y".
	 */
	explicit ExtendedSystem(const System& sys, double eps=0);

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

	/**
	 * \brief Load a (n-dimensional) box into an (n+1-dimensional) extended box
	 *
	 *  The goal variable is skipped.
	 */
	void write_ext_box(const IntervalVector& box, IntervalVector& ext_box) const;

	/**
	 * \brief Load an extended (n+1-dimensional) box into a (n-dimensional) box
	 *
	 *  The goal variable is skipped.
	 */
	void read_ext_box(const IntervalVector& ext_box, IntervalVector& box) const;

	/**
	 * \brief Load a (n-dimensional) vector into an (n+1-dimensional) extended vector
	 *
	 *  The goal variable is skipped.
	 */
	void write_ext_vec(const Vector& x, Vector& xy) const;

	/**
	 * \brief Load an extended (n+1-dimensional) vector into a (n-dimensional) vector
	 *
	 *  The goal variable is skipped.
	 */
	void read_ext_vec(const Vector& xy, Vector& x) const;
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
