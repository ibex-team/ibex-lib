//============================================================================
//                                  I B E X                                   
// File        : Cells
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 10, 2012
// Last Update : May 10, 2012
//============================================================================

#ifndef __IBEX_CELL_H__
#define __IBEX_CELL_H__

#include "ibex_IntervalVector.h"
#include "ibex_Backtrackable.h"
#include "ibex_SymbolMap.h"
#include <typeinfo>

namespace ibex {

class Strategy; // for friendship

/** \ingroup strategy
 *
 * \brief Representation of the search space.
 *
 * This representation includes default data (current box) and data related to
 * user-defined contractors or bisectors. A different cell is associated to each
 * node and cell construction/inheritance can be controlled (see #ibex::Backtrackable).
 *
 * The cell on its own contains the minimum of information associated to the actual search space.
 * Besides the current box (the search space), this minimum information includes, e.g., the number
 * of the last bisected variable (other fields might be added with future releases).
 *
 * The amount of information contained in a cell can be arbitrarily augmented thanks to the
 * "data registration" technique (see #ibex::Contractor::require()).
 */
class Cell {
public:

	/**
	 * \brief Bisect this cell.
	 *
	 * The box of the first (resp. second) cell is \a left (resp. \a right).
	 * Each subcell inherits from the data of this cell via the
	 * \link #ibex::Backtrackable::down() down \endlink
	 *
	 * <p>
	 * This function is called by the bisector. Note that the actual
	 * bisector class can simply bisect a box into two subboxes, the
	 * cell bisection has a default implementation in #ibex::Bsc.
	 */
	std::pair<Cell*,Cell*> bisect(const IntervalVector& left, const IntervalVector& right);

	/**
	 * \brief Delete *this.
	 */
	~Cell();

	/**
	 * \brief Return true if this cell is the root cell.
	 */
	//bool is_root() const;

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

private:
	friend class Strategy;
	int status; // managed by Strategy

	Cell(const IntervalVector& box);

	/* A constant to be used when no variable has been split yet (root cell). */
	//static const int ROOT_CELL;
};

ostream& operator<<(ostream& os, const Cell& c);

} // end namespace ibex

#endif // __IBEX_CELL_H__
