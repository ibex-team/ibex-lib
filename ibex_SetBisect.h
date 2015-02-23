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

	SetBisect(int var, double pt, SetNode* left, SetNode* right,SetNode* father);

	/**
	 * \brief Delete this
	 */
	virtual ~SetBisect();

	/** \see SetNode */
	virtual bool is_leaf() const;

	/** \see SetNode */
	virtual SetNode * copy() const;

	/** \see SetNode */
	virtual void inter(NodeType x_status);

	/** \see SetNode */
	virtual void _union(NodeType x);

	/** \see SetNode */
	virtual void oper(SetNode * other,bool op);

	/** \see SetNode */
	virtual void operator_ir(const IntervalVector& box,const IntervalVector& nodebox, NodeType val,bool op, double eps);

	/** \see SetNode */
	virtual void operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType valin,NodeType valout, bool op,double eps);

	/** \see SetNode */
	virtual SetNode * fakeLeaf(const IntervalVector& box,const IntervalVector& subbox,NodeType val,bool op, double eps);

	/** \see SetNode */
	virtual void cleave(const IntervalVector& box, Sep& sep, double eps);

	/** \see SetNode */
	virtual void gather(bool go_up);

	/** \see SetNode */
	virtual void cutDeadBranch();

	/** \see SetNode */
	virtual void checkFat();

	/** \see SetNode */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const;

	/** \see SetNode */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const;

	/** \see SetNode */
	virtual void setFathers();





	/**
	 * \brief Rerturn left_box according to var, pt and nodebox
	 */
	IntervalVector left_box(const IntervalVector& nodebox) const;

	/**
	 * \brief Rerturn left_box according to var, pt and nodebox
	 */
	IntervalVector right_box(const IntervalVector& nodebox) const;

//protected:
	friend class SetNode;
	friend class SetInterval;

	// partial initialization used by SetInterval::load only
	SetBisect(int var, double pt);


	int var;
	double pt;
	SetNode* left;
	SetNode* right;

private:
	SetBisect(const SetBisect&); // forbidden
};

} // namespace ibex

#endif // __IBEX_SET_BISECT_H__
