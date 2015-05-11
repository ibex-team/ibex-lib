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
#include "ibex_NodeType.h"
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
	 * \brief Creates a node of given status
	 */
	SetNode(NodeType status);

	/**
	 * \brief Delete this.
	 */
	virtual ~SetNode();

	/**
	 * \brief True iff this node is a leaf.
	 */
	virtual bool is_leaf() const=0;

	/**
	 * \brief Synchronization with an i-set represented implicitly by a Sep
	 */
	SetNode* sync(const IntervalVector& nodebox, Sep& sep, double eps);

	/**
	 * \brief Synchronization with an explicit i-set "other"
	 *
	 * skip_other_maybe: don't consider UNK box of the other set. This is important
	 *                   because the other set may be a temporary set produced by
	 *                   the contract function (and the UNK box will be refined later)
	 */
	SetNode* sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps, bool skip_other_maybe);

	/**
	 * \brief Synchronization with an i-set reduced to a single box "x" of status "x_status".
	 *
	 */
	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps)=0;

	/**
	 * \brief Synchronization with a Sep (recursive call)
	 *
	 * Once the current "nodebox" has been synchronized with the two sets obtained by applying the inner and
	 * outer contraction, a recursive call is performed (where the Sep is applied on the sub-boxes).
	 *
	 * If this node is a leaf, it means it has to be bisected.
	 */
	virtual SetNode* sync_rec(const IntervalVector& nodebox, Sep& sep, double eps)=0;

	/**
	 * \brief Intersection with an i-set represented implicitly by a Sep
	 */
	SetNode* inter(const IntervalVector& nodebox, Sep& sep, double eps);

	/**
	 * \brief Intersection with an explicit i-set "other"
	 */
	SetNode* inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	/**
	 * \brief Intersection with an i-set reduced to a single box "x" of status "x_status".
	 */
	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps)=0;

	/**
	 * \brief Intersection with a Sep (recursive call)
	 *
	 * Once the current "nodebox" has been intersected with the two sets obtained by applying the inner and
	 * outer contraction, a recursive call is performed (where the Sep is applied on the sub-boxes).
	 *
	 * If this node is a leaf, it means it has to be bisected.
	 */
	virtual SetNode* inter_rec(const IntervalVector& nodebox, Sep& sep, double eps)=0;

	/**
	 * \brief Union with an explicit i-set "other"
	 */
	SetNode* union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	/**
	 * \brief Union with an i-set reduced to a single box "x" of status "x_status".
	 */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps)=0;

	/**
	 * \brief Visit the leaves of a tree with a callback "func"
	 */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const=0;

	/**
	 * \brief Display the structure on output stream "os"
	 */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const=0;

	/**
	 * \brief The status of the node
	 */
	NodeType status;
};


SetNode* diff(const IntervalVector& x, const IntervalVector& y, NodeType x_status, NodeType y_status, double eps);

SetNode* contract_set(const IntervalVector& x, Sep& sep, double eps);

} // namespace ibex

#endif // __IBEX_SET_NODE_H__
