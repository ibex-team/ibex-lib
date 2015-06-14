//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#ifndef __IBEX_SET_H__
#define __IBEX_SET_H__

#include "ibex_SetNode.h"
#include "ibex_SetLeaf.h"
#include "ibex_BoolInterval.h"
#include "ibex_SetVisitor.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \defgroup iset Set
 */

/**
 * \ingroup iset
 * \brief Interval-based representation of a set
 *
 */
class Set {
public:

	/**
	 * \brief Creates a n-dimensional set (-oo,oo)x...x(-oo,oo)
	 */
	Set(int n, BoolInterval status=YES);

	/**
	 * \brief Creates a set from a simple box
	 */
	Set(const IntervalVector& box, BoolInterval status=YES);

	/**
	 * \brief Loads a set from a data file.
	 *
	 * \see #save().
	 */
	Set(const char* filename);

	/**
	 * \brief Build the set (f(x) op 0).
	 */
	Set(Function& f, CmpOp op, double eps);

	/**
	 * \brief Build the set from a constraint.
	 */
	Set(NumConstraint& ctr, double eps);

	/**
	 * \brief Build the set from a system of constraints.
	 */
	Set(const System& sys, double eps);

	/*
	 * \brief Delete this
	 */
	~Set();

	/**
	 * \brief Intersection with another set
	 *
	 * If [x] designates this set and [y] the set in argument, then this set
	 * will be replaced by { x \cap y, x\in[x] and y\in[y] }.
	 *
	 * Note: In Jaulin's terminology, this operator is the "i-set extension of the intersection".
	 *
	 */
	Set& operator&=(const Set& set);

	/**
	 * \brief i-Set Union
	 *
	 * If [x] designates this set and [y] the set in argument, then this set
	 * will be replaced by { x \cup y, x\in[x] and y\in[y] }.
	 *
	 * Note: In Jaulin's terminology, this operator is the "i-set extension of the union".
	 */
	Set& operator|=(const Set& set);

	/**
	 * \brief True if this set is empty
	 *
	 * \warning: not to be confused with an empty interval set.
	 */
	bool is_empty() const;

	/**
	 * \brief Serialize the set and save it into a file
	 */
	void save(const char* filename);

	/**
	 * \brief Visit the set
	 */
	void visit(SetVisitor& visitor) const;

	/**
	 * \brief Distance of the point "pt" wrt the set (if inside is true)
	 * of the complementary of the set (if inside is false).
	 *
	 */
	double dist(const Vector& pt, bool inside) const;

	/**
	 * \brief The leaves organized by connected components.
	 *
	 * Two leaves belong to the same connected component if there exists
	 * a path between the two boxes that either entirely belong to the
	 * "inside" of the set (if the leaves have status YES), the "outside"
	 * (if the status is NO) or the "boundary".
	 *
	 * The number of connected components is the size of the returned vector.
	 */
	std::vector<std::vector<SetLeaf*> > connected_components();

	/**
	 * \brief YES only if this set is a superset of the box
	 */
	BoolInterval is_superset(const IntervalVector& box) const;

	/**
	 * \brief Box corresponding to a node
	 */
	IntervalVector node_box(const SetNode* node) const;

protected:
	friend class Sep;

	/**
	 * \brief Inflate a box by one float.
	 */
	static IntervalVector inflate_one_float(const IntervalVector& box);

	/**
	 * \brief Load the set from a file
	 */
	void load(const char* filename);

	friend std::ostream& operator<<(std::ostream& os, const Set& set);

	/**
	 * Root node.
	 *
	 * NULL means no existing set (warning: different from empty set!)
	 */
	SetNode* root;

	/**
	 * (-oo,oo)x..x(-oo,oo)
	 */
	IntervalVector Rn;
};

std::ostream& operator<<(std::ostream& os, const Set& set);

} // namespace ibex

#endif // __IBEX_SET_H__
