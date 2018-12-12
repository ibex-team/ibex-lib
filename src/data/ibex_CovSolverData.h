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
#include "ibex_Solver.h"

namespace ibex {

class CovSolverDataFactory;

class CovSolverData : public CovManifold {
public:
	typedef enum { INNER, SOLUTION, BOUNDARY, UNKNOWN, PENDING } BoxStatus;

	CovSolverData(const CovSolverDataFactory&);

	CovSolverData(const char* filename);

	virtual ~CovSolverData();

	virtual int subformat_number() const;

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
	 * By default: empty strings.
	 */
	std::vector<std::string> var_names;

	/*
	 * \brief Return status of the last solving.
	 *
	 * By default: ??
	 */
	Solver::Status solver_status;

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
	const size_t nb_pending;

	/**
	 * \brief Number of unknown boxes.
	 *
	 * By default: #CovIUList::nb_unknown.
	 */
	const size_t nb_unknown;

protected:
	friend class CovSolverDataFactory;

	BoxStatus*        _solver_status;    // status of the ith box
	IntervalVector**  _solver_pending;
	IntervalVector**  _solver_unknown;
};

inline CovSolverData::BoxStatus CovSolverData::status(int i) const {
	return _solver_status[i];
}

inline const IntervalVector& CovSolverData::pending(int j) const {
	return *_solver_pending[j];
}

inline const IntervalVector& CovSolverData::unknown(int j) const {
	return *_solver_unknown[j];
}

class CovSolverDataFile;

class CovSolverDataFactory : public CovManifoldFactory {
public:
	CovSolverDataFactory(size_t n);

	virtual ~CovSolverDataFactory();

	void add_pending(const IntervalVector& x);

	void set_var_names(const std::vector<std::string>&);

	void set_status(Solver::Status status);

	void set_time(double time);

	void set_nb_cells(unsigned long nb_cells);

private:
	friend class CovSolverData;
	friend class CovSolverDataFile;

	CovSolverDataFactory(const char* filename);

	void build(CovSolverData&) const;

	std::vector<std::string> var_names;

	Solver::Status status;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 */
	double time;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 */
	unsigned long nb_cells;

	/*
	 * Indices of pending boxes.
	 * Must be a subset of the 'unknown' CovIUList boxes.
	 */
	std::vector<unsigned int> pending;
};

inline void CovSolverDataFactory::set_var_names(const std::vector<std::string>& var_names) {
	this->var_names = var_names;
}

inline void CovSolverDataFactory::set_status(Solver::Status status) {
	this->status = status;
}

inline void CovSolverDataFactory::set_time(double time) {
	this->time = time;
}

inline void CovSolverDataFactory::set_nb_cells(unsigned long nb_cells) {
	this->nb_cells = nb_cells;
}


class CovSolverDataFile : public CovManifoldFile {
public:
	static std::ifstream* read(const char* filename, CovSolverDataFactory& factory);

	/**
	 * \brief Write a CovSolverData into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovSolverData& cov);

	/**
	 * \brief Display the format of a CovSolverData file.
	 */
	static string format();

protected:
	static void format(std::stringstream& ss, const string& title);

	/* read the variable names */
	static void read_vars(std::ifstream& f, size_t n, std::vector<std::string>& var_names);

	/* write the variable names */
	static void write_vars(std::ofstream& f, size_t n, const std::vector<std::string>& var_names);
};

inline std::string CovSolverDataFile::format() {
	std::stringstream ss;
	format(ss,"CovSolverData");
	return ss.str();
}

} /* namespace ibex */

#endif /* __IBEX_COV_SOLVER_DATA_H__ */
