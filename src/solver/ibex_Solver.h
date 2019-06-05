//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Oct 01, 2017
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
#include "ibex_Linear.h"
#include "ibex_CovSolverData.h"

#include <vector>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Solver.
 *
 * This class implements a branch and prune algorithm that looks for
 * all the solutions of a systems of equalities and/or inequalities.
 *
 */

class CellLimitException : public Exception {} ;

class CovSolverData;
class CovSolverDataFactory;

class Solver {
public:
	/**
	 * \brief Return status of the solver
	 *
	 * See comments for solve(...) below.
	 */
	typedef enum { SUCCESS, INFEASIBLE, NOT_ALL_VALIDATED, TIME_OUT, CELL_OVERFLOW } Status;

	/**
	 * \brief Boundary test strength
	 *
	 * ALL_TRUE:  by default for under constrained problems (0<m<n).
	 * FULL_RANK: the gradients of all constraints (equalities and
	 *            potentially activated inequalities) must be linearly independent.
	 * HALF_BALL: **not implemented yet** the intersection of the box and the
	 *            solution set is homeomorphic to a half-ball of R^n
	 * ALL_FALSE: by default if m=0 or m=n (inequalities only/square systems)
	 */
	typedef enum { ALL_TRUE, FULL_RANK, HALF_BALL, ALL_FALSE } boundary_test_strength;

	/**
	 * \brief Build a solver with certification.
	 *
	 * \param sys     - The system to be solved
	 * \param ctc     - The contractor (for contracting each node of the search tree)
	 * \param bsc     - The bisector   (for branching).
	 * \param buffer  - The cell buffer (a CellStack in a depth first search strategy)
	 * \param eps_min - Criterion to stop bisection
	 * \param eps_max - Criterion to force bisection
	 */
	Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
			const Vector& eps_min, const Vector& eps_max);

	/**
	 * \brief Force some dimensions to be parameters in the automatic proof.
	 *
	 * Note: not necessarily n-m bits have to be set.
	 */
	void set_params(const VarSet& params);

	/**
	 * \brief Destructor.
	 */
	~Solver();

	/**
	 * \brief Solve the system (non-interactive mode).
	 *
	 * \param init_box - the initial box (the search space)
	 * 
	 * \return Possible values:
	 *
	 *   SUCCESS:           (complete search) all solutions found and all
	 *                      output boxes validated.
	 *
	 *   INFEASIBLE:        (complete search) infeasible problem.
	 *
	 *   NOT_ALL_VALIDATED: (incomplete search) a non-validated box
	 *                      reaching the minimal width (eps-min)
	 *                      has been found.
	 *
	 *   TIME_OUT:          (incomplete search) time out
	 *
	 *   CELL_OVERFLOW:     (incomplete search) cell overflow : the number of
	 *                      cell has exceeded the limit.
	 *
	 * The vector of "solutions" (output boxes) found by the solver
	 * are retrieved with #get_solutions().
	 */
	Status solve(const IntervalVector& init_box);

	/**
	 * \brief Continue solving of the system.
	 *
	 * \param cov - COV data structure containing the input paving;
	 */
	Status solve(const CovSolverData& cov);

	/**
	 * \brief Continue solving of the system.
	 *
	 * \param filename - Name of the file containing the input paving;
	 */
	Status solve(const char* filename);

	/**
	 * \brief Solve the system
	 *
	 * Continue exploration from an initial paving.
	 */
	//Status solve(const char* init_paving);

	/**
	 * \brief Start solving (interactive mode).
	 *
	 * Can also be used to restart a new search.
	 */
	void start(const IntervalVector& init_box);

	/**
	 * \brief Start solving (interactive mode).
	 *
	 * Can also be used to restart a new search.
	 */
	void start(const CovSolverData& data);

	/**
	 * \brief Start solving (interactive mode).
	 *
	 * Can also be used to restart a new search.
	 */
	void start(const char* input_paving);

	/**
	 * \brief Find the next covering box (interactive mode).
	 *
	 * The next box is either a new solution, a boundary or an unknown box.
	 *
	 * \return true if a new covering box is found.
	 *
	 * \param status - status of the new box found. Undefined if an exception is
	 *               thrown, or the search is over.
	 * \param sol - (output argument) pointer to the new box. This parameter is
	 *              ignored if set to NULL (default value). Otherwise, in return, *sol
	 *              is the address of the last element added in the CovSolverData
	 *              structure.
	 *              *sol is set to NULL if search is over, time is out or the number
	 *              of cells exceeds the limit.
	 *
	 * \throw CellLimitException or TimeOutException
	 */
	bool next(CovSolverData::BoxStatus& box_status, const IntervalVector** sol=NULL);

	/**
	 * \brief Displays on standard output a report of the last call to solve(...).
	 */
	void report();

	/**
	 * \brief Get the "solutions" (output boxes).
	 *
	 * \return the output boxes of the last call to solve(...).
	 */
	const CovSolverData& get_data() const;

	/**
	 * \brief Get the time spent.
	 *
	 * \return the total CPU time of last call to solve(...).
	 */
	double get_time() const;

	/**
	 * \brief Get the number of cells.
	 *
	 * \return the number of cells generated by the last call to solve(...).
	 */
	double get_nb_cells() const;
	/**
	 * \brief The contractor.
	 *
	 * Contractor used by the solver for contracting the current box at each node:
	 * generally, a sequence (with or without fixpoint) of different contractors (HC4,
	 * Acid, Newton, a linear relaxation, ... )
	 *
	 */ 
	Ctc& ctc;

	/**
	 * \brief The bisector.
	 */
	Bsc& bsc;

	/**
	 * \brief Cell buffer.
	 */
	CellBuffer& buffer;

	/**
	 * \brief Minimal width of boxes (criterion to stop bisection)
	 */
	const Vector eps_x_min;

	/**
	 * \brief Maximal width of boxes (criterion to force bisection)
	 */
	const Vector eps_x_max;

	/**
	 * \brief Boundary test strength
	 */
	boundary_test_strength boundary_test;

	/**
	 * \brief Maximum CPU time used by the solver.
	 *
	 * This parameter allows to bound running time.
	 * The value can be fixed by the user. By default, it is -1 (no limit).
	 */

	double time_limit;

	/**
	 * \brief Maximal number of cells created by the solver.
	 *
	 * This parameter allows to bound the number of nodes in the search tree.
	 * The value can be fixed by the user. By default, it is -1 (no limit).
	 */
	long cell_limit;

	/**
	 * \brief Trace level
	 *
	 *  Possible values:
	 *  0  - No trace  (default value)
	 *  1  - Each time a new "solution" is found, it is printed;
	 *  2  - At each node of the search, the current box is printed and
	 *       each time a new solution is found, it is printed.
	 */
	int trace;


protected:
	/**
	 * \brief Call "next" until search is over.
	 */
	Status solve();

	/*
	 * \brief Return a new "output box" that potentially contains solutions.
	 * \throw An exception otherwise (no solution inside).
	 *
	 * \param box - input box
	 * \param sol - output box.
	 *
	 * If the status of the return box is INNER, the box may have
	 * slightly changed (due to inflating Newton) and the actual "solution"
	 * is stored in the existence box of the output.
	 */
	CovSolverData::BoxStatus check_sol(const IntervalVector& box);

	/**
	 * \brief Check if the box is "BOUNDARY"
	 * \see SolverOutputBox.
	 */
	bool is_boundary(const IntervalVector& box);

	/**
	 * \brief True if width(box)>eps_x_max.
	 */
	bool is_too_large(const IntervalVector& box);

	/**
	 * \brief True if width(box)>eps_x_min.
	 */
	bool is_too_small(const IntervalVector& box);

	bool check_ineq(const IntervalVector& box);

	/**
	 * \brief Check if time is out.
	 */
	void time_limit_check();

	/**
	 * \brief Flush
	 */
	void flush();

	/*
	 * \brief Initial box of the current search.
	 */
	IntervalVector solve_init_box;

	/**
	 * \brief Number of variables
	 */
	int n;

	/**
	 * \brief Number of equalities.
	 */
	int m;

	/**
	 * \brief Number of inequalities.
	 */
	int nb_ineq;

	/*
	 * \brief The equalities to be solved (NULL if none).
	 */
	const System* eqs;

	/*
	 * \brief The inequalities to be solved (NULL if none).
	 */
	const System* ineqs;

	/**
	 * \brief The forced parameters (if any, NULL otherwise).
	 */
	VarSet params;

	/*
	 * \brief Solutions found in the current search.
	 */
	CovSolverData* manif;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 */
	double time;

	/*
	 * \brief CPU running time of the previous call.
	 */
	double old_time;

	Timer timer;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 */
	unsigned int nb_cells;

	/**
	 * \brief Number of cells of the previous call.
	 */
	unsigned int old_nb_cells;
};

/*============================================ inline implementation ============================================ */

inline const CovSolverData& Solver::get_data() const {
	return *manif;
}

inline double Solver::get_time() const {
	return get_data().time();
}

inline double Solver::get_nb_cells() const {
	return get_data().nb_cells();
}

} // end namespace ibex

#endif // __IBEX_SOLVER_H__
