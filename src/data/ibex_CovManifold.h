//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 24, 2018
//============================================================================

#ifndef __IBEX_COV_MANIFOLD_H__
#define __IBEX_COV_MANIFOLD_H__

#include "ibex_CovIBUList.h"
#include "ibex_VarSet.h"

namespace ibex {


/**
 * \ingroup strategy
 *
 * \brief Qualified box (calculated by strategies like ibexsolve/ibexopt).
 *
 * Given a system of m equalities f(x)=0 and inequalities g(x)<=0:
 *
 * A qualified box ([p],[x]) is INNER only if
 *
 *     for all x in [x], for all p in [p]
 *     g(x,p)<=0                                      (1)
 *
 *     for all p in [p] there exists x in [x]
 *     f(x,p)=0                                       (2)
 *
 * When the system is well-constrained (m=n), (2) boils down to:
 *
 *     there exists x in [x], f(x)=0
 *
 * so that the qualified box is a box containing a solution, according
 * to the usual meaning.
 *
 * A qualified box ([p],[x]) is BOUNDARY only if (2) holds and the manifold
 * f=0 crosses the inequalities in a "regular" way, the exact definition
 * of "regular" depending on the boundary_test flag of the solver.
 * See #Solver::boundary_test_strength.
 *
 * When the system is under-constrained, the "solution" box ([x],[p])" may be a
 * large box (compared to eps-min). The "varset" structure indicates which
 * components correspond to x and p. It is NULL in case of well-constrained
 * systems (no parameters) or if m=0 (all parameters).

 * The status is UNKNOWN if the box has been processed (precision eps-min reached)
 * but nothing could be proven.
 *
 * The status is PENDING if the box has not been processed (the search has been
 * interrupted because of a timeout/memory overflow).
 *
 */
class CovManifold : public CovIBUList {
public:

	/**
	 * \brief Possible status of a qualified box.
	 *
	 * See above.
	 */
	typedef enum { INNER, SOLUTION, BOUNDARY, UNKNOWN } BoxStatus;

	/**
	 * \brief Create a CovManifold.
	 */
	CovManifold(size_t n, size_t m, size_t nb_ineq=0);

	CovManifold(const char* filename);

	virtual ~CovManifold();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	virtual void add(const IntervalVector& x);

	virtual void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	virtual void add_boundary(const IntervalVector& x);

	void add_solution(const IntervalVector& existence);

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity);

	void add_solution(const IntervalVector& existence, const VarSet& varset);

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	BoxStatus status(int i) const;

	bool is_solution(int i) const;

	bool is_boundary(int i) const;

	/**
	 * \brief Existence box.
	 *
	 * Represents the smallest box found enclosing the manifold.
	 */
	const IntervalVector& solution(int j) const;

	/**
	 * \brief Unicity box.
	 *
	 * Represents the largest superset of solution(j) found such
	 * that the solution enclosed is unique.
	 */
	const IntervalVector& unicity(int j) const;

	/**
	 * \brief Certificate of the jth solution.
	 *
	 * Variable/Parameter structure used to certify the box.
	 */
	const VarSet& varset(int j) const;

	const IntervalVector& boundary(int j) const;

	/**
	 * \brief Number of equalities.
	 *
	 * By default : 0.
	 */
	const unsigned int m;

	/**
	 * \brief Number of inequalities.
	 *
	 * By default : 0.
	 */
	const size_t nb_ineq;

	/**
	 * \brief Number of solutions.
	 */
	size_t nb_solution() const;

	/**
	 * \brief Number of boundary boxes.
	 */
	size_t nb_boundary() const;

	/**
	 * \brief Display the format of a CovManifold file.
	 */
	static string format();

protected:

	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovManifold& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovManifold into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovManifold& cov, std::stack<unsigned int>& format_seq);

	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);

	/* read parameters of the parametric proof */
	static VarSet read_varset(std::ifstream& f, size_t n, size_t m);

	/* write parameters of the parametric proof */
	static void write_varset(std::ofstream& f, const VarSet& varset);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

	std::vector<BoxStatus>        _manifold_status;    // status of the ith box
	std::vector<IntervalVector*>  _manifold_solution;  // pointer to 'solution' boxes
	std::vector<IntervalVector*>  _manifold_boundary;  // pointer to 'boundary' boxes
	std::vector<IntervalVector>   _manifold_unicity;   // all the unicity boxes

	// in the special case where m=n, there is only one
	// possible varset, so a unique varset is stored:
	// _varset[0][0].
	std::vector<VarSet>          _manifold_varset;
};

std::ostream& operator<<(std::ostream& os, const CovManifold& manif);

inline CovManifold::BoxStatus CovManifold::status(int i) const {
	return _manifold_status[i];
}

inline bool CovManifold::is_solution(int i) const {
	return status(i)==SOLUTION;
}

inline bool CovManifold::is_boundary(int i) const {
	return status(i)==BOUNDARY;
}

inline const IntervalVector& CovManifold::solution(int i) const {
	return *_manifold_solution[i];
}

inline const IntervalVector& CovManifold::boundary(int i) const {
	return *_manifold_boundary[i];
}

inline const IntervalVector& CovManifold::unicity(int j) const {
	return _manifold_unicity[j];
}

inline const VarSet& CovManifold::varset(int j) const {
	if (m<n)
		return _manifold_varset[j];
	else
		return _manifold_varset[0];
}

inline void CovManifold::add_solution(const IntervalVector& existence) {
	add_solution(existence, existence);
}

inline void CovManifold::add_solution(const IntervalVector& existence, const VarSet& varset) {
	add_solution(existence, existence, varset);
}

inline size_t CovManifold::nb_solution() const {
	return _manifold_solution.size();
}

inline size_t CovManifold::nb_boundary() const {
	return _manifold_boundary.size();
}

//class CovRegularBoundaryManifold : public CovManifold {
//public:
//protected:
//	bool* is_regular; // whether the ith not_solution box is regular.
//	int nb_regular;
//	int* regular; // indices of regular boxes
//
//	int nb_not_regular;
//	int* not_regular;
//};
//

} /* namespace ibex */

#endif /* __IBEX_COV_MANIFOLD_H__ */
