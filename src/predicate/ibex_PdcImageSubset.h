//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcImageSubset.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 21, 2012
// Last Update : Dec 21, 2012
//============================================================================

#ifndef __IBEX_PDC_IMAGE_SUBSET_H__
#define __IBEX_PDC_IMAGE_SUBSET_H__

#include "ibex_Pdc.h"
#include "ibex_Function.h"

namespace ibex {

/** \ingroup predicate
 *
 * \brief Test if a box is inside the range of a function over an (implicit) set.
 *
 * Based on the algorithm by Goldsztejn & Jaulin. See p.12 of the paper
 * "Inner Approximation of the Range of Vector-valued Functions"
 * Reliable Computing, 2010.
 *
 * \precondition - the function f must be "square" (from R^n to R^n).
 */
class PdcImageSubset : public Pdc {
public:
	/*
	 * Build the predicate that will check if
	 * [y] is inside the set of all f(x), for x in [x0]\cap S,
	 * where S is typically a set defined implicitly by a constraint.
	 *
	 * <p>
	 * The predicate p_in will represent this constraint.
	 *
	 * \param f - A function from R^n to R^n. Given by reference.
	 * \param x0 - The "source" box in IR^n. Given by reference.
	 * \param p_in - A predicate that returns YES only if the box [x] is inside S. Given by reference.
	 */
	PdcImageSubset(Function& f, const IntervalVector& x0, Pdc& p_in);

	/**
	 * \param A box ([x],[y]) such that [x] is inside [x0] and [y] is an outer approximation of range(f,[x]).
	 *
	 * \return YES if it could been proved that [y] is inside range(f,[x0]\cap S), MAYBE otherwise.
	 */
	virtual BoolInterval test(const IntervalVector& box);

	// The function
	Function& f;

	// The initial box we want to calculate the image by f
	// (of all the points also being inside S).
	const IntervalVector& x0;

	// The predicate
	Pdc& p_in;
};

} // end namespace ibex
#endif // __IBEX_PDC_IMAGE_SUBSET_H__
