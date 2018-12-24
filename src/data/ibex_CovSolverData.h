//============================================================================
//                                  I B E X
// File        : ibex_CovSolverData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 12, 2018
//============================================================================

#ifndef __IBEX_COV_SOLVER_DATA_H__
#define __IBEX_COV_SOLVER_DATA_H__

#include "ibex_CovManifold.h"

namespace ibex {

class CovSolverDataFile;

class CovSolverData : public CovManifold {
public:
	typedef enum { INNER, SOLUTION, BOUNDARY, UNKNOWN, PENDING } BoxStatus;

	CovSolverData(size_t n, size_t m, size_t nb_ineq=0);

	CovSolverData(const char* filename);

	virtual ~CovSolverData();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	virtual void add(const IntervalVector& x);

	virtual void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	virtual void add_boundary(const IntervalVector& x);

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity);

	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	virtual void add_pending(const IntervalVector& x);


	BoxStatus status(int i) const;

	/**
	 * \brief The jth pending box.
	 */
	const IntervalVector& pending(int j) const;

	/**
	 * \brief The jth unknown box.
	 */
	const IntervalVector& unknown(int j) const;

	/**
	 * \brief Names of the variables.
	 *
	 * By default: empty vector.
	 */
	std::vector<std::string> var_names;

	/*
	 * \brief Return status of the last solving.
	 *
	 * Return type can be safely cast to Solver::Status.
	 *
	 * By default: SUCCESS (means "unknown" if time==-1)
	 */
	unsigned int solver_status;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 *
	 * By default: -1 (means "unknown").
	 */
	double time;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 *
	 * By default: 0 (means "unknown").
	 */
	unsigned long nb_cells;

	/**
	 * \brief Number of pending boxes.
	 *
	 * By default: 0.
	 */
	size_t nb_pending() const;

	/**
	 * \brief Number of unknown boxes.
	 *
	 * By default: #CovIUList::nb_unknown.
	 */
	size_t nb_unknown() const;

protected:
	friend class CovSolverDataFile;

	std::vector<BoxStatus>        _solver_status;    // status of the ith box
	std::vector<IntervalVector*>  _solver_pending;
	std::vector<IntervalVector*>  _solver_unknown;
};

std::ostream& operator<<(std::ostream& os, const CovSolverData& solver);

inline CovSolverData::BoxStatus CovSolverData::status(int i) const {
	return _solver_status[i];
}

inline const IntervalVector& CovSolverData::pending(int j) const {
	return *_solver_pending[j];
}

inline const IntervalVector& CovSolverData::unknown(int j) const {
	return *_solver_unknown[j];
}


inline size_t CovSolverData::nb_pending() const {
	return _solver_pending.size();
}

inline size_t CovSolverData::nb_unknown() const {
	return _solver_unknown.size();
}


class CovSolverDataFile : public CovManifoldFile {
public:
	static std::ifstream* read(const char* filename, CovSolverData& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovSolverData into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovSolverData& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Display the format of a CovSolverData file.
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

	/* read the variable names */
	static void read_vars(std::ifstream& f, size_t n, std::vector<std::string>& var_names);

	/* write the variable names */
	static void write_vars(std::ofstream& f, const std::vector<std::string>& var_names);
};

} /* namespace ibex */

#endif /* __IBEX_COV_SOLVER_DATA_H__ */
