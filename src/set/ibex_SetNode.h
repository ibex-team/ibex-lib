//============================================================================
//                                  I B E X                                   
// File        : ibex_SetNode.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_NODE_H__
#define __IBEX_SET_NODE_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_BoolInterval.h"
#include "ibex_SetVisitor.h"

namespace ibex {

class Sep;
class SetBisect;

/**
 * \brief Set node.
 */
class SetNode {

public:
	SetNode();

	/**
	 * \brief Father node (NULL by default)
	 */
	SetBisect* father;

	/**
	 * \brief Callback for "visit_leaves"
	 */
	typedef void (*leaf_func) (const IntervalVector&, BoolInterval);

	/**
	 * \brief Delete this.
	 */
	virtual ~SetNode();

	/**
	 * \brief True iff this node is a leaf.
	 */
	virtual bool is_leaf() const=0;

	/**
	 * \brief Intersection with an (i-)set represented implicitly by a Sep
	 */
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, Sep& sep, double eps)=0;

	/**
	 * \brief Intersection with an explicit (i-)set "other"
	 *
	 * Important: what is outside of "other" is considered to be "IN"
	 */
	SetNode* inter(bool iset, const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox);

	/**
	 * \brief Intersection with an (i-)set reduced to a single box "x" of status "x_status".
	 *
	 * \param iset: whether we are dealing with i-set or sets.
	 *              If iset==true, what is outside of x is considered to be "UNK"
	 *              If iset==false, what is outside of x is considered to be "IN"
	 */
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status)=0;

	/**
	 * \brief Union with an explicit set "other"
	 *
	 * Important:  what is outside of "other" is considered to be "OUT"
	 */
	SetNode* union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox);

	/**
	 * \brief Union with an set reduced to a single box "x" of status "x_status".
	 *
	 * Important:  what is outside of x is considered to be "OUT"
	 *
	 */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status)=0;

	/**
	 * \brief Visit the tree
	 */
	virtual void visit(const IntervalVector& nodebox, SetVisitor& visitor) const=0;

	/**
	 * \brief Display the structure on output stream "os"
	 */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const=0;

	/**
	 * \brief True if this node is a superset of the box
	 */
	virtual BoolInterval is_superset(const IntervalVector& nodebox, const IntervalVector& box) const=0;

	/**
	 * \brief Fast contraction.
	 *
	 * Contracts this set to the box but (for efficiency reasons) performs no "diff".
	 * So the contraction can actually only merge leaves (recursively).
	 *
	 * The contracted part is of indicated status.
	 *
	 * The resulting set is an enclosure of "box".
	 */
	virtual SetNode* contract_no_diff(BoolInterval status, const IntervalVector& nodebox, const IntervalVector& box)=0;

};

} // namespace ibex

#endif // __IBEX_SET_NODE_H__
