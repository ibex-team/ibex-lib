//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultSolver.h
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 27, 2012
// Last Update : Jul 06, 2014
//============================================================================

#ifndef __IBEX_DEFAULT_SOLVER_H__
#define __IBEX_DEFAULT_SOLVER_H__

#include "ibex_Solver.h"
#include "ibex_System.h"
#include "ibex_CellBuffer.h"
#include "ibex_CtcCompo.h"
#include "ibex_Memory.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Default solver.
 */
class DefaultSolver : private Memory, public Solver {
public:
	/**
	 * \brief Create a default solver.
	 *
	 * \param sys  - The system to solve
	 * \param eps_x_min - Criterion for stopping bisection (absolute precision)
	 * \param eps_x_max - Criterion for forcing bisection  (absolute precision)
	 * \param dfs       - true: depth-first search. false: breadth-first search
	 */
    DefaultSolver(System& sys, double eps_x_min=default_eps_x_min, double eps_x_max=default_eps_x_max, bool dfs=true, double random_seed=default_random_seed);

    /**
	 * \brief Create a default solver.
	 *
	 * \param sys  - The system to solve
	 * \param eps_x_min - Criterion for stopping bisection (vector of absolute
	 *                    precisions, one for each variable)
	 * \param eps_x_max - Criterion for forcing bisection  (absolute precision)
	 * \param dfs       - true: depth-first search. false: breadth-first search
	 */
    DefaultSolver(System& sys, const Vector& eps_x_min, double eps_x_max=default_eps_x_max, bool dfs=true, double random_seed=default_random_seed);

	/**
	 * \brief Default minimal width: 1e-6.
	 */
	static constexpr double default_eps_x_min = 1e-3;

	/**
	 * \brief Default maximal width: +oo.
	 */
	
	// TODO: constexpr definition not possible with FILIB (POS_INFINITY is not
	// a constexpr with FILIB). 
	//static constexpr double default_eps_x_max = POS_INFINITY;
	static double default_eps_x_max;
	
	/**
	 * Default random seed: 1.0.
	 */
	static constexpr double default_random_seed = 1.0;

	System& sys;

private:

	/**
	 * The contractor: hc4 + acid(hc4) + newton (if the system is square) + xnewton
	 */
	Ctc* ctc(System& sys, double prec);

//	std::vector<CtcXNewton::corner_point>* default_corners ();

};

} // end namespace ibex

#endif // __IBEX_DEFAULT_SOLVER_H__
