//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_H__
#define __IBEX_SET_H__

#include "ibex_SetNode.h"
#include "ibex_Separator.h"

namespace ibex {

/**
 * \defgroup iset Set Interval
 */

/**
 * \ingroup iset
 * \brief Set Interval
 *
 * See "Solving set-valued constraint satisfaction problem", Luc Jaulin, Computing. Volume 94, Issue 2, Page 297-311.
 *
 */
class SetInterval {
public:

	/**
	 * \brief Creates a set interval from a simple box
	 *
	 * If x is the bounding box in argument, the set interval is either
	 * [empty, x] if inner==false or [x,x] if inner==false.
	 */
	SetInterval(const IntervalVector& bounding_box, double eps, bool inner=true);

	/*
	 * \brief Delete this
	 */
	~SetInterval();

	/**
	 * \brief i-Set Intersection
	 *
	 * In Jaulin's terminology, this operator is the "i-set extension of the intersection".
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x \cap y, x\in[x] and y\in[y] }.
	 */
	SetInterval& operator&=(const SetInterval& set);

	/**
	 * \brief i-Set Union
	 *
	 * In Jaulin's terminology, this operator is the "i-set extension of the union".
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x \cup y, x\in[x] and y\in[y] }.
	 */
	SetInterval& operator|=(const SetInterval& set);

	/**
	 * \brief i-Set synchronization
	 *
	 * In Jaulin's terminology, this operator is the "intersection of i-sets" (squared symbol)
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x, x\in[x] and x\in[y] }.
	 */
	void sync(Separator& sep);

	/**
	 * \brief True if this i-set is empty
	 *
	 * \warning: an empty i-set is different from a i-set containing (and possibly only containing) the empty set.
	 */
	bool is_empty() const;

	void contract(Separator& sep);

	void visit_leaves(SetNode::leaf_func func) const;

protected:
	friend std::ostream& operator<<(std::ostream& os, const SetInterval& set);

	SetNode* root; // NULL means no existing set (warning: different from empty set!)

	double eps;

	IntervalVector bounding_box; // not sure it is really necessary
};

std::ostream& operator<<(std::ostream& os, const SetInterval& set);

} // namespace ibex

#endif // __IBEX_SET_H__
