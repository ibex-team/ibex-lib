//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Feb 28, 2019
//============================================================================

#ifndef __IBEX_COV_MANIFOLD_H__
#define __IBEX_COV_MANIFOLD_H__

#include "ibex_CovIBUList.h"
#include "ibex_VarSet.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Covering of a manifold
 *
 * Given a system of m equalities f(x)=0 and inequalities g(x)<=0:
 *
 * A box ([p],[x]) is a 'SOLUTION' only if
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
 * so that a 'solution' box is a box containing a solution, according
 * to the usual meaning.
 *
 * When the system is under-constrained, the "solution" box ([x],[p])" may be a
 * large box. The "varset" structure indicates which components correspond to x and p.
 *
 * A box ([p],[x]) is BOUNDARY only if (2) holds and the manifold
 * f=0 crosses the inequalities. The exact meaning of "crosses" depends on the
 * 'boundary_type' field:
 * - EQU_ONLY  : equalities are certified (in the same way as solution boxes) but the box
 * 				 could not be proven to be entirely inside inequalities
 * - FULL_RANK : same as EQU_ONLY and the gradients of all active constraints are linearly
 *               independent
 * - HALF_BALL : the intersection of the manifold and the box is homeomorphic to a
 *               half-ball of R^n (not implemented yet).
 */
class CovManifold : public CovIBUList {
public:

	/**
	 * \brief Possible status of a box in a manifold covering.
	 *
	 * See above.
	 */
	typedef enum { SOLUTION, BOUNDARY, UNKNOWN } BoxStatus;

	/**
	 * \brief Type of a boundary box.
	 */
	typedef enum { EQU_ONLY, FULL_RANK, HALF_BALL } BoundaryType;

	/**
	 * \brief Create an empty manifold covering.
	 */
	CovManifold(size_t n, size_t m, size_t nb_ineq=0, BoundaryType boundary_type=EQU_ONLY);

	/**
	 * \brief Load a manifold covering from a COV file.
	 */
	CovManifold(const char* filename);

	/**
	 * \brief Conversion from a COV.
	 */
	CovManifold(const Cov& cov, bool copy=false);

	/**
	 * \brief Delete this
	 */
	~CovManifold();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Add a new 'unknown' box at the end of the list.
	 */
	virtual void add(const IntervalVector& x);

	/**
	 * \brief Add a new 'inner' box at the end of the list.
	 */
	virtual void add_inner(const IntervalVector& x);

	/**
	 * \brief Add a new 'unknown' box at the end of the list.
	 */
	virtual void add_unknown(const IntervalVector& x);

	/**
	 * \brief Add a new 'boundary' box at the end of the list.
	 */
	void add_boundary(const IntervalVector& x);

	/**
	 * \brief Add a new 'boundary' box at the end of the list.
	 */
	virtual void add_boundary(const IntervalVector& x, const VarSet& varset);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 */
	void add_solution(const IntervalVector& existence);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 *
	 * \param unicity - the unicity box. Represents the largest superset
	 *                  of 'existence' found such that the solution enclosed
	 *                  is unique.
	 */
	void add_solution(const IntervalVector& existence, const IntervalVector& unicity);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 *
	 * \param varset - indicates which components of the box are parameters
	 *                 in the proof, and which are variables.
	 */
	void add_solution(const IntervalVector& existence, const VarSet& varset);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 *
	 * \param unicity - see above.
	 * \param varset  - see above.
	 */
	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	/**
	 * \brief Status of the ith box.
	 */
	BoxStatus status(int i) const;

	/**
	 * \brief Whether the ith box is 'solution'.
	 */
	bool is_solution(int i) const;

	/**
	 * \brief Whether the ith box is 'boundary'.
	 */
	bool is_boundary(int i) const;

	/**
	 * \brief Existence box of the jth solution.
	 *
	 * Represents the smallest box found enclosing the manifold.
	 */
	const IntervalVector& solution(int j) const;

	/**
	 * \brief Unicity box of the jth solution.
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
	const VarSet& solution_varset(int j) const;

	/**
	 * \brief Get the jth boundary box.
	 */
	const IntervalVector& boundary(int j) const;

	/**
	 * \brief Certificate of the jth boundary box.
	 *
	 * Variable/Parameter structure used to certify the box.
	 */
	const VarSet& boundary_varset(int j) const;

	/**
	 * \brief Get the jth unknown box.
	 */
	const IntervalVector& unknown(int j) const;

	/**
	 * \brief Number of equalities.
	 *
	 * By default : 0.
	 */
	size_t nb_eq() const;

	/**
	 * \brief Number of inequalities.
	 *
	 * By default : 0.
	 */
	size_t nb_ineq() const;

	/**
	 * \brief Type of boundary boxes.
	 *
	 * By default: EQU_ONLY.
	 */
	BoundaryType boundary_type() const;

	/**
	 * \brief Number of solutions.
	 */
	size_t nb_solution() const;

	/**
	 * \brief Number of boundary boxes.
	 */
	size_t nb_boundary() const;

	/**
	 * \brief Number of unknown boxes.
	 */
	size_t nb_unknown() const;

	/**
	 * \brief Display the format of a CovManifold file.
	 */
	static std::string format();

	/**
	 * \brief COVManifold file format version.
	 */
	static const unsigned int FORMAT_VERSION;

protected:

	/**
	 * \brief Load a manifold covering from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovManifold& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write a manifold covering into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovManifold& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const std::string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

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

	struct Data {
		unsigned int                 _manifold_nb_eq;
		size_t                       _manifold_nb_ineq;
		BoundaryType                 _manifold_boundary_type;
		std::vector<BoxStatus>       _manifold_status;    // status of the ith box
		std::vector<size_t>          _manifold_solution;  // indices of 'solution' boxes
		std::vector<size_t>          _manifold_boundary;  // indices of 'boundary' boxes
		std::vector<size_t>          _manifold_unknown;  // indices of 'unknown' boxes
		std::vector<IntervalVector>  _manifold_unicity;   // all the unicity boxes

		// in the special cases where m=0 or m=n, there is only one
		// possible varset, so a unique varset is stored:
		// _varset[0][0].
		std::vector<VarSet>          _manifold_solution_varset;
		std::vector<VarSet>          _manifold_boundary_varset;
	} *data;

	bool own_data;
};

/**
 * \brief Stream out a manifold covering.
 */
std::ostream& operator<<(std::ostream& os, const CovManifold& manif);

/*================================== inline implementations ========================================*/

inline CovManifold::BoxStatus CovManifold::status(int i) const {
	return data->_manifold_status[i];
}

inline bool CovManifold::is_solution(int i) const {
	return status(i)==SOLUTION;
}

inline bool CovManifold::is_boundary(int i) const {
	return status(i)==BOUNDARY;
}

inline const IntervalVector& CovManifold::solution(int i) const {
	return (*this)[data->_manifold_solution[i]];
}

inline const IntervalVector& CovManifold::boundary(int i) const {
	return (*this)[data->_manifold_boundary[i]];
}

inline const IntervalVector& CovManifold::unicity(int j) const {
	return data->_manifold_unicity[j];
}

inline const VarSet& CovManifold::solution_varset(int j) const {
	if (nb_eq()>0 && nb_eq()<n)
		return data->_manifold_solution_varset[j];
	else
		return data->_manifold_solution_varset[0];
}

inline const VarSet& CovManifold::boundary_varset(int j) const {
	if (nb_eq()>0 && nb_eq()<n)
		return data->_manifold_boundary_varset[j];
	else
		return data->_manifold_solution_varset[0];
}

inline const IntervalVector& CovManifold::unknown(int j) const {
	return (*this)[data->_manifold_unknown[j]];
}

inline void CovManifold::add_solution(const IntervalVector& existence) {
	add_solution(existence, existence);
}

inline void CovManifold::add_solution(const IntervalVector& existence, const VarSet& varset) {
	add_solution(existence, existence, varset);
}

inline size_t CovManifold::nb_solution() const {
	return data->_manifold_solution.size();
}

inline size_t CovManifold::nb_boundary() const {
	return data->_manifold_boundary.size();
}

inline size_t CovManifold::nb_unknown() const {
	return data->_manifold_unknown.size();
}

inline size_t CovManifold::nb_eq() const {
	return data->_manifold_nb_eq;
}

inline size_t CovManifold::nb_ineq() const {
	return data->_manifold_nb_ineq;
}

inline CovManifold::BoundaryType CovManifold::boundary_type() const {
	return data->_manifold_boundary_type;
}

} /* namespace ibex */

#endif /* __IBEX_COV_MANIFOLD_H__ */
