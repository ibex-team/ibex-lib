//============================================================================
//                                  I B E X
// File        : ibex_StsLPSolver.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb, 13, 2025
// Last Update : Feb, 13, 2025
//============================================================================

#ifndef __IBEX_STS_LP_SOLVER_H__
#define __IBEX_STS_LP_SOLVER_H__

#include "ibex_Sts.h"
#include <map>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Statistics of LP Solver
 *
 */
class StsLPSolver : public Sts {
public:

	/**
	 * \brief Build statistics for a LP solver.
	 */	
	StsLPSolver(const std::string& op_name="LP Solver");

	/**
	 * \brief Report statistics.
	 */
	virtual std::string report() const override;
	
	/** 
	 * \brief Unexpected status  
	 * 
	 * Add another occurence of a unexpected internal 
	 * return status of the LP solver occur (considered 
	 * as a failure).
	 */
	void add_unexpected_status(int other_status);

protected:
	/*
	 * Maps an internal status to the number of its occurrences.
	 */
	std::map<int, unsigned int> other_status;
};

} /* namespace ibex */

#endif /* __IBEX_STS_LP_SOLVER_H__ */
