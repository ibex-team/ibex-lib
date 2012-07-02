/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTRACTOR_H__
#define __IBEX_CONTRACTOR_H__

#include "ibex_IntervalVector.h"
//#include "ibex_Indicators.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_BoolMask.h"

namespace ibex {

class Cell;

/**
 * \defgroup contractor Contractors
 * \ingroup contractor
 *
 * \brief Contractor interface.
 */
class Ctc {

public:
	/**
	 * \brief Build a contractor for n-dimensional boxes
	 */
	Ctc(int n);

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/**
	 * \brief Incremental contraction.
	 *
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 * By default, this function calls contract(box).
	 *
	 * \see #contract(IntervalVector&).
	 */
	virtual void contract(IntervalVector& box, const BoolMask& impact);

	/*
	 * \brief Contraction of a cell (optional)
	 *
	 * Implementation is optional. By default, call
	 * contract(cell.box).
	 */
	//virtual void contract(Cell& cell);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Ctc();

	/**
	 * \brief Add backtrackable data (optional)
	 *
	 * Allows to add the backtrackable data required
	 * by this contractor to the root cell before a
	 * strategy is executed.<br>
	 *
	 * By default: does nothing.
	 */
	//virtual void init_root(Cell& root) { }


	/**
	 * \brief Whether this contractor is independent (optional)
	 *
	 * By default: return false.
	 */
	virtual bool indempotent() {
		return false;
	}

	/**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;

	/**
	 * \brief The input variables
	 */
	BoolMask input;

	/**
	 * \brief The output variables
	 */
	BoolMask output;
};

} // namespace ibex
#endif // __IBEX_CONTRACTOR_H__
