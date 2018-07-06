//============================================================================
//                                  I B E X
// File        : ibex_Bxp.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 01, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_BitSet.h"
#include "ibex_BisectionPoint.h"

#ifndef __IBEX_BOX_PROPERTY_H__
#define __IBEX_BOX_PROPERTY_H__

namespace ibex {

class BoxProperties;

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
class Bxp {
public:

	/**
	 * All the sibling Bxp instances must have the same id.
	 */
	Bxp(long id);

	/**
	 * \brief Create a copy.
	 *
	 * The copy must have the same id.
	 */
	virtual Bxp* copy() const=0;

	/**
	 * \brief Update the property upon box modification.
	 *
	 * \param new_box  - the box after modification.
	 * \param contract - true only if new_box is new_box is
	 *                   a subset of the current box.
	 */
	void update(const IntervalVector& new_box, bool contract, const BoxProperties& prop);

	/**
	 * \brief Update the property upon box modification.
	 *
	 * \param new_box  - the box after modification
	 * \param contract - true only if new_box is new_box is
	 *                   a subset of the current box
	 * \param impact   - the modified variables.
	 */
	virtual void update(const IntervalVector& new_box, bool contract, const BitSet& impact, const BoxProperties& prop)=0;

	/**
	 * \brief Create data associated to child cells.
	 *
	 * The two property instances must have the same id as this.
	 */
	virtual std::pair<Bxp*,Bxp*> update_bisect(const BisectionPoint& bp, const IntervalVector& left, const IntervalVector& right, const BoxProperties& prop);

	/**
	 * \brief Delete this.
	 */
	virtual ~Bxp();

	/**
	 * \brief Identifying number.
	 */
	const long id;

	/**
	 * \brief Dependencies.
	 *
	 * The identifying numbers of all the other properties required
	 * by this property.
	 */
	std::vector<long> dependencies;
};

/*================================== inline implementations ========================================*/

inline Bxp::Bxp(long id) : id(id) {
}

inline void Bxp::update(const IntervalVector& new_box, bool contract, const BoxProperties& prop) {
	update(new_box, contract, BitSet::all(new_box.size()), prop);
}

inline std::pair<Bxp*,Bxp*> Bxp::update_bisect(const BisectionPoint& bp, const IntervalVector& left, const IntervalVector& right, const BoxProperties& prop) {
	return std::make_pair(copy(),copy());
}

inline Bxp::~Bxp() {
}

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTY_H__ */
