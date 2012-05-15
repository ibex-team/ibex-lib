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
#include "ibex_Indicators.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

class Cell;

/**
 * \defgroup level2
 *
 * \brief Contractor interface.
 */
class Ctc {

public:
	/**
	 * Build a contractor for n-dimensional boxes
	 */
	Ctc(int n);

	/**
	 * \brief Contraction.
	 */
	virtual void contract(IntervalVector& box)=0;

	/**
	 * \brief Contraction of a cell.
	 *
	 * Implementation is optional. By default, call
	 * contract(cell.box).
	 */
	virtual void contract(Cell& cell);

	/**
	 * \brief Contraction with indicators.
	 *
	 * By default, call the #contract(Domain&) function.
	 */
	virtual void contract(IntervalVector& box, const Indicators& idc) {
		contract(box);
	}

	/**
	 * \brief Return true if this contractor can contract the dimension
	 * v of the box.
	 *
	 * By default, return true.
	 */
	virtual bool can_contract(int v) const {
		return true;
	}

	/**
	 * \brief Delete *this.
	 */
	virtual ~Ctc();

	/**
	 * Allows to add the backtrackable data required
	 * by this contractor to the root cell before a
	 * strategy is executed.<br>
	 * By default: does nothing.
	 */
	virtual void init_root(Cell& root) { }

	/**
	 * \brief The number of variables this contractor works with.
	 */
	const int nb_var;
};

} // namespace ibex
#endif // __IBEX_CONTRACTOR_H__
