//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Nov 13, 2018
//============================================================================

#ifndef __IBEX_COV_MANIFOLD_H__
#define __IBEX_COV_MANIFOLD_H__

#include "ibex_CovIBUList.h"

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

	virtual int subformat_number() const;

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
	 */
	const unsigned int m;

	/**
	 * \brief Number of inequalities.
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
	return *_varset[j];
}

class CovManifoldFile;

class CovManifoldFactory : public CovIBUListFactory {
public:
	CovManifoldFactory(size_t n);

	virtual ~CovManifoldFactory();

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	void set_nb_equ(size_t);

	void set_nb_ineq(size_t);

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


inline void CovManifoldFactory::set_nb_equ(size_t nb_eq) {
	this->nb_eq = nb_eq;
}

inline void CovManifoldFactory::set_nb_ineq(size_t nb_ineq) {
	this->nb_ineq = nb_ineq;
}

class CovManifoldFile : public CovIBUListFile {
public:
	static std::ifstream* read(const char* filename, CovManifoldFactory& factory);

	/**
	 * \brief Write a CovManifold into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovManifold& cov);

	/**
	 * \brief Display the format of a CovManifold file.
	 */
	static string format();

protected:
	static void format(std::stringstream& ss, const string& title);

	/* read parameters of the parametric proof */
	static VarSet read_varset(std::ifstream& f, size_t n, size_t m);

	/* write parameters of the parametric proof */
	static void write_varset(std::ofstream& f, const VarSet& varset);
};

inline std::string CovManifoldFile::format() {
	std::stringstream ss;
	format(ss,"CovManifold");
	return ss.str();
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
