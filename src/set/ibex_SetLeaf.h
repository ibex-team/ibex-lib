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
	SetLeaf(NodeType status);

	/**
	 * \brief Delete this.
	 */
	virtual ~SetLeaf();

	/** \see SetNode */
	virtual bool is_leaf() const;

	//virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps, Mode mode);

	/** \see SetNode */
	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps);

	/** \see SetNode */
	virtual SetNode* sync_rec(const IntervalVector& nodebox, Sep& sep, double eps);

	/** \see SetNode */
	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps);

	/** \see SetNode */
	virtual SetNode* inter_rec(const IntervalVector& nodebox, Sep& sep, double eps);

	/** \see SetNode */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps);

	/** \see SetNode */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

private:
	SetLeaf(const SetLeaf&); // forbidden

};

} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
