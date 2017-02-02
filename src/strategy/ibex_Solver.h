//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Feb 02, 2017
//============================================================================

#ifndef __IBEX_SOLVER_H__
#define __IBEX_SOLVER_H__

#include "ibex_Ctc.h"
#include "ibex_Pdc.h"
#include "ibex_Bsc.h"
#include "ibex_CellBuffer.h"
#include "ibex_SubPaving.h"
#include "ibex_Timer.h"
#include "ibex_Exception.h"

#include <vector>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief  Solver.
 *
 * This class implements an branch and prune algorithm that finds all the solutions of a systems of equations (the system may contain additional inequalities).
 *
 */


class CellLimitException : public Exception {} ;

class Solver {
public:
	/**
	 * \brief Build a solver.
	 *
	 * \param ctc    - The contractor (for contracting each node of the search tree)
	 * \param bsc    - The bisector   (for branching). Contains the stop criterion.
	 * \param buffer - The cell buffer (a CellStack in a depth first search strategy)
	 */
	Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer);

	/**
	 * \brief Build a solver with certification.
	 *
	 * \param sys    - The system to be solved
	 * \param ctc    - The contractor (for contracting each node of the search tree)
	 * \param bsc    - The bisector   (for branching). Contains the stop criterion.
	 * \param buffer - The cell buffer (a CellStack in a depth first search strategy)
	 */
	Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer);

	/**
	 * \brief Build a solver with certification.
	 *
	 * \param sys    - The system to be solved
	 * \param params - Force which dimensions correspond to parameters
	 * \param ctc    - The contractor (for contracting each node of the search tree)
	 * \param bsc    - The bisector   (for branching). Contains the stop criterion.
	 * \param buffer - The cell buffer (a CellStack in a depth first search strategy)
	 */
	Solver(const System& sys, const BitSet& params, Ctc& ctc, Bsc& bsc, CellBuffer& buffer);

	/**
	 * \brief Destructor.
	 */
	~Solver();

	/**
	 * \brief Solve the system (non-interactive mode).
	 *
	 * \param init_box - the initial box (the search space)
	 * 
	 * Return :the vector of solutions (small boxes with the required precision) found by the solver.
	 */
	std::vector<IntervalVector> solve(const IntervalVector& init_box);

	/**
	 * \brief Start solving (interactive mode).
	 *
	 * Can also be used to restart a new search.
	 */
	void start(const IntervalVector& init_box);

	/**
	 * Return type for the next(...) function.
	 *
	 * \see #next(std::vector<IntervalVector>&).
	 */
	typedef enum { SEARCH_OVER, NEW_SAFE, NEW_UNSAFE, SAFE, UNSAFE } sol_type;

	/**
	 * \brief Continue solving (interactive mode).
	 *
	 * Look for the next solution and push it into the vector.
	 *
	 * When the system is well-constrained, a "solution" is a box [x] such that
	 * there exists x in [x] f(x)=0.
	 *
	 * When the system is under-constrained, a "solution" is a box ([x],[p]) such that
	 * for all p in [p] there exists x in f(x,p)=0.
	 *
	 * A "new" solution is a solution that is proven to have an empty intersection with all
	 * previously found solutions. For efficiency reason, this test is not performed in the
	 * case of under-constrained systems.
	 *
	 * If certification is not required, the return type is always UNSAFE.
	 *
	 * \return
	 *          NEW_SAFE:        The box is proven to contain a new solution.
	 *          NEW_UNSAFE:      The box may contain a solution and if it does, this is a new solution.
	 *          SAFE:            The box is proven to contain a solution but that may have already been found earlier.
	 *          UNSAFE:          The box may contain a solution and if it does, the solution may have already been found earlier;
	 *          SEARCH_OVER:     There is no more solution.
	 */
	sol_type next(std::vector<IntervalVector>& sols);

	/**
	 * \brief The contractor.
	 *
	 * Contractor used by the solver for contracting the current box at each node:
	 * generally, a sequence (with or without fixpoint) of different contractors (HC4,
	 * Acid, Newton, a linear relaxation, ... )
	 *
	 */ 
	Ctc& ctc;

	/** Bisector (tests also precision of boxes). */
	Bsc& bsc;

	/** Cell buffer. */
	CellBuffer& buffer;

	/** Maximum cpu time used by the solver.
	 * This parameter allows to bound time complexity.
	 * The value can be fixed by the user. By default, it is -1 (no limit). */

	double time_limit;


	/** Maximal number of cells created by the solver.
	 * This parameter allows to bound the number of nodes in the search tree.
	 * The value can be fixed by the user. By default, it is -1 (no limit). */
	long cell_limit;

	/**
	 * \brief Trace level
	 *
	 *  the trace level. 
	 *  0  : no trace  (default value)
	 *  1   the solutions are printed each time a new solution is found
	 *  2   the solutions are printed each time a new solution is found and the current box is printed at each node of the branch and prune algorithm 
	 */
	int trace;

	/** Number of nodes in the search tree */
	int nb_cells;

	/** Remember running time of the last exploration */
	double time;

protected :

	void init(const System& sys, const BitSet* params);

	void time_limit_check();

	/*
	 * Return false if the new solution "x" is surely not a new solution.
	 * If it returns true, a more precise status of the solution is given in "type".
	 * If the status is SAFE or NEW_SAFE the box "x" may have slightly changed (due
	 * to inflating Newton) and the actual solution is stored at the end of "solve_solutions".
	 * So, in any case the new solution (safe or not) should be read from "solve_solutions".
	 */
	bool check_sol(IntervalVector& x, sol_type& type);


	BitSet impact;

	/*
	 * Initial box of the current search.
	 */
	IntervalVector solve_init_box;

	/*
	 * The equations to be solved. Only used if certification required.
	 */
	const System* eqs;

	/*
	 * The inequalities to be solved. Only used if certification required.
	 */
	const System* ineqs;

	/**
	 * The forced parameters (if any). Only used if certification required.
	 */
	const BitSet* params;

	/*
	 * Solutions found in the current search.
	 *
	 * Each solution is represented by an existence (first) and unicity (second) box.
	 */
	std::vector<std::pair<IntervalVector,IntervalVector> > solve_solutions;

};

} // end namespace ibex
#endif // __IBEX_SOLVER_H__
