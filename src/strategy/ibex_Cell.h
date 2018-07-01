//============================================================================
//                                  I B E X                                   
// File        : Cells
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : Jun 07, 2018
//============================================================================

#ifndef __IBEX_CELL_H__
#define __IBEX_CELL_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoxProperties.h"
#include "ibex_BisectionPoint.h"
#include "ibex_Map.h"

namespace ibex {

/**
 * \defgroup strategy Strategies
 */

/** \ingroup strategy
 *
 * \brief Node in an interval exploration binary tree.
 *
 * This representation includes default data (current box) and data related to
 * user-defined operators (like bisectors). A different cell is associated to each
 * node and cell construction/inheritance can be controlled (see #ibex::Backtrackable).
 *
 * The cell on its own contains the minimum of information associated to the actual
 * search space: the current box (other fields might be added with future releases).
 *
 * The amount of information contained in a cell can be arbitrarily augmented thanks
 * to the "data registration" technique (see #ibex::Backtrackable).
 */
class Cell {
public:

	/**
	 * \brief Create the root cell.
	 *
	 * \param box - Box (passed by copy).
	 */
	explicit Cell(const IntervalVector& box, int bisected_var=-1);

	/**
	 * \brief Constructor by copy.
	 */
	explicit Cell(const Cell& e);

	/**
	 * \brief Bisect this cell.
	 *
	 * The box of the first (resp. second) cell is \a left (resp. \a right).
	 * Each subcell inherits from the data of this cell via the
	 * \link #ibex::Backtrackable::down(const BisectionPoint&) down \endlink
	 * function.
	 *
	 * <p>
	 * This function is called by the bisector.
	 */
	std::pair<Cell*,Cell*> subcells(const BisectionPoint& b) const;

//	void sync_contract(const BitSet& impact);
//
//	void sync_change();

	/**
	 * \brief Delete *this.
	 */
	virtual ~Cell();

	/**
	 * \brief The box
	 */
	IntervalVector box;

	/**
	 * \brief Data bound to the box.
	 *
	 * Includes search node properties (SearchNodeProp) and
	 * box properties (BoxProp).
	 *
	 * Box properties are visible by all operators, including contractors.
	 * Search node properties are not visible by contractors.
	 */
	BoxProperties prop;

	/**
	 * Last bisected variable (-1 if root node).
	 */
	int bisected_var;
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

} // end namespace ibex

#endif // __IBEX_CELL_H__
