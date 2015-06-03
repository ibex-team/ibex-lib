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
#include "ibex_Sep.h"
#include "ibex_SetLeaf.h"

namespace ibex {

/**
 * \defgroup iset Set Interval
 */

/**
 * \ingroup iset
 * \brief Set Interval
 *
 * See "Solving set-valued constraint satisfaction problem", Luc Jaulin, Computing. Volume 94, Issue 2, Page 297-311.
 *
 */
class SetInterval {
public:

	/**
	 * \brief Creates a set interval from a simple box
	 *
	 * If x is the bounding box in argument, the set interval is either
	 * [empty, x] if inner==false or [x,x] if inner==false.
	 */
	SetInterval(const IntervalVector& bounding_box, double eps, bool inner=true);


	/**
	 * \brief Loads a set from a data file.
	 *
	 * \see #save().
	 */
	SetInterval(const char* filename);

	/*
	 * \brief Delete this
	 */
	~SetInterval();

	/**
	 * \brief i-Set Intersection
	 *
	 * In Jaulin's terminology, this operator is the "i-set extension of the intersection".
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x \cap y, x\in[x] and y\in[y] }.
	 */
	SetInterval& operator&=(const SetInterval& set);

	/**
	 * \brief i-Set Union
	 *
	 * In Jaulin's terminology, this operator is the "i-set extension of the union".
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x \cup y, x\in[x] and y\in[y] }.
	 */
	SetInterval& operator|=(const SetInterval& set);

	/**
	 * \brief i-Set synchronization
	 *
	 * In Jaulin's terminology, this operator is the "intersection of i-sets" (squared symbol)
	 *
	 * If [x] designates this i-set and [y] the i-set in argument, then this will be replace by
	 *  { x, x\in[x] and x\in[y] }.
	 */
	void sync(Sep& sep);

	/**
	 * \brief True if this i-set is empty
	 *
	 * \warning: an empty i-set is different from a i-set containing (and possibly only containing) the empty set.
	 */
	bool is_empty() const;

	void contract(Sep& sep);

	/**
	 * \brief Serialize the set and save it into a file
	 */
	void save(const char* filename);

	void visit_leaves(SetNode::leaf_func func) const;

	/**
	 * \brief Distance of the point "pt" wrt the set (if inside is true)
	 * of the complementary of the set (if inside is false).
	 *
	 * \note If inside==false and the pt is outside the bounding box,
     *       the distance is 0.
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

	/**
	 * \brief Load the set from a file
	 */
	void load(const char* filename);

	friend std::ostream& operator<<(std::ostream& os, const SetInterval& set);

	SetNode* root; // NULL means no existing set (warning: different from empty set!)

	double eps;

	IntervalVector bounding_box; // not sure it is really necessary

};

std::ostream& operator<<(std::ostream& os, const SetInterval& set);

} // namespace ibex

#endif // __IBEX_SET_H__
