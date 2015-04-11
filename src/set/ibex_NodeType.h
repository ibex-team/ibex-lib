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
 * __IBEX_IN__       : the node box is inside the set
 * __IBEX_OUT__      : the node box is outside the set
 * __IBEX_UNK__      : the node is of unknown status (typically, corresponds to a boundary box)
 * __IBEX_UNK_IN     : the subtree contains IN and UNK leaves
 * __IBEX_UNK_OUT    : the subtree contains OUT and UNK leaves
 * __IBEX_UNK_IN_OUT : the subtree contains IN, OUT and UNK leaves
 * __IBEX_IN_TMP__   : used by "inter" functions only. Means "inside the original set" (but not necessarily inside the intersection)
 */
typedef enum { __IBEX_IN__,
	           __IBEX_OUT__,
	           __IBEX_UNK__,
	           __IBEX_UNK_IN__,
	           __IBEX_UNK_OUT__,
	           __IBEX_UNK_IN_OUT__,
	           __IBEX_IN_TMP__ } NodeType;

/**
 * \brief Status of a union of two nodes.
 *
 * \note Status only useful for "sync" functions. So the IN_TMP status is ignored (and this makes no problem so far).
 *
 * Ex: __IBEX_IN__ | __IBEX_UNK__ gives __IBEX_IN_UNK__.
 *
 * \see SetBisect constructor.
 */
NodeType operator|(NodeType x, NodeType y);
/**
 * \brief return value of union of two equal boxes
 */
NodeType uni(NodeType x,NodeType y);
/**
 * \brief return value of union of two boxes, assuming the on associated to y is include in the one associated to x
 */
NodeType uni_in(NodeType x, NodeType y);
/**
 * \brief return value of inter of two equal boxes
 */
NodeType inte(NodeType x, NodeType y);
/**
 * \brief return value of inter of two boxes, assuming the on associated to y is include in the one associated to x
 */
NodeType inte_in(NodeType x, NodeType y);
/**
 * \brief return value of inter of two boxes, assuming the on associated to y is include in the one associated to x
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
