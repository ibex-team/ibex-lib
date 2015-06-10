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
	SetInterval(const IntervalVector& box, BoolInterval status=YES);

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
	 * \brief i-Set intersection with a set interval implicitly represented by a separator
	 *
	 * In Jaulin's terminology, this operator is the "intersection of i-sets" (squared symbol)
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x, x\in[x] and x\in[y] }.
	 *
	 * \param eps - The separator is applied recursively on this i-set. This parameter
	 *              is a precision for controlling the recursivity.
	 */
	SetInterval&  inter(Sep& sep, double eps);

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
	 * \brief Apply a function to all the leaves
	 */
	void visit_leaves(SetNode::leaf_func func) const;

	/**
	 * \ingroup iset
	 * \brief Exception thrown by sync function.
	 *
	 * Thrown when the i-set is empty (<=> no set exists).
	 */
	class NoSet {

	};
};

std::ostream& operator<<(std::ostream& os, const Set& set);

} // namespace ibex

#endif // __IBEX_SET_INTERVAL_H__
