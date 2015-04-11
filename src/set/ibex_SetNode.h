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
 * \ingroup iset
 * \brief Exception thrown by sync function.
 *
 * use to compute regular cutting of a interval
 */
class vcut{
	int var;
	double pt;
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
	 * \brief Creates a node of given status and father
	 */
	 SetNode(NodeType status,SetNode *father);


	/**
	 * \brief Delete this.
	 */
	virtual ~SetNode();

	/**
	 * \brief True iff this node is a leaf.
	 */
	virtual bool is_leaf() const=0;

	/**
	 * \brief Return a copy of the node.
	 */
	virtual SetNode * copy() const=0;


	/**
	 * \brief Intersection or Union between a box of NodeType val and a regular setInterval.
	 * Box may not respect the regularity of the regular setInterval.
	 * The part of the set that intersect nodebox only is modify.
	 */
	virtual void operator_ir(const IntervalVector& box,const IntervalVector& nodebox, NodeType val, bool op, double eps)=0;

	/**
	 * \brief Call by operator_ir, create a subtree from a leaf with "fake leves" that are setbisect with left and right set to null
	 * should avoid to create leaf and eventually destruct it if it need to be cut i.e replace by a setbisect
	 * method abandonned, oddly slower than creating leaves and replace them by setbisect.
	 */
	virtual SetNode * fakeLeaf(const IntervalVector& box,const IntervalVector& subbox,NodeType val,bool op, double eps)=0;

	/**
	 * \brief Overloaded function, apply valout to the set outside of the box and valin to the inside, thus modify the whole set.
	 */
	virtual void operator_ir(const IntervalVector& box,const IntervalVector& subbox, NodeType valin,NodeType valout, bool op,double eps)=0;

	/**
	 * \brief Intersection between two setNode.
	 */
	virtual void oper(SetNode* other,bool op)=0;

	/**
	 * \brief Intersection between setnode and a status.
	 */
	virtual void inter(NodeType x_status)=0;

	/**
	 * \brief Union between setnode and a status.
	 */
	virtual void _union(NodeType x)=0;

	/**
	 * \brief Change value of branch if right and left got the same value IN, OUT or UNK.
	 */
	virtual void gather(bool go_up)=0;
	/**
	 * \brief Delete the branch if left and right got the same value IN, OUT or UNK.
	 */
	virtual void cutDeadBranch()=0;

	/**
	 * \brief Check if fathers are initialized.
	 */
	virtual void checkFat()=0;

	/**
	 * \brief Contrat i-set w.r.t separator sep, split leaves until boxin and boxout returned by sep are disjoints, 
	 * and then call operator_ir to contract on them
	 */	
	virtual void cleave(const IntervalVector& box, Sep& sep,double eps)=0;

	/**
	 * \brief Visit the leaves of a tree with a callback "func"
	 */
	virtual void visit_leaves(leaf_func func, const IntervalVector& nodebox) const=0;

	/**
	 * \brief Display the structure on output stream "os"
	 */
	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const=0;

	/**
	 * \brief Set fathers of the nodes
	 */
	virtual void setFathers()=0;



	/**
	 * \brief The status of the node
	 */
	NodeType status;
	SetNode * father;
};


} // namespace ibex

#endif // __IBEX_SET_NODE_H__
