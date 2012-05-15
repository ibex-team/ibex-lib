//============================================================================
//                                  I B E X                                   
// File        : Backtrackable data
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 11, 2012
// Last Update : May 11, 2012
//============================================================================

#ifndef __IBEX_BACKTRACKABLE_H__
#define __IBEX_BACKTRACKABLE_H__

#include <utility>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Operator-specific backtrackable data.
 *
 * A <i>backtrackable</i> is a structure propagated through the search tree. It is therefore
 * only used by \link ibex::Paver pavers \endlink. Each node in the search tree has its own structure, this
 * structure being inherited from the father node. The mechanism of inheritance can be controled
 * thanks to the \link #Backtrackable(Backtrackable&, bool) copy constructor \endlink.
 * For the moment, control can only be made downward: there is no way of updating a node structure by aggregating
 * children node structures when backtracking (this shall be fixed in a future version).
 *
 * This class is an interface to be implemented by any operator data class associated to a cell.
 * Backtrackable must be viewed as a metaclass, each object being a subclass whose copy is controlled by the copy
 * constructor.
 *
 */
class Backtrackable {
public:

	/**
	 * \brief Create data of the root cell.
	 *
	 * The data of the root cell is the one obtained
	 * by calling the default constructor.
	 */
	Backtrackable() { }

	/**
	 * \brief Create data associated to child cells.
	 */
	virtual std::pair<Backtrackable*,Backtrackable*> down()=0;

	/**
	 * \brief Delete *this.
	 */
	virtual ~Backtrackable() { }
};


} // end namespace ibex
#endif // __IBEX_BACKTRACKABLE_H__
