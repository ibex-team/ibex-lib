//============================================================================
//                                  I B E X                                   
// File        : ibex_SetImage.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_SET_IMAGE_H__
#define __IBEX_SET_IMAGE_H__

#include "ibex_Function.h"
#include "ibex_Ctc.h"
#include "ibex_Pdc.h"
#include <vector>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Set image using Goldsztejn-Jaulin algorithm (Reliable Computing, 2010).
 *
 * Based on the algorithm by Goldsztejn & Jaulin. See
 * "Inner Approximation of the Range of Vector-valued Functions"
 * Reliable Computing, 2010.
 *
 * Note: the bisection procedure is fixed to LargestFirst.
 *
 * \param f - A function from R^n to R^n
 * \param x - The box in IR^n
 *
 */
class SetImage {
public:
	/**
	 * Build the set image algorithm for the given function and
	 * for the domain defined by to "bracketing" contractors.
	 *
	 * \param c_out - Contractor that removes infeasible points
	 * \param p_in -  A predicate that return true iff the box is an inner box
	 *
	 */
	SetImage(Function& f, Ctc& c_out, Pdc& p_in);

	/**
	 * Build the set image algorithm for the function f
	 * and the domain x.
	 */
	SetImage(Function& f, const IntervalVector& x);

	/**
	 * Run the set image algorithm that will generate
	 * two subpavings, one for the interior (returned by
	 * #interior()) and one for the boundary (returned by
	 * #boundary()).
	 *
	 * \param epsilon - the precision up to which the x box will be bisected by the algorithm.
	 */
	void pave(const IntervalVector& x, double epsilon);

	/**
	 * Return the set of boxes proven to be inside the
	 * interior of range(f,x) by the previous call to
	 * #pave(const IntervalVector&).
	 *
	 * \note the vector is empty if pave() has not
	 * bee executed yet.
	 */
	const std::vector<IntervalVector>& interior();

	/**
	 * Return a set S of boxes such that
	 * range(f,x) is included in the union of interior() and S.
	 *
	 * \note the vector is empty if pave() has not
	 * bee executed yet.
	 */
	const std::vector<IntervalVector>& boundary();

private:
	Function &f;
	int n;

	Ctc &c_out;
	Pdc &p_in;

	std::vector<IntervalVector> Linside;
	std::vector<IntervalVector> Lboundary;
};

inline const std::vector<IntervalVector>& SetImage::interior() {
	return Linside;
}

inline const std::vector<IntervalVector>& SetImage::boundary() {
	return Lboundary;
}

} // end namespace ibex
#endif // __IBEX_SET_IMAGE_H__
