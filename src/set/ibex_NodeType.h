//============================================================================
//                                  I B E X
// File        : ibex_NodeType.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 18 Aug 2014
//============================================================================

#ifndef __IBEX_NODE_TYPE_H__
#define __IBEX_NODE_TYPE_H__

namespace ibex {

/** \ingroup iset */
/*@{*/

/**
 * \brief Status of a node in the representation of an i-set.
 *
 * __IBEX_IN__       : the node is inside the set
 * __IBEX_OUT__      : the node is outside the set
 * __IBEX_UNK__      : the node is of unknown status (typically, corresponds to a boundary box)
 * __IBEX_UNK_IN     : the node intersects the set
 * __IBEX_UNK_OUT    : the node intersects the complementary of the set
 * __IBEX_UNK_IN_OUT : the node intersects the set and its complementary
 */
typedef enum { __IBEX_IN__,
	           __IBEX_OUT__,
	           __IBEX_UNK__,
	           __IBEX_UNK_IN__,
	           __IBEX_UNK_OUT__,
	           __IBEX_UNK_IN_OUT__ } NodeType;


/**
 * \brief Relation between two equi-dimensional sets
 */
typedef enum { __IBEX_DISJOINT__,
		       __IBEX_OVERLAP__,
               __IBEX_SUPSET__,
               __IBEX_SUBSET__,
               __IBEX_SET_EQ__ } SetRelation;

/**
 * \brief Status of the union of two nodes.
 *
 * Given two nodes n1 and n2 of status s1 and s2 w.r.t. the set S,
 * the result is the status of (n1 union n2) w.r.t. S.
 *
 * Note.
 * 1/ This function assumes the sets n1 and n2 are associated to the
 * the same. The result is not the status of (n1 union n2) w.r.t. (S1 union S2)
 * where S1 (resp. S2) is the set n1 (resp. n2) is associated to.
 * E.g.: n1=[0,1] is associated to S1=[0,3] and n2=[0,1] to [2,3].
 * If s1=IN and s2=OUT, the result is UNK_IN_OUT which is wrong because (n1 union n2)=[0,1]
 * does not intersect the complementary of [0,3].
 *
 * 2/ The result is the status of the union and not the disjunction of the status.
 *
 *    status(n1) v status(n2) <> status(n1 union n2)
 *
 * E.g., if s1=IN and s2=OUT, the disjunction of the status is "either IN or OUT" (the node is
 * either fully inside or outside) but the result will be UNK_IN_OUT (contain both points inside
 * and outside) which is not an over-approximation but a wrong result.
 *
 * 3/ If we have a particular set relation between n1 and n2, like n1\subset n2 or n1=n2, this
 * results give an overestimation of the true result. See sync.
 *
 * \see SetBisect constructor.
 */
NodeType operator|(NodeType x, NodeType y);
//NodeType _union(SetRelation rel, NodeType x, NodeType y);

/**
 * \brief Status of the intersection of two nodes.
 *
 * Given two nodes n1 and n2 of status s1 and s2 w.r.t. the sets S1
 * and S2, the result is the status of (n1 inter n2) w.r.t. (S1 inter S2).
 *
 * Ex: __IBEX_IN__ & __IBEX_UNK_OUT__ gives __IBEX_UNK_OUT__ if n2 \subset n1
 *     or __UNK__ otherwise.
 *
 * \see SetBisect constructor.
 */
NodeType inter(SetRelation rel, NodeType x, NodeType y);

/**
 * \brief Status of the intersection of two nodes.
 *
 * Given two nodes n1 and n2 of status s1 and s2 w.r.t. the set S
 * and S2, the result is the status of (n1 inter n2) w.r.t. S

 * Ex: __IBEX_IN__ & __IBEX_UNK_OUT__ gives __IBEX_IN__.
 *     __IBEX_IN__ & __IBEX_OUT__ throws NoSet.
 *
 */
NodeType sync(SetRelation rel, NodeType x, NodeType y);


/**
 * \brief Status of the subset of a node
 *
 * Ex: subset(__IBEX_UNK_IN__) = __IBEX_UNK__
 *
 * \see SetBisect constructor.
 */
NodeType subset(NodeType x);


/**
 * \brief False only if the subtree contains no inner point
 */
bool possibly_contains_in(NodeType x);

/**
 * \brief False only if the subtree contains no outer point
 */
bool possibly_contains_out(NodeType x);

/**
 * \brief True only if the subtree contains inner points
 */
bool certainly_contains_in(NodeType x);

/**
 * \brief True only if the subtree contains outer points
 */
bool certainly_contains_out(NodeType x);

/**
 * \brief Convert the status to char
 */
char to_string(const NodeType& status);

/*@}*/

} // namespace ibex

#endif // __IBEX_NODE_TYPE_H__
