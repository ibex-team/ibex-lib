//============================================================================
//                                  I B E X                                   
// File        : Cells
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : Dec 12, 2017
//============================================================================

#ifndef __IBEX_CELL_H__
#define __IBEX_CELL_H__

#include "ibex_IntervalVector.h"
#include "ibex_Backtrackable.h"
#include "ibex_SymbolMap.h"

#include <typeinfo>

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
	explicit Cell(const IntervalVector& box);

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

	/**
	 * \brief Delete *this.
	 */
	virtual ~Cell();

	/**
	 * \brief Retrieve backtrackable data from this cell.
	 *
	 * The data is identified by its classname.
	 * \pre Class \a T is a subclass of #ibex::Backtrackable.
	 */
	template<typename T>
	T& get() {
		return (T&) *data[typeid(T).name()];
	}

	/**
	 * \brief Retrieve backtrackable data from this cell.
	 *
	 * The data is identified by its classname.
	 * \pre Class \a T is a subclass of #ibex::Backtrackable.
	 */
	template<typename T>
	const T& get() const {
		return (T&) *data[typeid(T).name()];
	}

	/**
	 * \brief Add backtrackable data into this cell.
	 *
	 * The data is identified by its classname.
	 * \pre Class \a T is a subclass of #ibex::Backtrackable.
	 */
	template<typename T>
	void add() {
		const char* id=typeid(T).name();
		if (!data.used(id)) data.insert_new(id,new T());
	}

	/**
	 * \brief The box
	 */
	IntervalVector box;

	/**
	 * \brief Other data.
	 */
	SymbolMap<Backtrackable*> data;
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

} // end namespace ibex

#endif // __IBEX_CELL_H__
