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
 * \brief Leaf node (i-set representation)
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
	virtual SetNode* inter(bool sync, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps);

	virtual SetNode* inter2(bool sync, const IntervalVector& nodebox, const std::pair<SetNode*,IntervalVector>& other, double eps);
	virtual std::pair<SetNode*,IntervalVector> subset(const IntervalVector& nodebox, const IntervalVector& box);

	/** \see SetNode */
	virtual SetNode* inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps);

	/** \see SetNode */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps);

	/** \see SetNode */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	/** \see SetNode */
	virtual BoolInterval is_superset(const IntervalVector& nodebox, const IntervalVector& box) const;

	/**
	 * \brief The status of the node
	 */
	BoolInterval status;

private:

	SetLeaf(const SetLeaf&); // forbidden

};


} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
