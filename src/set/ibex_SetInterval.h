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
	 * \brief Creates a copy of an existing setInterval
	 *
	 */
	SetInterval(const SetInterval& set);


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


	/*
	 * \brief Replace the current root by a setbisect pointing on 2 leaves with the root status
	 */	
	void cutRoot();

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
	 * \brief i-Set Intersection
	 *
	 * Intersection of a setInterval with a box of interior value valin and exterior value valout. 
	 * Computation is more efficient than create a setInterval contracted on the box, and then
	 * compute intersection of the two setInterval. 
	 */
	SetInterval& interBox(const IntervalVector& box,NodeType valin,NodeType valout);

	/**
	 * \brief i-Set Intersection
	 *
	 * Union of a setInterval with a box of interior value valin and exterior value valout. 
	 * Computation is more efficient than create a setInterval contracted on the box, and then
	 * compute intersection of the two setInterval. 
	 */
	SetInterval& unionBox(const IntervalVector& box,NodeType valin,NodeType valout);

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
	 * \brief gather leaves that have the same value
	 */
	void gather();

	/**
	 * \brief Check if fathers are initialized
	 */
	void checkFat();

	/**
	 * \brief True if this i-set is empty
	 *
	 * \warning: an empty i-set is different from a i-set containing (and possibly only containing) the empty set.
	 */
	bool is_empty() const;

	/**
	 * \brief Contrat i-set w.r.t a separator sep
	 */
	void contract(Sep& sep);

	/**
	 * \brief Serialize the set and save it into a file
	 */
	void save(const char* filename);

	void visit_leaves(SetNode::leaf_func func) const;

	/**
	 * \brief Distance of the point "pt" wrt the set (if inside is true)
	 * of the complementary of the set (if inside is false).
	 */
	double dist(const Vector& pt, bool inside) const;

	

	

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
