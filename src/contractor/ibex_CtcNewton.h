//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcNewton.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#ifndef __IBEX_CTC_NEWTON_H__
#define __IBEX_CTC_NEWTON_H__

#include "ibex_Ctc.h"
#include "ibex_Newton.h"
#include "ibex_VarSet.h"

namespace ibex {

/** \ingroup contractor
 * \brief Newton contractor.
 *
 **/
class CtcNewton : public Ctc {
public:
	/**
	 * See #ibex::newton(const Fnc&, IntervalVector&, double, double);
	 * \param ceil - Criterion for applying interval Newton contractor.
	 *  When the diameter of all components of the current box is smaller than \a ceil,
	 *  the interval Newton method is applied. This criterion is useful to avoid useless
	 *  computations of the Jacobian matrix for wide boxes.
	 *
	 *  Default value is #default_ceil.
	 */
	CtcNewton(const Fnc& f,
			double ceil=default_ceil,
			double prec=default_newton_prec,
			double ratio=default_gauss_seidel_ratio);


	/**
	 * Variant where only a sub-set of variables is considered
	 */
	CtcNewton(const Fnc& f, const VarSet& vars,
			double ceil=default_ceil,
			double prec=default_newton_prec,
			double ratio=default_gauss_seidel_ratio);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	void contract(IntervalVector& box, ContractContext& context);

	/** The function. */
	const Fnc& f;

	/**
	 * The sub-set of variables on which Newton is applied
	 *
	 * The other are considered as parameters.
	 * NULL means "all".
	 */
	const VarSet* vars;

	/** Newton application ceiling. */
	const double ceil;
	/** Precision. See #ibex::newton(const Fnc&, IntervalVector&, double, double);*/
	const double prec;
	/** Gauss-Seidel ratio. See #ibex::newton(const Fnc&, IntervalVector&, double, double);*/
	const double gauss_seidel_ratio;

	/** Initialized to 0.01 */
	static constexpr double default_ceil = 0.01;

};

} // end namespace ibex
#endif // __IBEX_CTC_NEWTON_H__
