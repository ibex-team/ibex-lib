//============================================================================
//                                  I B E X                                   
// File        : ibex_Strategy.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#ifndef __IBEX_STRATEGY_H__
#define __IBEX_STRATEGY_H__

#include "ibex_CellBuffer.h"
#include "ibex_Bsc.h"
#include "ibex_StrategyException.h"
#include "ibex_Timer.h"

namespace ibex {

class Strategy {
public:
	/**
	 * \brief Create a strategy with round robin and cell stack (depth-first search)
	 */
	Strategy();

	/**
	 * \brief Create a new strategy with a cell stack
	 */
	Strategy(Bsc& bsc);

	/**
	 * \brief Create a new strategy with round robin
	 */
	Strategy(CellBuffer& buffer);

	/**
	 * \brief Create a new strategy.
	 */
	Strategy(Bsc& bsc, CellBuffer& buffer);

	/**
	 * \brief Delete *this.
	 */
	~Strategy();

	/**
	 * \brief Return the next cell to be processed.
	 *
	 * \return NULL if the search is over.
	 */
	Cell* next_cell();

	/**
	 * \brief Total number of cells created
	 */
	long nb_cells;

	/**
	 * \brief Cell limit.
	 *
	 * Maximal number of cells created by the strategy.
	 * This parameter allows to bound space/time complexity.
	 * The value can be fixed by the user. By default, it is -1 (no limit).
	 */
	long cell_limit;

	/**
	 * \brief Total running time.
	 */
	double time;

	/**
	 * \brief Time limit.
	 *
	 * Maximum cpu time used by the strategy.
	 * This parameter allows to bound time complexity.
	 * The value can be fixed by the user. By default, it is -1 (no limit).
	 */
	double time_limit;

	/**
	 * \brief Trace activation flag.
	 *
	 * A flag for printing the trace. If set, the top of the buffer is printed
	 * on the standard output each time a new cell is created. Default value is \c false.
	 */
	bool trace;

protected:
	/**
	 * \brief The bisector.
	 */
	Bsc& bsc;

	/**
	 * \brief The Cell buffer.
	 */
	CellBuffer& buffer;

	/**
	 * \brief Initialize the strategy.
	 *
	 * Create a new cell with init_box and push it in the buffer.
	 */
	void start(const IntervalVector& init_box);

	/**
	 * \brief Check the number of cells created and the time spent
	 */
	void check_limits();

	/**
	 * \brief Display the top of the buffer
	 */
	void show_buffer();

	/** Number of screens in the trace */
	int screen;

private:
	/**
	 * \brief Perform a bisection step.
	 */
	void bisect();

	enum { NOT_VISITED, IN_VISIT, VISITED };

	bool standalone_bsc;
	bool standalone_buffer;

};

} // end namespace ibex
#endif // __IBEX_STRATEGY_H__
