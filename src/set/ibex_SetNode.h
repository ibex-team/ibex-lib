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
#include "ibex_Bracket.h"

namespace ibex {

// IN_TMP used by inter only. means inside the original set (but not necessarily inside the intersection)
typedef enum { IN, OUT, UNK, UNK_IN, UNK_OUT, UNK_IN_OUT, IN_TMP } SetType;

SetType operator|(SetType x, SetType y);

bool possibly_contains_in(SetType x);

bool possibly_contains_out(SetType x);

bool certainly_contains_in(SetType x);

bool certainly_contains_out(SetType x);

const char* color(SetType status);

/**
 * \brief Exception thrown by sync function.
 *
 * Thrown when the set interval is empty (<=> no set exists).
 */
class NoSet {

};

class Set;

/**
 * \brief Set node.
 */
class SetNode {

public:
	typedef const char* (*color_code) (SetType);

	SetNode(SetType status);

	virtual ~SetNode();

	SetType status;

	virtual bool is_leaf() const=0;

	SetNode* sync(const IntervalVector& nodebox, Bracket& br, double eps);

	/**
	 * skip_other_maybe: don't consider UNK box of the other set. This is important
	 *                   because the other set may be a temporary set produced by
	 *                   the contract function (and the UNK box will be refined later)
	 */
	SetNode* sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps, bool skip_other_maybe);

	virtual SetNode* sync(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps)=0;

	virtual SetNode* sync_rec(const IntervalVector& nodebox, Bracket& br, double eps)=0;

	SetNode* inter(const IntervalVector& nodebox, Bracket& br, double eps);

	SetNode* inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps);

	virtual SetNode* inter(const IntervalVector& nodebox, const IntervalVector& x, SetType x_status, double eps)=0;

	virtual SetNode* inter_rec(const IntervalVector& nodebox, Bracket& br, double eps)=0;

	virtual void to_vibes(color_code color_func, const IntervalVector& nodebox) const=0;

	virtual void print(std::ostream& os, const IntervalVector& nodebox, int shift) const=0;

	/**
	 * Set all "IN" leaves to "IN_TMP".
	 *
	 * This function is useful for calculating set intersection only.
	 * It sets all the inner nodes of the original set to IN_TMP before.
	 * This allows to distinguish a node that is inside the original set (IN_TMP)
	 * from a node that is proven to be inside the intersection (IN).
	 */
	virtual void set_in_tmp()=0;

	/**
	 * Set all "IN_TMP" leaves to "UNK"
	 *
	 * This function is useful for calculating set intersection only.
	 * Once the intersection is calculated, the nodes that was in the original
	 * set but that could not be proven to be inside the intersection
	 * have "UNK" status.
	 */
	virtual void unset_in_tmp()=0;
};

char to_string(const SetType& status);

SetNode* diff(const IntervalVector& x, const IntervalVector& y, SetType x_status, SetType y_status, double eps);

SetNode* contract_set(const IntervalVector& x, Ctc& c, SetType x_status, SetType y_status, double eps);

} // namespace ibex

#endif // __IBEX_SET_NODE_H__
