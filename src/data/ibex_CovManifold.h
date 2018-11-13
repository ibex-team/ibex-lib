//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#ifndef __IBEX_BXS_MANIFOLD_H__
#define __IBEX_BXS_MANIFOLD_H__

#include "ibex_CovIBUList.h"

namespace ibex {

class CovManifoldFactory;

class CovManifold : public CovIBUList {
public:
	CovManifold(const CovManifoldFactory&);

	virtual ~CovManifold();

	virtual int subformat_number() const;

	/**
	 * \brief Number of equalities.
	 */
	const unsigned int m;

	/**
	 * \brief Number of inequalities.
	 */
	const size_t nb_ineq;

	const size_t nb_solution;

	const IntervalVector& solution(int i) const;

	const size_t nb_boundary;

	const IntervalVector& boundary(int i) const;

protected:
	friend class CovManifoldFactory;

	bool* _manifold_is_solution;  // whether the ith 'manifold_boundary' is a solution or not.
	IntervalVector**  _manifold_solution;
	IntervalVector**  _manifold_boundary;
	//	Certificate* proofs; // proofs for the solution.
};

inline const IntervalVector& CovManifold::solution(int i) const {
	return *_manifold_solution[i];
}

inline const IntervalVector& CovManifold::boundary(int i) const {
	return *_manifold_boundary[i];
}

class CovManifoldFactory : public CovIBUListFactory {
public:
	CovManifoldFactory();

	virtual ~CovManifoldFactory();

	void add_solution(const IntervalVector& x);

	virtual void add_boundary(const IntervalVector& x);

	void set_nb_equ(size_t);

	void set_nb_ineq(size_t);

private:
	friend class CovManifold;
	void build(CovManifold&) const;

	size_t m;
	size_t nb_ineq;

	std::vector<bool> is_solution;
	int nb_solution;
};


inline void CovManifoldFactory::set_nb_equ(size_t m) {
	this->m = m;
}

inline void CovManifoldFactory::set_nb_ineq(size_t nb_ineq) {
	this->nb_ineq = nb_ineq;
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
//class CovSolverDataCova : public CovRegularBoundaryManifold {
//public:
//	/**
//	 * \brief Names of the variables.
//	 *
//	 * By default: empty strings.
//	 */
//	std::string *var_names;
//
//	/*
//	 * \brief Return status of the last solving.
//	 */
//	Solver::Status status;
//
//
//	/*
//	 * \brief CPU running time used to obtain this manifold.
//	 */
//	double time;
//
//	/**
//	 * \brief Number of cells used to obtain this manifold.
//	 */
//	unsigned int nb_cells;
//protected:
//	bool* is_pending; // whether the ith not_boundary box is 'pending' or 'unknwon'.
//	int nb_pending;
//	int* pending; // indices of pending boxes
//
//	int nb_not_pending;
//	int* not_pending;
//
//};

} /* namespace ibex */

#endif /* __IBEX_BXS_MANIFOLD_H__ */
