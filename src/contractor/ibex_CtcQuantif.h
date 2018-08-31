//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcQuantif.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 21, 2014
//============================================================================

#ifndef __IBEX_CTC_QUANTIF_H__
#define __IBEX_CTC_QUANTIF_H__


#include "ibex_Ctc.h"
#include "ibex_LargestFirst.h"
#include "ibex_NumConstraint.h"
#include "ibex_BitSet.h"
#include "ibex_VarSet.h"

namespace ibex {

/**
 * \brief Abstract contractor for quantified constraints (proj-union/proj-inter)
 */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
class CtcQuantif : public Ctc {

public:
	/**
	 * \brief Create the contractor for "exists/forall y in [y_init] such that c(x,y)"
	 *
	 * The constraint c must take n+m arguments where n is the size of x and m the size of y.
	 * Contrary to what the notation c(x,y) suggests, symbols y1...ym does not necessarily
	 * need to appear after the symbols x1...xn in c.
	 *
	 * \param c       The constraint
	 * \param vars    Structure to separate variables and parameters
	 * \param y_init  Initial box for the parameters
	 * \param prec    Bisection precision on the parameters (the contraction involves a
	 *                bisection process on y)
	 */
	CtcQuantif(const NumConstraint& c, const VarSet& vars, const IntervalVector& y_init, double prec);

	/**
	 * \brief Proj-union/Proj-inter operator applied on the contractor c.
	 *
	 * This is the generalization of the quantified constraint proposed in "Contractor programming".
	 * The constraint is implicitly represented by a contractor.
	 *
	 * \param c       The contractor
	 * \param vars    Structure to separate variables and parameters
	 * \param y_init  Initial box for the parameters
	 * \param prec    Bisection precision on the parameters
	 * \param own_ctc Boolean which indicate if the ctc mast be destroy with this
	 */
	CtcQuantif(Ctc& c, const VarSet& vars, const IntervalVector& y_init, double prec, bool own_ctc=false);

	/**
	 * \brief Delete this.
	 */
	virtual ~CtcQuantif();

	/**
	 * \brief Initial box of the parameters (can be set dynamically)
	 */
	IntervalVector y_init;

protected:
	/**
	 * \brief Contract the "full" box (x,y)
	 *
	 * \param x the vector of variables
	 * \param y the vector parameters.
	 *
	 * Return true if the contractor is inactive.
	 */
	bool contract(IntervalVector& x, IntervalVector& y);

	/**
	 * \brief The Contractor.
	 */
	Ctc* ctc;

	/**
	 * \brief a bisector
	 */
	LargestFirst* bsc;

	/**
	 * vars[i]=true <=> the ith component is a variable ("x_k")
	 * Otherwise, the ith component is a parameter ("y_k")
	 */
	VarSet vars;

	/**
	 * \brief precision
	 */
	double prec;

private:

	/* Information for cleanup only */
	bool _own_ctc;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
} // namespace ibex
#endif // __IBEX_CTC_QUANTIF_H__
