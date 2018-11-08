//============================================================================
//                                  I B E X
// File        : ibex_BxsSolver.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#ifndef __IBEX_BXS_SOLVER_H__
#define __IBEX_BXS_SOLVER_H__

#include "ibex_BxsManifold.h"
#include "ibex_Solver.h"

namespace ibex {

class BxsSolverFactory;

class BxsSolver : public BxsManifold {
public:
	BxsSolver(const BxsSolverFactory&);

	virtual ~BxsSolver();

	virtual int subformat_number() const;

	/**
	 * \brief Names of the variables.
	 *
	 * By default: empty strings.
	 */
	std::string *var_names;

	/*
	 * \brief Return status of the last solving.
	 */
	Solver::Status status;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 *
	 * -1 means: unknown
	 */
	double time;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 *
	 * 0 means: unknown
	 */
	unsigned long nb_cells;

	const size_t nb_pending;

	const IntervalVector& pending(int i) const;

	const size_t solver_nb_unknown;

	const IntervalVector& solver_unknown(int i) const;

protected:
	friend class BxsSolverFactory;

	bool* _closed_set_is_pending;  // whether the ith 'closed_set_unknown' is a pending box or not.
	IntervalVector**  _solver_pending;
	IntervalVector**  _solver_unknown;
};

inline const IntervalVector& BxsSolver::pending(int i) const {
	return *_solver_pending[i];
}

inline const IntervalVector& BxsSolver::solver_unknown(int i) const {
	return *_solver_unknown[i];
}

class BxsSolverFactory : public BxsManifoldFactory {
public:
	BxsSolverFactory();

	virtual ~BxsSolverFactory();

	void add_pending(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	void set_var_names(std::string*);

	void set_status(Solver::Status status);

	void set_time(double time);

	void set_nb_cells(unsigned long nb_cells);

private:
	friend class BxsSolver;
	void build(BxsSolver&) const;

	std::string* var_names;

	Solver::Status status;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 */
	double time;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 */
	unsigned long nb_cells;

	std::vector<bool> is_pending;
	int nb_pending;
};

inline void BxsSolverFactory::set_var_names(std::string*) {

}

inline void BxsSolverFactory::set_status(Solver::Status status) {
	this->status = status;
}

inline void BxsSolverFactory::set_time(double time) {
	this->time = time;
}

inline void BxsSolverFactory::set_nb_cells(unsigned long nb_cells) {
	this->nb_cells = nb_cells;
}

} /* namespace ibex */

#endif /* __IBEX_BXS_SOLVER_H__ */
