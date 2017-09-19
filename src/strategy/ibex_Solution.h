//============================================================================
//                                  I B E X
// File        : ibex_Solution.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last update : Sep 19, 2017
//============================================================================

#ifndef __IBEX_SOLUTION_H__
#define __IBEX_SOLUTION_H__

#include "ibex_IntervalVector.h"
#include "ibex_VarSet.h"

namespace ibex {

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
	/**
	 * Solution type.
	 *
	 * Return type for the next(...) function.
	 *
	 * \see #next(std::vector<IntervalVector>&).
	 */
	typedef enum { UNKNOWN=0, SOLUTION=1 } sol_status;

	 /*
	  * \brief Status of the "solution".
	  *
	  * SOLUTION: The box is proven to contain a solution.
	  * UNKNOWN:  The box may contain a solution.
	 */
	const sol_status status;

	/**
	 * \brief Get the solution box.
	 *
	 * Corresponds to the existence box.
	 */
	operator const IntervalVector&() const;

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
 * \brief Print the solution
 *
 * Print its status and the parameters/variables
 * selected.
 */
std::ostream& operator<<(std::ostream& os, const Solution& sol);


/*============================================ inline implementation ============================================ */

inline Solution::Solution(int n) : status(UNKNOWN), varset(NULL), _existence(n), _unicity(NULL) {

}

inline Solution::Solution(const Solution& sol) : status(sol.status),
		varset(sol.varset? new VarSet(*sol.varset) : NULL), _existence(sol._existence),
		_unicity(sol._unicity? new IntervalVector(*sol._unicity) : NULL) {

}

inline Solution& Solution::operator=(const Solution& sol) {
	(sol_status&) status=sol.status;
	if (varset) delete varset;
	varset=sol.varset? new VarSet(*sol.varset) : NULL;
	_existence=sol._existence;
	if (_unicity) delete _unicity;
	_unicity=sol._unicity? new IntervalVector(*sol._unicity) : NULL;
	return *this;
}

inline Solution::~Solution() {
	if (_unicity) delete _unicity;
	if (varset) delete varset;
}

inline Solution::operator const IntervalVector&() const {
	return existence();
}

inline const IntervalVector& Solution::existence() const {
	return _existence;
}

inline const IntervalVector& Solution::unicity() const {
	return _unicity? *_unicity : _existence;
}

} /* namespace ibex */

#endif /* __IBEX_SOLUTION_H__ */
