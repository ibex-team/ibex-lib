//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 12, 2018
//============================================================================

#ifndef __IBEX_COV_MANIFOLD_H__
#define __IBEX_COV_MANIFOLD_H__

#include "ibex_CovIBUList.h"
#include "ibex_VarSet.h"

namespace ibex {

class CovManifoldFactory;

class CovManifold : public CovIBUList {
public:
	typedef enum { INNER, SOLUTION, BOUNDARY, UNKNOWN } BoxStatus;

	CovManifold(const CovManifoldFactory&);

	CovManifold(const char* filename);

	virtual ~CovManifold();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename);

	BoxStatus status(int i) const;

	bool is_solution(int i) const;

	bool is_boundary(int i) const;

	const IntervalVector& solution(int j) const;

	const IntervalVector& unicity(int j) const;

	/**
	 * \brief Certificate of the jth solution.
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
	const size_t nb_solution;

	/**
	 * \brief Number of boundary boxes.
	 */
	const size_t nb_boundary;

protected:
	friend class CovManifoldFactory;

	BoxStatus*        _manifold_status;    // status of the ith box
	IntervalVector**  _manifold_solution;  // pointer to 'solution' boxes
	IntervalVector**  _manifold_boundary;  // pointer to 'boundary' boxes
	IntervalMatrix    _unicity;            // all the unicity boxes (stored in a matrix)

	// in the special case where m=n, there is only one
	// possible varset, so a unique varset is stored:
	// _varset[0][0].
	VarSet**          _varset;
};

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
	return _unicity[j];
}

inline const VarSet& CovManifold::varset(int j) const {
	if (m<n)
		return *_varset[j];
	else
		return *_varset[0];
}

class CovManifoldFile;

class CovManifoldFactory : public CovIBUListFactory {
public:
	CovManifoldFactory(size_t n);

	virtual ~CovManifoldFactory();

	void add_solution(const IntervalVector& existence);

	void add_solution(const IntervalVector& existence, const IntervalVector& unicity);

	void add_solution(const IntervalVector& existence, const VarSet& varset);

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	void set_nb_equ(size_t);

	void set_nb_ineq(size_t);

	size_t nb_solution() const;

	size_t nb_boundary() const;

	size_t nb_eq;

	size_t nb_ineq;

private:
	friend class CovManifold;
	friend class CovManifoldFile;

	CovManifoldFactory(const char* filename);

	void build(CovManifold&) const;

	/*
	 * Indices of solution boxes.
	 * Must be a subset of the 'boundary' CovIBUList boxes.
	 */
	std::vector<unsigned int> solution;
	std::vector<IntervalVector> unicity;
	std::vector<VarSet> varset;
};

std::ostream& operator<<(std::ostream& os, const CovManifold& manif);

inline void CovManifoldFactory::set_nb_equ(size_t nb_eq) {
	this->nb_eq = nb_eq;
}

inline void CovManifoldFactory::set_nb_ineq(size_t nb_ineq) {
	this->nb_ineq = nb_ineq;
}

inline void CovManifoldFactory::add_solution(const IntervalVector& existence) {
	add_solution(existence, existence);
}

inline void CovManifoldFactory::add_solution(const IntervalVector& existence, const VarSet& varset) {
	add_solution(existence, existence, varset);
}

inline size_t CovManifoldFactory::nb_solution() const {
	return solution.size();
}

inline size_t CovManifoldFactory::nb_boundary() const {
	return CovIBUListFactory::nb_boundary() - nb_solution();
}

class CovManifoldFile : public CovIBUListFile {
public:
	static std::ifstream* read(const char* filename, CovManifoldFactory& factory, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovManifold into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovManifold& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Display the format of a CovManifold file.
	 */
	static string format();

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

protected:
	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);

	/* read parameters of the parametric proof */
	static VarSet read_varset(std::ifstream& f, size_t n, size_t m);

	/* write parameters of the parametric proof */
	static void write_varset(std::ofstream& f, const VarSet& varset);
};

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
