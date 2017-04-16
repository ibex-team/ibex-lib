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

	SetLeaf(NodeType status,SetNode* father);

	/**
	 * \brief Delete this.
	 */
	virtual ~SetLeaf();

	/** \see SetNode */
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


private:
	SetLeaf(const SetLeaf&); // forbidden

};

} // namespace ibex

#endif // __IBEX_SET_LEAF_H__
