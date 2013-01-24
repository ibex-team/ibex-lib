//============================================================================
//                                  I B E X                                   
// File        : Paver
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_PAVER_H__
#define __IBEX_PAVER_H__

#include "ibex_Array.h"
#include "ibex_Ctc.h"
#include "ibex_Bsc.h"
#include "ibex_CellBuffer.h"
#include "ibex_SubPaving.h"

namespace ibex {

class CapacityException : public Exception { };

/** \ingroup strategy
 * \brief Paver.
 *
 * This class implements a branch & contract algorithm.
 * It allows to apply alternatively a list of contractors and a bisector until the buffer
 * of boxes (or, more exactly, cells) gets empty.
 * See the description of this algorithm in <a href="www.references.html#cha09">[cha09]</a>
 *
 */
class Paver {
public:
	/*
	 * \brief Create a paver with default buffer.
	 *
	 * The cell buffer is a stack (depth-first search)
	 */
	//Paver(const Array<Ctc>& c, Bsc& b);

	/**
	 * \brief Create a paver.
	 */
	Paver(const Array<Ctc>& c, Bsc& b, CellBuffer& cells);

	/**
	 * \brief Run the paver.
	 *
	 * The paving returned is an array that must be disallocated by the caller.
	 */
	SubPaving* pave(const IntervalVector& init_box);

	/*----------------------------------------------------------------------------------*/
	/*                                        PARAMETERS                                */
	/*----------------------------------------------------------------------------------*/

	/**
	 * \brief Capacity of the solver.
	 *
	 * The total number of boxes that can be stored.
	 * This parameter allows to bound space complexity.
	 * The value can be fixed by the user. By default, it is -1 (no limit).
	 *
	 */
	long capacity;

	/**
	 * \brief Time limit.
	 *
	 * Maximum cpu time used by the strategy.
	 * This parameter allows to bound time complexity.
	 * The value can be fixed by the user.
	 */
	double timeout;

	/**
	 * \brief Contraction fix-point flag.
	 *
	 * If true, the paver will loop on contractors until the fixpoint
	 * is reached, for each cell. Otherwise, contractors are only called once.
	 * Default value is  \c true.
	 */
	bool ctc_loop;

	/**
	 * \brief Trace activation flag.
	 *
	 * A flag for printing the trace. If set, the top of the buffer is printed
	 * on the standard output each time a new cell is created. Default value is \c false.
	 */
	bool trace;

	/** Contractors. */
	Array<Ctc> ctc;

	/** Bisector. */
	Bsc& bsc;

	/** Cell buffer. */
	CellBuffer& buffer;

protected:

	/**
	 * \brief Calls all the contractors until the fix-point is reached.
	 *
	 * Contracted parts are put into the paving in argument.
	 *
	 * \return the contractor number that entirely emptied the box, if any.
	 * Otherwise, return -1 (the box is non empty and the fixpoint is reached).
	 */
	void contract(Cell& c, SubPaving*);

	/**
	 * \brief Check the number of boxes stored in the paving.
	 */
	void check_capacity(SubPaving*);

	/**
	 * \brief Bisect the cell and push the two subcells into the buffer.
	 */
	void bisect(Cell& c);

};


/*============================================ inline implementation ============================================ */


} // end namespace ibex
#endif // __IBEX_PAVER_H__
