//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_INTERVAL_H__
#define __IBEX_SET_INTERVAL_H__

#include "ibex_Set.h"

namespace ibex {

/**
 * \ingroup iset

 * \brief Set interval (or i-set)
 *
 * Note: private inheritance as a safety for dangerous conversion
 * (semantics of operators change between Set and SetInterval).
 *
 * \see "Solving set-valued constraint satisfaction problem", Luc Jaulin, Computing. Volume 94, Issue 2, Page 297-311.
 */
class SetInterval : private Set {

public:

	/**
	 * \brief Creates a set interval from a simple box
	 *
	 * If x is the bounding box in argument, the set interval is either
	 * [empty,empty] if status=NO
	 * [empty, x] if status=MAYBE
	 * or [x,x] if status=YES.
	 */
	SetInterval(const IntervalVector& box, BoolInterval status=MAYBE);

	/**
	 * \brief Loads a i-set from a data file.
	 *
	 * \see #save().
	 */
	SetInterval(const char* filename);

	/**
	 * \brief Delete this.
	 */
	~SetInterval();

	/**
	 * \brief True if this i-set is empty
	 *
	 * \warning: an empty i-set is different from a i-set containing (and possibly only containing) the empty set.
	 */
	bool is_empty() const;

	/**
	 * \brief Serialize the i-set and save it into a file
	 */
	void save(const char* filename);

	/**
	 * \brief Apply a visitor
	 */
	void visit(SetVisitor& visitor) const;

	/**
	 * \ingroup iset
	 * \brief Exception thrown by sync function.
	 *
	 * Thrown when the i-set is empty (<=> no set exists).
	 */
	class NoSet {

	};

	friend class Sep;
};

std::ostream& operator<<(std::ostream& os, const Set& set);

} // namespace ibex

#endif // __IBEX_SET_INTERVAL_H__
