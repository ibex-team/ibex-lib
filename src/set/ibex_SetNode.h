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
#include "ibex_Sep.h"
#include "ibex_BoolInterval.h"
#include "ibex_BoolInterval.h"

namespace ibex {

/**
 * \ingroup iset
 * \brief Exception thrown by sync function.
 *
 * Thrown when the i-set is empty (<=> no set exists).
 */
class NoSet {

};

/**
 * \brief Set node.
 */
class SetNode {

public:
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
	 * \brief Synchronization with an explicit i-set "other"
	 *
	 * Important:  what is outside of "other" is considered to be "UNK"
	 */
	SetNode* sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	/**
	 * \brief Intersection with an i-set represented implicitly by a Sep
	 */
	SetNode* inter(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps);

	/**
	 * \brief Intersection with an explicit i-set "other"
	 *
	 * Important: what is outside of "other" is considered to be "IN"
	 */
	SetNode* inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	/**
	 * \brief Intersection with an i-set reduced to a single box "x" of status "x_status".
	 *
	 * \param sync: whether we synchronize the two sets or calculate their intersection.
	 *              If sync==true, what is outside of x is considered to be "UNK"
	 *              If sync==false, what is outside of x is considered to be "IN"
	 */
	virtual SetNode* inter(bool sync, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps)=0;

	/**
	 * \brief Intersection with a Sep (recursive call)
	 *
	 * Once the current "nodebox" has been intersected with the two sets obtained by applying the inner and
	 * outer contraction, a recursive call is performed (where the Sep is applied on the sub-boxes).
	 *
	 * If this node is a leaf, it means it has to be bisected.
	 *
	 * \param sync: whether we synchronize the two sets or calculate their intersection.
	 *              If sync==true, what is outside of x is considered to be "UNK"
	 *              If sync==false, what is outside of x is considered to be "IN"
	 */
	virtual SetNode* inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps)=0;

	/**
	 * \brief Union with an explicit i-set "other"
	 *
	 * Important:  what is outside of "other" is considered to be "OUT"
	 */
	SetNode* union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	/**
	 * \brief Union with an i-set reduced to a single box "x" of status "x_status".
	 *
	 * Important:  what is outside of x is considered to be "OUT"
	 *
	 */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps)=0;

	/**
	 * \brief Visit the leaves of a tree with a callback "func"
	 */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const=0;

	/**
	 * \brief Display the structure on output stream "os"
	 */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const=0;

};


SetNode* diff(const IntervalVector& x, const IntervalVector& y, BoolInterval x_status, BoolInterval y_status, double eps);

} // namespace ibex

#endif // __IBEX_SET_NODE_H__
