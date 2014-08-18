//============================================================================
//                                  I B E X                                   
// File        : ibex_SetBisect.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_BISECT_H__
#define __IBEX_SET_BISECT_H__

#include "ibex_SetNode.h"

namespace ibex {

/**
 * \ingroup iset
 * \brief Bisection node (i-set representation)
 */
class SetBisect : public SetNode {

public:

	/**
	 * \brief Create a bisection node
	 *
	 * The variable of the nodebox is bisected at point "pt", resulting
	 * in two subnodes, left and right.
	 */
	SetBisect(int var, double pt, SetNode* left, SetNode* right);

	/**
	 * \brief Delete this
	 */
	virtual ~SetBisect();

	/** \see SetNode */
	virtual bool is_leaf() const;

	/** \see SetNode */
	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps);

	/** \see SetNode */
	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, NodeType x_status, double eps);

	/** \see SetNode */
	virtual SetNode* sync_rec(const IntervalVector& nodebox, Separator& sep, double eps);

	/** \see SetNode */
	virtual SetNode* inter_rec(const IntervalVector& nodebox, Separator& sep, double eps);

	/** \see SetNode */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	/** \see SetNode */
	virtual void set_in_tmp();

	/** \see SetNode */
	virtual void unset_in_tmp();

	IntervalVector left_box(const IntervalVector& nodebox) const;

	IntervalVector right_box(const IntervalVector& nodebox) const;

protected:
	friend class SetNode;

	SetNode* try_merge();

	int var;
	double pt;
	SetNode* left;
	SetNode* right;
};

} // namespace ibex

#endif // __IBEX_SET_BISECT_H__
