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
	 * Solution type.
	 *
	 * Return type for the next(...) function.
	 *
	 * \see #next(std::vector<IntervalVector>&).
	 */
	typedef enum { UNKNOWN=0, SOLUTION=1 } sol_status;

	/**
	 * \brief Solution of a system.
	 *
	 * When the system is well-constrained, a "solution" is a box [x] such that
	 * there exists x in [x] f(x)=0 and g(x)<=0.
	 *
	 * The status is SOLUTION if the previous property is proven, UNKNOWN otherwise.
	 *
	 * When the system is under-constrained, a "solution" is a box ([x],[p]) such that
	 * for all p in [p] there exists x in f(x,p)=0 and g(x,p)<=0. It may be a large box
	 * (compared to the precision). The "varset" structure indicates which components
	 * correspond to x and p. It is NULL in case of well-constrained systems (no
	 * parameters) or if m=0 (all parameters).
	 *
	 * The status is SOLUTION if the previous property is proven, UNKNOWN otherwise.
	 *
	 * If certification is not required, the status of the solution is always UNKNOWN.
	 */
	class Solution {
	public:
		 /*
		  * \brief Status of the "solution".
		  *
		  * SOLUTION: The box is proven to contain a solution.
		  * UNKNOWN:  The box may contain a solution.
		 */
		const sol_status status;

		/**
		 * \brief Existence box.
		 *
		 * If the status is SOLUTION, represents the smallest box found
		 * enclosing a solution.
		 * Otherwise, represents the "unknown" box.
		 */
		const IntervalVector& existence() const;

		/**
		 * \brief Unicity box.
		 *
		 * If the status is SOLUTION, represents the largest superset
		 * of existence() found such that the solution enclosed is unique.
		 * Otherwise, represents the "unknown" box.
		 */
		const IntervalVector& unicity() const;

		/**
		 * \brief Variable/Parameter structure.
		 *
		 * Structure used to certify the box.
		 *
		 * This field is NULL in the following cases:
		 *   - the system is well-constrained (all are variables)
		 *   - the system has no equalities (all are parameters)
		 *   - the set of parameters has been fixed by the user
		 *     (same parameters for all solutions)
		 *   - the status is UNKNOWN
		 */
		const VarSet* varset;

		/**
		 * \brief Duplicate the solution
		 */
		Solution(const Solution& sol);

		/**
		 * \brief Assignment
		 */
		Solution& operator=(const Solution&);

		/**
		 * \brief Destructor.
		 */
		~Solution();

	private:
		friend class Solver;

		Solution(int n);

		IntervalVector _existence;
		IntervalVector* _unicity; // NULL if status=UNKNOWN or m=0
	};

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
	 * \brief Continue solving (interactive mode).
	 *
	 * Look for the next solution and push it into the vector.
	 *
	 * \return true iff a new solution has been found (false means that the search is over).
	 */
	bool next(std::vector<IntervalVector>& sols);

	/**
	 * \brief Find the next solution.
	 *
	 * \param sol - (output argument) pointer to the new solution (if found). This
	 *              is just the address of the last element in the "solutions" vector.
	 *              Set to NULL if search is over.
	 *
	 * \return true iff a new solution has been found (false means that the search is over).
	 */
	bool next(const Solution*& sol);

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
	 *
	 * Tests also precision of boxes.
	 */
	Bsc& bsc;

	/**
	 * \brief Cell buffer.
	 */
	CellBuffer& buffer;

	/**
	 * \brief Maximum cpu time used by the solver.
	 *
	 * This parameter allows to bound time complexity.
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
	 *  1  - Each time a new solution is found, it is printed;
	 *  2  - At each node of the search, the current box is printed and
	 *       each time a new solution is found, it is printed.
	 */
	int trace;

	/**
	 * \brief Number of nodes in the current search.
	 */
	int nb_cells;

	/**
	 * \brief Running time of the current search.
	 */
	double time;

	/*
	 * \brief Solutions found in the current search.
	 */
	std::vector<Solution> solutions;

protected:

	/**
	 * Whether certification is required or not.
	 */
	bool certification();

	/**
	 * Called by constructors.
	 */
	void init(const System& sys, const BitSet* params);

	/*
	 * Return true if the box "x" may contain a new solution, false otherwise.
	 *
	 * \param x   - (input) Candidate box
	 * \param sol - (output) Solution. Only built if return value is true.
	 *
	 * If it returns true and the status of "sol" is SOLUTION, the box "x" may have
	 * slightly changed (due to inflating Newton) and the actual solution
	 * is stored as the existence box of "sol".
	 */
	bool check_sol(IntervalVector& x, Solution& sol);

	/**
	 * Store the solution in "solutions" and print it (if trace>=0).
	 */
	void store_sol(const Solution& sol);

	void time_limit_check();


	BitSet impact;

	/*
	 * Initial box of the current search.
	 */
	IntervalVector solve_init_box;

	/**
	 * Number of variables
	 */
	int n;

	/**
	 * Number of equalities. Only used if certification required (-1 otherwise)
	 */
	int m;

	/*
	 * The equalities to be solved. Only used if certification required.
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

};

/*============================================ inline implementation ============================================ */

inline Solver::Solution::Solution(int n) : status(UNKNOWN), varset(NULL), _existence(n), _unicity(NULL) {

}

inline Solver::Solution::Solution(const Solution& sol) : status(sol.status),
		varset(sol.varset? new VarSet(*sol.varset) : NULL), _existence(sol._existence),
		_unicity(sol._unicity? new IntervalVector(*sol._unicity) : NULL) {

}

inline Solver::Solution& Solver::Solution::operator=(const Solution& sol) {
	(sol_status&) status=sol.status;
	if (varset) delete varset;
	varset=sol.varset? new VarSet(*sol.varset) : NULL;
	_existence=sol._existence;
	if (_unicity) delete _unicity;
	_unicity=sol._unicity? new IntervalVector(*sol._unicity) : NULL;
	return *this;
}

inline Solver::Solution::~Solution() {
	if (_unicity) delete _unicity;
	if (varset) delete varset;
}

inline const IntervalVector& Solver::Solution::existence() const {
	return _existence;
}

inline const IntervalVector& Solver::Solution::unicity() const {
	return _unicity? *_unicity : _existence;
}

inline bool Solver::certification() {
	return m!=-1;
}

inline bool Solver::next(std::vector<IntervalVector>& sols) {
	const Solution* sol;
	bool res=next(sol);
	if (res) sols.push_back(sol->existence());
	return res;
}

} // end namespace ibex

#endif // __IBEX_SOLVER_H__
