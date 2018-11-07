//============================================================================
//                                  I B E X
// File        : ibex_Bxs.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_CLOSED_SET_H__
#define __IBEX_BXS_CLOSED_SET_H__

#include "ibex_BxsSet.h"

#include <vector>

namespace ibex {

class BxsClosedSetFactory;

class BxsClosedSet : public BxsSet {
public:
	BxsClosedSet(const BxsClosedSetFactory&);

	virtual ~BxsClosedSet();

	virtual int subformat_number() const;

	bool closed_set_is_boundary(int i) const;

	const size_t closed_set_nb_boundary;

	const IntervalVector& closed_set_boundary(int i) const;

	const size_t closed_set_nb_unknown;

	const IntervalVector& closed_set_unknown(int i) const;

protected:
	friend class BxsClosedSetFactory;

	bool* _closed_set_is_boundary;  // whether the ith 'set_unknown' box crosses the boundary or not.
	IntervalVector**  _closed_set_boundary;     // boundary boxes
	IntervalVector**  _closed_set_unknown;      // other 'unknown' boxes
};

inline bool BxsClosedSet::closed_set_is_boundary(int i) const {
	return _closed_set_is_boundary[i];
}

inline const IntervalVector& BxsClosedSet::closed_set_boundary(int i) const {
	return *_closed_set_boundary[i];
}

inline const IntervalVector& BxsClosedSet::closed_set_unknown(int i) const {
	return *_closed_set_unknown[i];
}

class BxsClosedSetFactory : public BxsSetFactory {
public:
	BxsClosedSetFactory();

	virtual ~BxsClosedSetFactory();

	void add_boundary(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class BxsClosedSet;
	void build(BxsClosedSet&) const;

	std::vector<bool> is_boundary;
	int nb_boundary;
};

//class BxsManifold : public BxsClosedSet {
//public:
//
//	/**
//	 * \brief Number of equalities.
//	 */
//	const unsigned int m;
//
//	/**
//	 * \brief Number of inequalities.
//	 */
//	const unsigned int nb_ineq;
//
//	const unsigned int nb_sol;
//
//	const IntervalVector& solution(int i);
//
//
//protected:
//	bool* is_solution; // whether the ith boundary box is a 'solution'.
//	int nb_solution;
//	int* solution; // indices of solution boxes
//
//	Certificate* proofs; // proofs for the solution.
//
//	int nb_not_solution;
//	int* not_solution;
//};
//
//class BxsRegularBoundaryManifold : public BxsClosedSet {
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
//class BxsSolverData : public BxsRegularBoundaryManifold {
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

#endif /* __IBEX_BXS_CLOSED_SET_H__ */
