//============================================================================
//                                  I B E X                                   
// File        : Backtrackable data
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 11, 2012
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_BACKTRACKABLE_H__
#define __IBEX_BACKTRACKABLE_H__

#include "ibex_BisectionPoint.h"

#include <utility>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Operator-specific backtrackable data.
 *
 * A <i>backtrackable</i> is a structure propagated through the search tree. It is therefore
 * only used by strategies. Each node in the search tree has its own structure, this
 * structure being inherited from the father node. The mechanism of inheritance can be controlled
 * thanks to the \link #Backtrackable::down(const BisectionPoint&) down \endlink function.
 * For the moment, control can only be made downward: there is no way of updating a node structure
 * by aggregating children node structures when backtracking (this might be done in a future release).
 *
 * This class is an interface to be implemented by any operator data class associated to a cell.
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
	 * \brief Create a copy.
	 */
	virtual Backtrackable* copy() const =0;

	/**
	 * \brief Create data associated to child cells.
	 */
	virtual std::pair<Backtrackable*,Backtrackable*> down(const BisectionPoint&)=0;

//	virtual void sync_change(const IntervalVector& new_box, const BitSet& impact) { }
//
//	void sync_change(const IntervalVector& new_box);
//
//	virtual void sync_contract(const IntervalVector& new_box, const BitSet& impact) { }
//
//	void sync_contract(const IntervalVector& new_box);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Backtrackable() { }

};

//void Backtrackable::sync_change(const IntervalVector& new_box) {
//	sync_change(new_box, BitSet::all(new_box.size()));
//}
//
//void sync_contract(const IntervalVector& new_box) {
//	sync_contract(new_box, BitSet::all(new_box.size()));
//}

} // end namespace ibex

#endif // __IBEX_BACKTRACKABLE_H__
