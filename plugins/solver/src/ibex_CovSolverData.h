//============================================================================
//                                  I B E X
// File        : ibex_CovSolverData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 27, 2018
//============================================================================

#ifndef __IBEX_COV_SOLVER_DATA_H__
#define __IBEX_COV_SOLVER_DATA_H__

#include "ibex_CovManifold.h"

namespace ibex {
/**
 * \ingroup data
 *
 * \brief Solver (IbexSolve) data.
 *
 * The solver data is a 'manifold covering', i.e., a list of boxes covering
 * a manifold, potentially bounded by inequalities. This manifold is usually
 * implicitly represented in Ibex by a #System object. This object is
 * usually built by ibexsolve from a Minibex input file.
 *
 * Unknown boxes of the mother class are spread into two categories.
 * A CovManifold unknown box is either:
 *
 * - 'unknown' if the box has been processed (precision eps-min reached)
 *   but nothing could be proven.
 *
 * - 'pending' if the box has not been processed (the search has been
 *   interrupted because of a timeout/memory overflow).
 *
 * Furthermore, this class contains additional information about the last solving:
 * - solver status
 * - number of cells
 * - running time.
 *
 */
class CovSolverData : public CovManifold {
public:

	/**
	 * \brief Possible status of a box in solver data.
	 */
	typedef enum { INNER, SOLUTION, BOUNDARY, UNKNOWN, PENDING } BoxStatus;

	/**
	 * \brief Create an empty solver data structure.
	 *
	 * \param n       - number of variables
	 * \param m       - number of equalities
	 * \param nb_ineq - number of inequalities (0 by default)
	 */
	CovSolverData(size_t n, size_t m, size_t nb_ineq=0);

	/**
	 * \brief Load solver data from a COV file.
	 */
	CovSolverData(const char* filename);

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
	virtual void add_boundary(const IntervalVector& x);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 *
	 * \see CovManifold.
	 */
	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity);

	/**
	 * \brief Add a new 'solution' box at the end of the list.
	 *
	 * \see CovManifold.
	 */
	virtual void add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset);

	/**
	 * \brief Add a new 'pending' box at the end of the list.
	 */
	virtual void add_pending(const IntervalVector& x);

	/**
	 * \brief Status of the ith box.
	 */
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
	 * \brief Status of the last solving.
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
	 */
	size_t nb_pending() const;

	/**
	 * \brief Number of unknown boxes.
	 */
	size_t nb_unknown() const;

	/**
	 * \brief Display the format of a CovSolverData file.
	 */
	static string format();

protected:

	/**
	 * \brief Load solver data from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovSolverData& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write solver data into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovSolverData& cov, std::stack<unsigned int>& format_seq);

	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);

	/* read the variable names */
	static void read_vars(std::ifstream& f, size_t n, std::vector<std::string>& var_names);

	/* write the variable names */
	static void write_vars(std::ofstream& f, const std::vector<std::string>& var_names);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

	std::vector<BoxStatus>        _solver_status;    // status of the ith box
	std::vector<IntervalVector*>  _solver_pending;
	std::vector<IntervalVector*>  _solver_unknown;

};

/**
 * \brief Stream out solver data.
 */
std::ostream& operator<<(std::ostream& os, const CovSolverData& solver);

/*================================== inline implementations ========================================*/

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

} /* namespace ibex */

#endif /* __IBEX_COV_SOLVER_DATA_H__ */
