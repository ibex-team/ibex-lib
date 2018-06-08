/*
 * ibex_SearchNodeProperty.h
 *
 *  Created on: Jun 6, 2018
 *      Author: gilles
 */

#ifndef IBEX_SEARCHNODEPROPERTY_H_
#define IBEX_SEARCHNODEPROPERTY_H_

#include "ibex_Property.h"
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
class SearchNodeProperty: public Property {
public:
	SearchNodeProperty();

	virtual SearchNodeProperty* copy() const=0;

	/**
	 * \brief Create data associated to child cells.
	 */
	virtual std::pair<SearchNodeProperty*,SearchNodeProperty*> update_children(const BisectionPoint&)=0;

	virtual ~SearchNodeProperty();
};

} /* namespace ibex */

#endif /* IBEX_SEARCHNODEPROPERTY_H_ */
