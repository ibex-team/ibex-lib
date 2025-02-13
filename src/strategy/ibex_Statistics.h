//============================================================================
//                                  I B E X
// File        : ibex_Statistics.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb, 13, 2025
// Last Update : Feb, 13, 2025
//============================================================================

#ifndef __IBEX_STATISTICS_H__
#define __IBEX_STATISTICS_H__

#include "ibex_Sts.h"

#include <iostream>
#include <vector>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Statistics
 *
 * This class allows to store all statistics of a strategy (solver, optimizer, etc.)
 */
class Statistics {
public:

	/** 
	 * \brief Delete all statistics. 
	 */
	~Statistics();

	/**
	 * \brief Add some statistics.
	 * 
	 * \para stats	A statistics object built on the heap.
	 * 				I am responsible of deleting it.
	 */
	void add(Sts* stats);
	
	friend std::ostream& operator<<(std::ostream& os, const Statistics&);

protected:
	/* All statistics objects, one for each registered operator. 
	 * Each operator keeps a reference to its own stats. */
	std::vector<Sts*> data;
};

/**
 * \brief Print the statistics
 */
std::ostream& operator<<(std::ostream& os, const Statistics&);

} /* namespace ibex */

#endif /* __IBEX_STATISTICS_H__ */
