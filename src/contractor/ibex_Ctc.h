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
 */

/**
 * \ingroup contractor
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
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Whether this contractor is idempotent (optional)
	 *
	 * By default: return false.
	 */
	virtual bool idempotent();

	/**
	 * \brief Contraction with specified impact.
	 *
	 * Information on the impact allows incremental contraction.
	 * The \a impact specifies the variables that have been
	 * modified since the last call to this contractor.
	 * By default, this function calls contract(box).
	 *
	 * \see #contract(IntervalVector&).
	 */
	void contract(IntervalVector& box, const BoolMask& impact);

	/**
	 * \brief Contraction of a cell.
	 */
	void contract(Cell& cell);

	/**
	 * \brief Contraction of a cell, with impact specified.
	 */
	void contract(Cell& cell, const BoolMask& impact);

	/**
	 * \brief Return the current impact (NULL pointer if none).
	 *
	 * \return if NULL, it does not mean that there is no impact but that the information on
	 * the impact is not provided.
	 */
	const BoolMask* impact();

	/**
	 * \brief Return the current cell (NULL pointer if none).
	 */
	Cell* cell();

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

private:
	Cell* _cell;
	const BoolMask* _impact;
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline const BoolMask* Ctc::impact() {
	return _impact;
}

inline Cell* Ctc::cell() {
	return _cell;
}


} // namespace ibex

#endif // __IBEX_CONTRACTOR_H__
