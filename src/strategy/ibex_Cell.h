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

	Cell(IntervalVector& box, int split_var=-1) : box(box), last_split_var(last_split_var) { }

	/** Delete this instance. */
	~Cell() {
		data.forall(_delete);
	}

	/**
	 * \brief Return true if this cell is the root cell.
	 */
	bool is_root() const;

	/**
	 * \brief Retreive backtrackable data from this cell.
	 *
	 * The data is identified by its classname.
	 * \pre Class \a T is a subclass of #ibex::Backtrackable. */
	template<typename T>
	T& get() {
		return data[typeid(T).name()];
	}

	/**
	 * \brief The box
	 */
	IntervalVector box;

	/**
	 * \brief The last split variable
	 *
	 * Undefined if this cell is the root.
	 * \sa #root().
	 */
	const int last_split_var;

	/**
	 * \brief Other data.
	 */
	SymbolMap<Backtrackable*> data;

private:
	static void _delete(const char* id, Backtrackable*& b) { delete b; }

	/* A constant to be used when no variable has been split yet (root cell). */
	static const int ROOT_CELL;
};

/*
bool Cell:is_root() const {
	return last_split_var==ROOT_CELL;
}*/


} // end namespace ibex

#endif // __IBEX_CELL_H__
