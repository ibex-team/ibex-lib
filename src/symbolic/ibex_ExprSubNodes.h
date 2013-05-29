//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSubNodes.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_EXPR_SUB_NODES_H__
#define __IBEX_EXPR_SUB_NODES_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * \brief All the (sub)nodes of an expression (including itself)
 *        sorted by topological order (the first is the root of the expression)
 *
 * Each sub-node appears only once in the set.
 * The size of the set is also root.size().
 */
class SubNodes {
public:
	/**
	 * \brief Build the subnodes of e.
	 */
	SubNodes(const ExprNode& e);

	/**
	 * \brief Build an uinitialized object (must be followed by a call to #init).
	 */
	SubNodes();

	/**
	 * \brief Delete *this.
	 */
	~SubNodes();

	/**
	 * \brief Build *this (as the subnodes of e).
	 */
	void init(const ExprNode& e);

	/**
	 * \brief Number of subnodes.
	 */
	int size() const;

	/**
	 * Return the ith subnode, by decreasing height.
	 */
	const ExprNode& operator[](int i) const;

	/**
	 * True iff e is a subnode.
	 */
	bool found(const ExprNode& e) const;

private:
	friend class ExprNodes;
	const ExprNode** tab;
	// map a node to its index in the array
	NodeMap<int> map;
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline const ExprNode& SubNodes::operator[](int i) const {
	return *tab[i];
}

inline int SubNodes::size() const {
	return tab[0]->size;
}

inline bool SubNodes::found(const ExprNode& e) const {
	return map.found(e);
}

} // end namespace ibex
#endif // __IBEX_EXPR_SUB_NODES_H__
