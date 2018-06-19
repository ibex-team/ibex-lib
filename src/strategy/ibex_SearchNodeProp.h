/*
 * ibex_SearchNodeProperty.h
 *
 *  Created on: Jun 6, 2018
 *      Author: gilles
 */

#ifndef IBEX_SEARCHNODEPROP_H_
#define IBEX_SEARCHNODEPROP_H_

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
class SearchNodeProp: public Property {
public:
	virtual SearchNodeProp* copy() const=0;

	/**
	 * \brief Create data associated to child cells.
	 */
	virtual std::pair<SearchNodeProp*,SearchNodeProp*> update_bisect(const BisectionPoint&);

	virtual ~SearchNodeProp();
};

inline std::pair<SearchNodeProp*,SearchNodeProp*> SearchNodeProp::update_bisect(const BisectionPoint&) {
	return std::make_pair(copy(),copy());
}

inline SearchNodeProp::~SearchNodeProp() {

}

} /* namespace ibex */

#endif /* IBEX_SEARCHNODEPROP_H_ */
