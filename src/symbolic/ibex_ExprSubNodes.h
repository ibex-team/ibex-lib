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
 * \brief All the (sub)nodes of an expression (including itself) sorted by
 * topological+DFS order.
 *
 * The "topological+DFS" order means that subnodes are first sorted with respect
 * to the topological order and then with respect to DFS (note: topological
 * order is not unique). However, when using the constructor variant with two
 * arguments (x and f(x)), the final relative ordering of variables is that of x,
 * not DFS.
 *
 * Each sub-node appears only once in the set.
 * The first subnode in the resulting vector is the root of the expression.
 * The size of the set is also root.size().
 */
class ExprSubNodes {
public:
	/**
	 * \brief Build the subnodes of e.
	 */
	ExprSubNodes(const ExprNode& e);

	/**
	 * \brief Build the subnodes of several expressions.
	 *
	 * Subnodes in common appear only once.
	 */
	ExprSubNodes(Array<const ExprNode> exprs);

	/**
	 * \brief Build the subnodes of x and f_x.
	 *
	 * Example:<br>
	 * with x=(a,b,c) and f_x=(c+b)-a<br>
	 * the result is<br>
	 * [ (c+b)-a , (c+b) , a , b , c ]
	 *
	 * \param symb_DFS: if true, DFS order applies among symbols, which implies
	 *        that all unused symbols appear at the end of the nodes array.
	 *        If false, the initial ordering (that of x) is respected and all
	 *        symbols appear at the end of the nodes array.
	 *
	 */
	ExprSubNodes(const Array<const ExprSymbol>& x, const ExprNode& y, bool symb_DFS=false);

	/**
	 * \brief Build an uinitialized object (must be followed by a call to #init).
	 */
	ExprSubNodes();

	/**
	 * \brief Delete *this.
	 */
	~ExprSubNodes();

	/**
	 * \brief Build *this (as the subnodes of e).
	 */
//	void init(const ExprNode& e);

	/**
	 * \brief Build *this (as the subnodes of x and f_x).
	 * \see comments in constructor.
	 * \note: the parameter symb_DFS has no effect if x is NULL.
	 */
	void init(const Array<const ExprSymbol>* x, const Array<const ExprNode>& y, bool symb_DFS);

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

	/**
	 * \brief Return the rank of a node
	 *
	 * This corresponds more or less to the rank of a node in
	 * a Polish prefix notation of the expression
	 */
	int rank(const ExprNode& e) const;

private:
	const ExprNode** tab;
	int _size;
	// map a node to its index in the array
	NodeMap<int> map;
};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline const ExprNode& ExprSubNodes::operator[](int i) const {
	return *tab[i];
}

inline int ExprSubNodes::size() const {
	return _size;
}

inline bool ExprSubNodes::found(const ExprNode& e) const {
	return map.found(e);
}

inline int ExprSubNodes::rank(const ExprNode& e) const {
	return map[e];
}

} // end namespace ibex
#endif // __IBEX_EXPR_SUB_NODES_H__
