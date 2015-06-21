//============================================================================
//                                  I B E X                                   
// File        : ibex_SetLeaf.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_LEAF_H__
#define __IBEX_SET_LEAF_H__

#include "ibex_SetNode.h"

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Leaf node (internal class used for set representation)
 */
class SetLeaf : public SetNode {

public:
	/**
	 * \brief Creates a leaf of the given status
	 */
	SetLeaf(BoolInterval status);

	/**
	 * \brief Delete this.
	 */
	virtual ~SetLeaf();

	/** \see SetNode */
	virtual bool is_leaf() const;

	/** \see SetNode */
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status);

	/** \see SetNode */
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, Sep& sep, double eps);

	/** \see SetNode */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status);

	/** \see SetNode */
	virtual BoolInterval is_superset(const IntervalVector& nodebox, const IntervalVector& box) const;

	/** \see SetNode */
	virtual SetNode* contract_no_diff(BoolInterval status, const IntervalVector& nodebox, const IntervalVector& box);

	/** \see SetNode */
	virtual void visit(const IntervalVector& nodebox, SetVisitor& visitor) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	/**
	 * \brief The status of the node
	 */
	BoolInterval status;

	/**
	 * \brief Replace this node with "node" and update the tree structure.
	 *
	 * \warning The current node is deleted.
	 */
	void replace_with(SetNode* node);

private:

	SetLeaf(const SetLeaf&); // forbidden
};

/**
 * Calculate the set difference between x and y under the form of
 * a set (SetNode). Returns a pair containing the root of this
 * set (first) and the leaf corresponding to the box y (last).
 *
 * If eps is >0, no boxes larger than epsilon are created.
 * When a box is enlarged, its status becomes MAYBE.
 *
 * Note that this may result in a leaf that does not correspond to y
 * but to an enlargement of it.
 */
std::pair<SetNode*,SetLeaf*> diff(const IntervalVector& x, const IntervalVector& y, BoolInterval x_status, BoolInterval y_status, double eps);


} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
