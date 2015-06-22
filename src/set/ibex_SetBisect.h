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
 * \brief Bisection node (internal class used for set representation)
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
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status);

	/** \see SetNode */
	virtual SetNode* inter(bool iset, const IntervalVector& nodebox, Sep& sep, double eps);

	/** \see SetNode */
	virtual SetNode* union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status);

	/** \see SetNode */
	virtual void visit(const IntervalVector& nodebox, SetVisitor& visitor) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	/** \see SetNode */
	virtual BoolInterval is_superset(const IntervalVector& nodebox, const IntervalVector& box) const;

	/** \see SetNode */
	virtual SetNode* contract_no_diff(BoolInterval status, const IntervalVector& nodebox, const IntervalVector& box);

	IntervalVector left_box(const IntervalVector& nodebox) const;

	IntervalVector right_box(const IntervalVector& nodebox) const;

//protected:
	friend class SetNode;
	friend class Set;

	// partial initialization used by SetInterval::load only
	// and diff
	SetBisect(int var, double pt);

	SetNode* try_merge();

	int var;
	double pt;
	SetNode* left;
	SetNode* right;

private:
	SetBisect(const SetBisect&); // forbidden
};

} // namespace ibex

#endif // __IBEX_SET_BISECT_H__
